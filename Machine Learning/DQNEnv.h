#pragma once

class Env {
public:
    static const int height = 5;
    static const int width = 5;
    //static const int state_size = 2;
    static const int action_size = 4;

    // 報酬マップ（初期化後に中身を設定）
    int rewards[height][width];

    // 開始位置
    int PosX = 2;
    int PosY = 2;
    // 状態(エージェントの位置、報酬の位置、ゴールの位置)
    Vector3D state;

    // 移動方向
    long double move[action_size][2] = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };

    // コンストラクタで報酬マップ,状態を初期化
    Env() {

        state = Vector3D(3, Vector2D(height, Vector1D(width, 0.0)));
        state[0][PosX][PosY] = 1.0;
        //state[2][height - 1][width - 1] = 10.0;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                rewards[y][x] = 1; // デフォルトは1
                state[1][y][x] = 1;
            }
        }

        //// 一部を罠に
        //for (int y = 3; y < 4; ++y) {
        //    for (int x = 3; x < 4; ++x) {
        //        rewards[y][x] = -1;
        //        state[1][y][x] = -1;
        //    }
        //}

        // ゴールを右下に
        /*rewards[height - 1][width - 1] = 10;
        state[1][height - 1][width - 1] = 10;*/


    }

    void reset() {
        PosX = 2;
        PosY = 2;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                state[0][x][y] = 0.0;
            }
        }
        state[0][PosX][PosY] = 1.0;
    }

    Memory step(int action_index) {
        int tempX = PosX + move[action_index][0];
        int tempY = PosY + move[action_index][1];

        double reward;
        bool done = false;
        Vector3D next_state = state;

        // 境界チェック
        if (0 <= tempX && tempX < width &&
            0 <= tempY && tempY < height) {

            reward = rewards[tempX][tempY];
            if (reward == 10) done = true;
        }
        else {
            reward = -5;
            done = true;
        }

        if (!done) {
            next_state[0][PosX][PosY] = 0.0;
            next_state[0][tempX][tempY] = 1.0;
        }

        PosX = tempX;
        PosY = tempY;

        Vector3D now_state = state;
        state = next_state;


        return { {now_state[0] }, action_index, reward, {next_state[0]}, done
        };

    }
};