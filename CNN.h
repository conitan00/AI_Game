#pragma once

// CNNモデル
class CNN {

private:
    static const int batch_size = 32;
    static const int H = 28; // 画像の縦
    static const int W = 28; // 画像の横
    static const int input_channel = 1; // 入力チャネル数
    static const int category_size = 10; // 最終層の出力数
    Loss _loss;
public:

    // ネットワーク構造

    vector<Convolution2D*> conv;
    vector<Flat*> flat;
    vector<FC*> fc;

    // コンストラクタ
    CNN() {
        conv.push_back(new Convolution2D(input_channel, 32, H, W, ActivationID::relu));
        conv.push_back(new Convolution2D(32, 64, H, W, ActivationID::relu));
        flat.push_back(new Flat(64, H, W));
        fc.push_back(new FC(64 * H * W, 32, ActivationID::relu));
        fc.push_back(new FC(32, category_size, ActivationID::softmax));
    }

    // デストラクタ
    ~CNN() {
        for (auto layer : conv) delete layer;
        for (auto layer : flat) delete layer;
        for (auto layer : fc) delete layer;
    }

    // 順伝播(画像を入力し、損失誤差を求める)
    long double forward(const Vector4D& data, const Vector2D& t) {
        vector<vector<vector<vector<long double>>>> res = data;
        vector<vector<long double>> output;
        for (auto& layer : conv) {
            res = layer->forward(res);
        }
        for (auto& layer : flat) {
            output = layer->forward(res);
        }
        for (auto& layer : fc) {
            output = layer->forward(output);
        }
        long double loss = _loss.Cross_entropy_error(output, t);
        return loss;
    }

    Vector1D fo(const Vector4D& data, const Vector2D& t) {
        vector<vector<vector<vector<long double>>>> res = data;
        vector<vector<long double>> output;
        for (auto& layer : conv) {
            res = layer->forward(res);
        }
        for (auto& layer : flat) {
            output = layer->forward(res);
        }
        for (auto& layer : fc) {
            output = layer->forward(output);
        }
        long double loss = _loss.Cross_entropy_error(output, t);
        return output[0];
    }

    // 逆伝播
    void backward(const Vector2D& t) {
        Vector2D d_loss = _loss.Cross_entropy_error_back(t);
        vector<vector<vector<vector<long double>>>> d_input;
        for (auto it = fc.rbegin(); it != fc.rend(); ++it) {
            d_loss = (*it)->backward(d_loss);
        }
        for (auto it = flat.rbegin(); it != flat.rend(); ++it) {
            d_input = (*it)->backward(d_loss);
        }
        for (auto it = conv.rbegin(); it != conv.rend(); ++it) {
            d_input = (*it)->backward(d_input);
        }
    }

    // 予測、損失計算
    long double predict(const Vector4D& data, const Vector2D& t) {
        long double loss = forward(data, t);
        return loss;
    }

    // 逆伝播、パラメータ更新
    void update(const Vector2D& t) {
        backward(t);
    }


};