#ifndef POPUPGAME_H
#define POPUPGAME_H

#include <vector>
#include <string>

class PopUpGame {
private:
    int& playerRef;       // 플레이어 잔고 참조
    int pot;              // 현재 판돈

    // 내부 로직용 함수
    void drawBarrel(const std::vector<bool>& holes, bool exploded);

public:
    PopUpGame(int& playerMoney);
    void play();          // 게임 실행 메인 루프
};

#endif