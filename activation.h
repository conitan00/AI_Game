#pragma once

// 活性化関数
enum ActivationID {
    relu,
    sigmoid,
    softmax,
    None
};

class Activation {

public:

    ActivationID activationID;
    vector<vector<long double>> last_data;

    Activation() {}
    // コンストラクタ
    Activation(ActivationID activationID) {
        this->activationID = activationID;
    }

    // ReLu関数(順伝播)
    vector<vector<long double>> ReLu(vector<vector<long double>>& x) {

        vector<vector<long double>> y;
        for (auto& batch : x) {
            vector<long double> res;
            for (auto data : batch) {
                long double ans;
                ans = (data >= 0) ? data : 0;
                res.push_back(ans);
            }
            y.push_back(res);
        }
        return y;

    }

    // ReLu関数(逆伝播)
    vector<vector<long double>> ReLu_back(vector<vector<long double>>& x) {

        vector<vector<long double>> res = x;
        for (int i = 0; i < res.size(); i++) {
            for (int j = 0; j < res[0].size(); j++) {
                res[i][j] = (last_data[i][j] > 0) ? res[i][j] : 0;
            }
        }
        return res;

    }

    // sigmoid関数(順伝播) //
    std::vector<std::vector<long double>> Sigmoid(std::vector<std::vector<long double>>& x)
    {
        std::vector<std::vector<long double>> res;

        for (auto batch : x)
        {
            // バッチごとに処理
            std::vector<long double> t;
            for (auto i : batch)
            {
                t.push_back(1.0 / (1.0 + exp(-i)));
            }
            res.push_back(t);
        }

        return res;
    }

    // sigmoid関数(逆伝播)
    std::vector<std::vector<long double>> Sigmoid_back(std::vector<std::vector<long double>>& x)
    {
        std::vector<std::vector<long double>> res;

        for (int i = 0; i < x.size(); ++i)
        {
            std::vector<long double> t;
            for (int j = 0; j < x[i].size(); ++j)
            {
                t.push_back(x[i][j] * (1 - last_data[i][j]) * last_data[i][j]);
            }
            res.push_back(t);
        }

        return res;
    }

    // softmax関数(順伝播)
    Vector2D Softmax(const Vector2D& x)
    {
        Vector2D res;

        for (const auto& batch : x)
        {
            // バッチごとに処理
            long double sum = 0;
            long double max_val = *max_element(batch.begin(), batch.end());
            for (const auto i : batch)
            {
                sum += exp(i - max_val);
            }
            Vector1D t;
            for (const auto& i : batch)
            {
                t.push_back(exp(i - max_val) / sum);
            }
            res.push_back(t);
        }

        return res;
    }

    // softmax関数(逆伝播)
    Vector2D Softmax_back(const Vector2D& x)
    {
        return x;
    }

    // 順伝播
    vector<vector<long double>> forward(vector<vector<long double>>& data) {
        last_data = data;
        if (this->activationID == ActivationID::relu) {
            return ReLu(data);
        }
        else if (this->activationID == ActivationID::sigmoid) {
            return Sigmoid(data);
        }
        else if (this->activationID == ActivationID::softmax) {
            return Softmax(data);
        }
        else return data;
    }

    // 逆伝播
    vector<vector<long double>> backward(vector<vector<long double>>& data) {
        if (this->activationID == ActivationID::relu) {
            return ReLu_back(data);
        }
        else if (this->activationID == ActivationID::sigmoid) {
            return Sigmoid_back(data);
        }
        else if (this->activationID == ActivationID::softmax) {
            return Softmax_back(data);
        }
        else return data;
    }

};

class Activation_4d {

public:

    ActivationID activationID;
    vector<vector<vector<vector<long double>>>> last_data;

    Activation_4d() {}
    // コンストラクタ
    Activation_4d(ActivationID activationID) {
        this->activationID = activationID;
    }


    // ReLu関数(順伝播)(4d)
    vector<vector<vector<vector<long double>>>> ReLu(vector<vector<vector<vector<long double>>>>& x) {

        vector<vector<vector<vector<long double>>>> y = x;
        for (auto& batch : y) { // バッチ
            for (auto& channel : batch) { // チャネル
                for (auto& row : channel) { // 高さ（H）
                    for (auto& data : row) { // 幅（W）
                        data = (data >= 0) ? data : 0; // ReLU適用
                    }
                }
            }
        }
        return y;

    }

    // ReLu関数(逆伝播)(4d)
    vector<vector<vector<vector<long double>>>> ReLu_back(vector<vector<vector<vector<long double>>>>& x) {

        vector<vector<vector<vector<long double>>>> res = x;
        for (int a = 0; a < res.size(); a++) {
            for (int b = 0; b < res[0].size(); b++) {
                for (int c = 0; c < res[0][0].size(); c++) {
                    for (int d = 0; d < res[0][0][0].size(); d++) {
                        res[a][b][c][d] = (last_data[a][b][c][d] > 0) ? res[a][b][c][d] : 0;
                    }
                }
            }
        }
        return res;

    }

    // sigmoid関数(順伝播) //
    //std::vector<std::vector<long double>> Sigmoid(std::vector<std::vector<long double>>& x)
    //{
    //    std::vector<std::vector<long double>> res;

    //    for (auto batch : x)
    //    {
    //        // バッチごとに処理
    //        std::vector<long double> t;
    //        for (auto i : batch)
    //        {
    //            t.push_back(1.0 / (1.0 + exp(-i)));
    //        }
    //        res.push_back(t);
    //    }

    //    return res;
    //}

    // sigmoid関数(逆伝播)
    /*std::vector<std::vector<long double>> Sigmoid_back(std::vector<std::vector<long double>>& x)
    {
        std::vector<std::vector<long double>> res;

        for (int i = 0; i < x.size(); ++i)
        {
            std::vector<long double> t;
            for (int j = 0; j < x[i].size(); ++j)
            {
                t.push_back(x[i][j] * (1 - last_data[i][j]) * last_data[i][j]);
            }
            res.push_back(t);
        }

        return res;
    }*/

    // 順伝播
    vector<vector<vector<vector<long double>>>> forward(vector<vector<vector<vector<long double>>>>& data) {
        last_data = data;
        if (this->activationID == ActivationID::relu) {
            return ReLu(data);
        }
        /*else if (this->activationID == ActivationID::sigmoid) {
            return Sigmoid(data);
        }*/
        else return data;
    }

    // 逆伝播
    vector<vector<vector<vector<long double>>>> backward(vector<vector<vector<vector<long double>>>>& data) {
        if (this->activationID == ActivationID::relu) {
            return ReLu_back(data);
        }
        /*else if (this->activationID == ActivationID::sigmoid) {
            return Sigmoid_back(data);
        }*/
        else return data;
    }

};