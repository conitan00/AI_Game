#pragma once

// 畳み込み層
class Convolution2D : public Layer<vector<vector<vector<vector<long double>>>>, vector<vector<vector<vector<long double>>>>> {

private:

    Activation_4d activation;
    vector<vector<vector<vector<long double>>>> kernel; // 重み (出力チャネル数、入力チャネル数、縦、横)
    vector<long double> bias; // バイアス
    int kernel_size = 3;
    int padding = 1;
    int stride = 1;
    int in_channel;
    int out_channel;

    long double beta1 = 0.9;
    long double beta2 = 0.999;
    long double e = 1e-8;
    int t = 1;
    Vector4D m1_k;
    Vector4D m2_k;
    Vector1D m1_b;
    Vector1D m2_b;

    int H; // 縦
    int W; // 横
    vector<vector<vector<vector<long double>>>> last_input;

public:

    // コンストラクタ
    Convolution2D(int in_channel, int out_channel, int H, int W, ActivationID activationID) {

        // 活性化関数設定
        this->activationID = activationID;
        activation = Activation_4d(this->activationID);
        this->in_channel = in_channel;
        this->out_channel = out_channel;
        this->H = H;
        this->W = W;
        // パラメータ初期化
        double sigma = 0.05;
        if (this->activationID == ActivationID::relu) sigma = std::sqrt(2.0 / (double)in_channel);
        else if (this->activationID == ActivationID::sigmoid) sigma = std::sqrt(1.0 / (double)in_channel);
        std::random_device seed;
        std::mt19937 engine(seed());
        std::normal_distribution<> generator(0.0, sigma);

        for (int out = 0; out < out_channel; out++) {
            bias.push_back(generator(engine));
            vector<vector<vector<long double>>> res;
            for (int in = 0; in < in_channel; in++) {
                vector<vector<long double>> ver;
                for (int kers = 0; kers < kernel_size; kers++) {
                    vector<long double> hor;
                    for (int ker = 0; ker < kernel_size; ker++) {
                        hor.push_back(generator(engine));
                    }
                    ver.push_back(hor);
                }
                res.push_back(ver);
            }
            kernel.push_back(res);
        }

        m1_k = Vector4D(out_channel, Vector3D(in_channel, Vector2D(H, Vector1D(W, 0.0))));
        m2_k = Vector4D(out_channel, Vector3D(in_channel, Vector2D(H, Vector1D(W, 0.0))));
        m1_b = Vector1D(out_channel, 0.0);
        m2_b = Vector1D(out_channel, 0.0);

    }

    // 順伝播（1層分）
    vector<vector<vector<vector<long double>>>> forward(const vector<vector<vector<vector<long double>>>>& input) override {


        // パディング処理
        Vector4D padded_input(input.size(), Vector3D(in_channel, Vector2D(H + 2 * padding, Vector1D(W + 2 * padding, 0))));

        // 入力をパディング
        for (int b = 0; b < input.size(); b++) {
            for (int c = 0; c < in_channel; c++) {
                for (int h = 0; h < H; h++) {
                    for (int w = 0; w < W; w++) {
                        padded_input[b][c][h + padding][w + padding] = input[b][c][h][w];
                    }
                }
            }
        }

        last_input = padded_input; // 逆伝播のために、最新のデータを記録

        vector<vector<vector<vector<long double>>>> output;
        // バッチごとに処理
        for (auto& batch : padded_input) {
            vector<vector<vector<long double>>> batch_data;
            // 出力チャネルごとに処理
            for (int out = 0; out < out_channel; out++) {
                vector<vector<long double>> out_data(H, vector<long double>(W, 0));
                // 入力チャネルごとに加算
                for (int in = 0; in < in_channel; in++) {
                    for (int h = 0; h < H; h++) {
                        for (int w = 0; w < W; w++) {
                            for (int ker_h = 0; ker_h < kernel_size; ker_h++) {
                                for (int ker_w = 0; ker_w < kernel_size; ker_w++) {
                                    out_data[h][w] += batch[in][h + ker_h][w + ker_w] * kernel[out][in][ker_h][ker_w];
                                }
                            }
                        }
                    }
                }
                batch_data.push_back(out_data);
            }
            output.push_back(batch_data);
        }
        // 非線形変換（活性化関数）
        if (activationID != ActivationID::None) {
            output = activation.forward(output);
        }
        return output;
    }

