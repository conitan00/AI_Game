#pragma once

// 爆弾
struct BOM {
    bool exist = false;
    float time = 9.0;
    int X, Y, X_cell, Y_cell;
};

// 学習を行うエージェント
class DQNAgent {


public:

    int X, Y, X_cell, Y_cell;
    int Dir = 0;
    int numBom = 1;
    int Mov = 2;
    int fireLength = 1;
    int picID;
    int death_cnt = 8;
    bool death = false;
    int totalMove = 0;
    string AorE;
    BOM boms[10];
    Vector3D state_pre;
    vector<bool> mask_pre;
    int action_pre = 0;
    int win_num = 0;
    int win_num_tmp = 0;
    bool kill = false;

    ///

    string action_name[4] = { "right", "left", "down", "up" };
    string action_ya[4] = { "→", "←", "↓", "↑" };
    //std::atomic<bool> check{ false };

    float gamma = 0.99; // 割引率
    //float lr = 0.005; // 学習率
    float epsilon = 1.0; // ε-greedy法のε
    float e_deg_rate = 0.01;
    float e_min = 0.05;
    int buffer_size = 5000; // バッファサイズ
    int batch_size = 16; // バッチサイズ
    static const int action_size = 8; // 行動の種類
    Vector2D diff_loss; // 誤差

    ReplayBuffer replay_buffer = { buffer_size, batch_size };
    DQN qnet;
    DQN qnet_target;

    // エージェント行動
    int get_action(Vector3D& state, vector<bool> mask) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dou(0, 1.0);
        uniform_int_distribution<int> in(0, action_size - 1);
        double ran = dou(gen);

        // 確率 ε でランダムに行動を決定
        if (ran <= epsilon) return in(gen);
        // 確率 1-ε でQ関数によりをランダムに決定
        else {
            Vector4D state_net = { state };
            vector<vector<long double>> qs = qnet.forward(state_net, { mask });
            int q_max_index = -1;
            long double q_max = -1000000;
            for (int i = 0; i < action_size; i++) {
                if (qs[0][i] > q_max) {
                    q_max = qs[0][i];
                    q_max_index = i;
                }
            }
            return q_max_index;
        }
    }

    // Qネットワーク更新
    void update(Memory& memory) {

        // バッファに経験を追加
        replay_buffer.add(memory);
        // バッファのデータ数がバッチサイズ未満の時、更新を行わない
        if (replay_buffer.buffer.size() < batch_size) return;
        // バッファからバッチサイズの経験を抽出
        vector<Memory> batch_memory = replay_buffer.get_batch();
        Vector4D state_m;
        vector<int> action_m;
        vector<double> reward_m;
        Vector4D next_state_m;
        vector<bool> done_m;
        vector<vector<bool>> mask_m;
        vector<vector<bool>> mask_next_m;
        for (int batch_index = 0; batch_index < batch_size; batch_index++) {
            state_m.push_back(batch_memory[batch_index].state);
            action_m.push_back(batch_memory[batch_index].action);
            reward_m.push_back(batch_memory[batch_index].reward);
            next_state_m.push_back(batch_memory[batch_index].next_state);
            done_m.push_back(batch_memory[batch_index].done);
            mask_m.push_back(batch_memory[batch_index].mask);
            mask_next_m.push_back(batch_memory[batch_index].mask_next);
        }
        // 各経験の状態における行動のq値
        vector<vector<long double>> qs = qnet.forward(state_m, mask_m);
        // 実際に取った行動のq値
        vector<long double> q;
        for (int batch_index = 0; batch_index < batch_size; batch_index++) {
            int action_index = action_m[batch_index];
            q.push_back(qs[batch_index][action_index]);
        }
        // 各経験の次の状態における各行動のq値
        vector<vector<long double>> next_qs = qnet_target.forward(next_state_m, mask_next_m);
        // q値が最大の行動のq値
        vector<long double> next_q;
        for (int batch_index = 0; batch_index < batch_size; batch_index++) {
            long double q_max = next_qs[batch_index][0];
            for (int i = 0; i < action_size; i++) {
                if (next_qs[batch_index][i] > q_max) {
                    q_max = next_qs[batch_index][i];
                }
            }
            next_q.push_back(q_max);
        }
        // TDターゲットを計算
        vector<long double> target;
        for (int batch_index = 0; batch_index < batch_size; batch_index++) {
            long double t = reward_m[batch_index];
            if (done_m[batch_index] == false) {
                t += gamma * next_q[batch_index];
            }
            target.push_back(t);
        }
        // 損失関数を計算
        long double loss = Loss::mean_squared_error(q, target);
        diff_loss.clear();
        for (int batch_index = 0; batch_index < batch_size; batch_index++) {
            vector<long double> res;
            for (int action_index = 0; action_index < action_size; action_index++) {
                if (action_m[batch_index] == action_index) {
                    res.push_back((q[batch_index] - target[batch_index]) / batch_size);
                }
                else {
                    res.push_back(0.0);
                }
            }
            diff_loss.push_back(res);
        }
        // 逆伝播 & パラメータ更新
        qnet.backward(diff_loss);

        //// 各経験の状態における行動のq値
        //qs = qnet.forward(state_m, mask_m);
        //// 実際に取った行動のq値
        //vector<long double> q_new;
        //for (int batch_index = 0; batch_index < batch_size; batch_index++) {
        //    int action_index = action_m[batch_index];
        //    q_new.push_back(qs[batch_index][action_index]);
        //}

        //long double loss_new = Loss::mean_squared_error(q_new, target);
        //if (loss < loss_new) {
        //    //cout << loss << " -> " << loss_new << endl;
        //    //cout << "miss!!";
        //}
        //cout << endl;
        /*else {
            cout << "ok\n";
            cout << loss << " -> " << loss_new << endl; 
        }*/

        //if (check) {
        //    Vector2D state(5, Vector1D(5, 0.0));
        //    for (int h = 4; h >= 0; h--) {
        //        for (int w = 0; w < 5; w++) {
        //            //cout << "(" << w << "," << h << ")" << endl;

        //            Vector2D state_tmp = state;
        //            state_tmp[w][h] = 1.0;
        //            Vector2D result = qnet.forward({ {state_tmp} });
        //            int index = 0;
        //            long double max = -100000;
        //            for (int i = 0; i < 4; i++) {
        //                if (result[0][i] > max) {
        //                    index = i;
        //                    max = result[0][i];
        //                }
        //            }
        //            /*for (int i = 0; i < 4; i++) {
        //                cout << action_name[i] << ": " << result[0][i] << endl;
        //            }*/
        //            cout << action_ya[index] << " ";
        //        }
        //        cout << endl;
        //    }
        //    check = false;
        //}


    }

    // ターゲットネットワークを同期
    void sync_qnet() {
        qnet_target = qnet;
    }

};