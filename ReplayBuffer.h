#pragma once

// 1回分の経験
struct Memory {
    Vector3D state;
    int action;
    double reward;
    Vector3D next_state;
    bool done;
    vector<bool> mask;
    vector<bool> mask_next;
};

// 経験再生バッファ
class ReplayBuffer {

public:
    int buffer_size; // バッファサイズ
    int batch_size; // バッチサイズ
    deque<Memory> buffer; // バッファ
    // コンストラクタ
    ReplayBuffer(int buffer_size, int batch_size) {
        this->buffer_size = buffer_size;
        this->batch_size = batch_size;
    }
    // 経験をバッファに追加
    void add(const Memory& memory) {
        buffer.push_back(memory);
        if (buffer.size() > buffer_size) buffer.pop_front();
    }

    // バッチサイズの経験を取得
    vector<Memory> get_batch() {
        vector<Memory> batch;
        // ランダムにサンプルを取得
        random_device rd;
        mt19937 gen(rd());
        sample(buffer.begin(), buffer.end(), back_inserter(batch), batch_size, gen);
        return batch;
    }

};