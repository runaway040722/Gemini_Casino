#ifndef INDIAN_POKER_H
#define INDIAN_POKER_H

#include <vector>
#include <string>

struct IndianCard {
    int suit;
    int rank;
};

struct IndianPlayer {
    std::string name;
    IndianCard card;
    int money;      // ★ 참조자(&) 제거! 일반 int로 변경
    bool isFolded;
    int totalBet;
};

class IndianPoker {
private:
    std::vector<IndianPlayer> players;
    std::vector<IndianCard> deck;
    int& playerRef;
    int pot;
    int currentDeckIdx;

public:
    IndianPoker(int& money);
    void InitDeck();
    void Play();
    void ShowTable(bool revealPlayerCard);
    int EvaluateWinner();
    std::string GetCardStr(IndianCard c);

    // AI 교체 및 파산 체크 함수
    void CheckAndReplaceAI();
};

#endif