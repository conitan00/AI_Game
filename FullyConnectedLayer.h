#pragma once

template <typename T, typename S>
// レイヤー
class Layer {

private:

public:

    float lr = 0.0001; // 学習率
    Activation activation;
    ActivationID activationID; // 活性化関数

    virtual ~Layer() = default;

    // 順伝播（1層分）
    virtual T forward(const S& input) = 0;

    // 順伝播（1層分）
    virtual S backward(const T& d_output) = 0;


};

// 全結合層
class FC : public Layer<vector<vector<long double>>, vector<vector<long double>>> {

private:

    vector<vector<long double>> weight; // 重み
    vector<long double> bias; // バイアス

    long double beta1 = 0.9;
    long double beta2 = 0.999;
    long double e = 1e-8;
    int t = 1;
    Vector2D m1_w;
    Vector2D m2_w;
    Vector1D m1_b;
    Vector1D m2_b;

    int in_num; // 入力数
    int out_num; // 出力数
    vector<vector<long double>> last_input;

public:
    vector<vector<long double>> grad_weight; // 勾配(重み)
    vector<long double> grad_bias; // 勾配(バイアス)

    // コンストラクタ
    FC(int in_num, int out_num, ActivationID activationID) {

        grad_weight.resize(out_num, vector<long double>(in_num, 0));
        grad_bias.resize(out_num, 0);

        // 活性化関数設定
        this->activationID = activationID;
        activation = Activation(this->activationID);
        this->in_num = in_num;
        this->out_num = out_num;
        // パラメータ初期化
        double sigma = 0.05;
        if (this->activationID == ActivationID::relu) sigma = std::sqrt(2.0 / (double)in_num);
        else if (this->activationID == ActivationID::sigmoid) sigma = std::sqrt(1.0 / (double)in_num);
        std::random_device seed;
        std::mt19937 engine(seed());
        std::normal_distribution<> generator(0.0, sigma);

        for (int out = 0; out < out_num; out++) {
            bias.push_back(generator(engine));
            m1_b.push_back(0.0);
            m2_b.push_back(0.0);
            vector<long double> weights;
            vector<long double> m1_w_tmp;
            vector<long double> m2_w_tmp;
            for (int in = 0; in < in_num; in++) {
                weights.push_back(generator(engine));
                m1_w_tmp.push_back(0.0);
                m2_w_tmp.push_back(0.0);
            }
            weight.push_back(weights);
            m1_w.push_back(m1_w_tmp);
            m2_w.push_back(m2_w_tmp);
        }
    }

    // 順伝播（1層分）
    vector<vector<long double>> forward(const vector<vector<long double>>& input) override {

        last_input = input; // 逆伝播のために、最新のデータを記録

        // 線形変換
        vector<vector<long double>> output;
        // バッチごとに処理
        for (auto& batch : input) {
            vector<long double> batch_data;
            // 出力データごとに処理
            for (int out_index = 0; out_index < out_num; out_index++) {
                long double out_data = 0;
                out_data += bias[out_index];
                // 入力データごとに重みをかけ、出力データに加算
                for (int in_index = 0; in_index < in_num; in_index++) {
                    out_data += batch[in_index] * weight[out_index][in_index];
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

    // 逆伝播（1層分）
    vector<vector<long double>> backward(const vector<vector<long double>>& d_out) override {

        vector<vector<long double>> d_output = d_out;

        // 活性関数に通す
        if (activationID != ActivationID::None) {
            d_output = activation.backward(d_output);
        }

        // 前の層に送るdxを求めるd
        vector<vector<long double>> ans;
        for (auto& batch : d_output) { // バッチごとに処理
            vector<long double> res;
            for (int in_index = 0; in_index < in_num; in_index++) {
                long double t = 0;
                for (int out_index = 0; out_index < out_num; out_index++) {
                    t += weight[out_index][in_index] * batch[out_index];
                }
                res.push_back(t);
            }
            ans.push_back(res);
        }

        // 勾配(重み)を求める
        grad_weight.clear(); // 重み初期化
        grad_weight.resize(out_num, vector<long double>(in_num, 0));
        for (int out_index = 0; out_index < out_num; out_index++) {
            for (int in_index = 0; in_index < in_num; in_index++) {
                long double t = 0;
                for (int batch_index = 0; batch_index < d_output.size(); batch_index++) {
                    t += last_input[batch_index][in_index] * d_output[batch_index][out_index];
                }
                grad_weight[out_index][in_index] = t;
            }
        }

        // 勾配(バイアス)を求める
        grad_bias.clear(); // バイアス初期化
        grad_bias.resize(out_num, 0);
        for (int out_index = 0; out_index < out_num; out_index++) {
            long double t = 0;
            for (int batch_index = 0; batch_index < d_output.size(); batch_index++) {
                t += d_output[batch_index][out_index];
            }
            grad_bias[out_index] = t;
        }

        // パラメータ更新
        
        if (!model) {
            // 重み
            for (int i = 0; i < weight.size(); i++) {
                for (int j = 0; j < weight[0].size(); j++) {
                    m1_w[i][j] = beta1 * m1_w[i][j] + (1 - beta1) * grad_weight[i][j];
                    m2_w[i][j] = beta2 * m2_w[i][j] + (1 - beta2) * grad_weight[i][j] * grad_weight[i][j];
                    long double m1 = m1_w[i][j] / (1 - pow(beta1, t));
                    long double m2 = m2_w[i][j] / (1 - pow(beta2, t));
                    weight[i][j] -= lr * (m1 / (sqrt(m2) + e));
                    //weight[i][j] -= lr * grad_weight[i][j];
                }
            }
            // バイアス
            for (int i = 0; i < bias.size(); i++) {
                m1_b[i] = beta1 * m1_b[i] + (1 - beta1) * grad_bias[i];
                m2_b[i] = beta2 * m2_b[i] + (1 - beta2) * grad_bias[i] * grad_bias[i];
                long double m1 = m1_b[i] / (1 - pow(beta1, t));
                long double m2 = m2_b[i] / (1 - pow(beta2, t));
                bias[i] -= lr * (m1 / (sqrt(m2) + e));
                //bias[i] -= lr * grad_bias[i];
            }
        }

        t++;

        return ans;

    }

    void save_para(int layer_num) {

        string name = "para" + to_string(layer_num) + ".csv";
        ofstream file(name, ios::app);

        for (int i = 0; i < weight.size(); i++) {
            for (int j = 0; j < weight[0].size(); j++) {
                file << weight[i][j] << "\n";
            }
        }
        for (int i = 0; i < bias.size(); i++) {
            file << bias[i] << "\n";
        }

    }

    void load_para(int layer_num) {
        string name = "para" + to_string(layer_num) + ".csv";
        ifstream file(name);

        for (int i = 0; i < weight.size(); i++) {
            for (int j = 0; j < weight[0].size(); j++) {
                file >> weight[i][j];
            }
        }
        for (int i = 0; i < bias.size(); i++) {
            file >> bias[i];
        }
    }

};