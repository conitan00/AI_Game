#pragma once

// 損失関数
class Loss {

public:

    Vector2D last;

    // 二乗和誤差
    static long double mean_squared_error(vector<long double>& data1, vector<long double>& data2) {
        long double sum = 0;
        for (int i = 0; i < data1.size(); i++) {
            sum += (data1[i] - data2[i]) * (data1[i] - data2[i]);
        }
        long double ans = 0.5 * sum;
        return ans;
    }

    // 二乗和誤差(逆伝播)
    static vector<vector<long double>> mean_squared_error_back(vector<vector<long double>>& diff_loss) {
        return diff_loss;
    }

    // クロスエントロピー誤差(順伝播)
    long double Cross_entropy_error(const Vector2D& o, const Vector2D& t) {
        last = o;
        long double delta = 1e-7;
        long double sum = 0;
        for (int b = 0; b < t.size(); b++) {
            for (int i = 0; i < t[0].size(); i++) {
                sum -= t[b][i] * log(o[b][i] + delta);
            }
        }
        return sum / t.size();
    }

    // クロスエントロピー誤差(逆伝播)
    Vector2D Cross_entropy_error_back(const Vector2D& t) {
        Vector2D ans;
        for (int b = 0; b < t.size(); b++) {
            Vector1D res;
            for (int i = 0; i < t[0].size(); i++) {
                res.push_back(last[b][i] - t[b][i]);
            }
            ans.push_back(res);
        }
        return ans;
    }

};