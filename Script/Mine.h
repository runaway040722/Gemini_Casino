#pragma once

class Mine {
private:
    int& playerRef;

public:
    Mine(int& playerMoney);
    void StartMining();
};

// ★ 이거 추가
void PlayMine(int& money);