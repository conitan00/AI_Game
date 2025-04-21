#pragma once

class Flat : public Layer<vector<vector<long double>>, vector<vector<vector<vector<long double>>>>> {

public:

    int channel;
    int height;
    int width;

    Flat(int channel, int height, int width) {
        this->channel = channel;
        this->height = height;
        this->width = width;
    }

    vector<vector<long double>> forward(const vector<vector<vector<vector<long double>>>>& input) override {

        vector<vector<long double>> output;

        for (int b = 0; b < input.size(); b++) {
            vector<long double> res;
            for (int c = 0; c < input[0].size(); c++) {
                for (int h = 0; h < input[0][0].size(); h++) {
                    for (int w = 0; w < input[0][0][0].size(); w++) {
                        res.push_back(input[b][c][h][w]);
                    }
                }

            }
            output.push_back(res);
        }
        return output;
    }

    vector<vector<vector<vector<long double>>>> backward(const vector<vector<long double>>& d_output) override {

        vector<vector<vector<vector<long double>>>> d_input(
            d_output.size(), vector<vector<vector<long double>>>(
                channel, vector<vector<long double>>(
                    height, vector<long double>(width, 0)
                    )
                )
        );

        for (int b = 0; b < d_output.size(); b++) {
            int index = 0;
            for (int c = 0; c < channel; c++) {
                for (int h = 0; h < height; h++) {
                    for (int w = 0; w < width; w++) {
                        d_input[b][c][h][w] = d_output[b][index];
                        index++;
                    }
                }

            }
        }

        return d_input;
    }

};