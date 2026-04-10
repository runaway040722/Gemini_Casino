#pragma once
#include <vector>
#include <string>

// 타 게임과 충돌 방지를 위한 고유 구조체명
struct IndianCard {
    int suit; // 0:♠, 1:◆, 2:♥, 3:♣
    int rank; // 2 ~ 10
};

struct IndianPlayer {
    std::string name;
    IndianCard card;
    int money;
    bool isFolded;
};

class IndianPoker {
private:
    std::vector<IndianCard> deck;
    std::vector<IndianPlayer> players;
    int& playerRef; // 메인 자산 연동
    int pot;

public:
    IndianPoker(int& money);
    void Play();

private:
    void InitDeck();
    void ShowTable(bool revealPlayerCard);
    int EvaluateWinner();
    std::string GetCardStr(IndianCard c);
};