    // 逆伝播
    vector<vector<vector<vector<long double>>>> backward(const vector<vector<vector<vector<long double>>>>& d_out) override {

        vector<vector<vector<vector<long double>>>> d_output = d_out;

        if (activationID != ActivationID::None) {
            d_output = activation.backward(d_output);
        }

        // 前の層に渡す勾配を格納する配列
        vector<vector<vector<vector<long double>>>> d_input(
            d_output.size(), vector<vector<vector<long double>>>(
                in_channel, vector<vector<long double>>(
                    H, vector<long double>(W, 0)
                    )
                )
        );

        vector<vector<vector<vector<long double>>>> d_input_padded(
            d_output.size(), vector<vector<vector<long double>>>(
                in_channel, vector<vector<long double>>(
                    H + 2 * padding, vector<long double>(W + 2 * padding, 0)
                    )
                )
        );

        vector<vector<vector<vector<long double>>>> d_kernel(
            out_channel, vector<vector<vector<long double>>>(
                in_channel, vector<vector<long double>>(
                    kernel_size, vector<long double>(kernel_size, 0)
                    )
                )
        );

        vector<long double> d_bias(out_channel, 0);

        // **1. バイアスの勾配の計算**
        for (int b = 0; b < d_output.size(); b++) {
            for (int out = 0; out < out_channel; out++) {
                for (int h = 0; h < H; h++) {
                    for (int w = 0; w < W; w++) {
                        d_bias[out] += d_output[b][out][h][w];  // チャネルごとに合計
                    }
                }
            }
        }

        // **2. カーネルの勾配の計算**
        for (int out = 0; out < out_channel; out++) {
            for (int in = 0; in < in_channel; in++) {
                for (int kh = 0; kh < kernel_size; kh++) {
                    for (int kw = 0; kw < kernel_size; kw++) {
                        long double sum = 0;
                        for (int b = 0; b < d_output.size(); b++) {
                            for (int h = 0; h < H; h++) {
                                for (int w = 0; w < W; w++) {
                                    sum += last_input[b][in][h * stride + kh][w * stride + kw] * d_output[b][out][h][w];
                                }
                            }
                        }
                        d_kernel[out][in][kh][kw] = sum;
                    }
                }
            }
        }

        // **3. 入力の勾配の計算**
        // カーネルを180°回転して、d_outputを「フル畳み込み」
        for (int b = 0; b < d_output.size(); b++) {
            for (int in = 0; in < in_channel; in++) {
                for (int h = 0; h < H; h++) {
                    for (int w = 0; w < W; w++) {
                        long double sum = 0;
                        for (int out = 0; out < out_channel; out++) {
                            for (int kh = 0; kh < kernel_size; kh++) {
                                for (int kw = 0; kw < kernel_size; kw++) {
                                    int h_idx = h + kh;
                                    int w_idx = w + kw;
                                    d_input_padded[b][in][h_idx][w_idx] +=
                                        d_output[b][out][h][w] * kernel[out][in][kernel_size - 1 - kh][kernel_size - 1 - kw];
                                }
                            }
                        }
                    }
                }
            }
        }

        for (int b = 0; b < d_output.size(); b++) {
            for (int in = 0; in < in_channel; in++) {
                for (int h = 0; h < H; h++) {
                    for (int w = 0; w < W; w++) {
                        d_input[b][in][h][w] = d_input_padded[b][in][h + padding][w + padding];
                    }
                }
            }
        }

        // パラメータ更新
        for (int out = 0; out < out_channel; out++) {
            m1_b[out] = beta1 * m1_b[out] + (1 - beta1) * d_bias[out];
            m2_b[out] = beta2 * m2_b[out] + (1 - beta2) * d_bias[out] * d_bias[out];
            long double m1b = m1_b[out] / (1 - pow(beta1, t));
            long double m2b = m2_b[out] / (1 - pow(beta2, t));
            bias[out] -= lr * (m1b / (sqrt(m2b) + e)); // バイアス
            //bias[out] -= lr * d_bias[out];
            for (int in = 0; in < in_channel; in++) {
                for (int h = 0; h < kernel_size; h++) {
                    for (int w = 0; w < kernel_size; w++) {
                        m1_k[out][in][h][w] = beta1 * m1_k[out][in][h][w] + (1 - beta1) * d_kernel[out][in][h][w];
                        m2_k[out][in][h][w] = beta2 * m2_k[out][in][h][w] + (1 - beta2) * d_kernel[out][in][h][w] * d_kernel[out][in][h][w];
                        long double m1k = m1_k[out][in][h][w] / (1 - pow(beta1, t));
                        long double m2k = m2_k[out][in][h][w] / (1 - pow(beta2, t));
                        kernel[out][in][h][w] -= lr * (m1k / (sqrt(m2k) + e)); // カーネル
                        //kernel[out][in][h][w] -= lr * d_kernel[out][in][h][w];
                    }
                }
            }
        }

        t++;

        return d_input;

    }

    void save_para(int layer_num) {

        string name = "para" + to_string(layer_num) + ".csv";
        ofstream file(name, ios::app);

        for (int out = 0; out < out_channel; out++) {
            for (int in = 0; in < in_channel; in++) {
                for (int h = 0; h < kernel_size; h++) {
                    for (int w = 0; w < kernel_size; w++) {
                        file << kernel[out][in][h][w] << "\n";
                    }
                }
            }
        }

        for (int out = 0; out < out_channel; out++) {
            file << bias[out] << "\n";
        }

    }

    void load_para(int layer_num) {
        string name = "para" + to_string(layer_num) + ".csv";
        ifstream file(name);

        for (int out = 0; out < out_channel; out++) {
            for (int in = 0; in < in_channel; in++) {
                for (int h = 0; h < kernel_size; h++) {
                    for (int w = 0; w < kernel_size; w++) {
                        file >> kernel[out][in][h][w];
                    }
                }
            }
        }

        for (int out = 0; out < out_channel; out++) {
            file >> bias[out];
        }
    }


};