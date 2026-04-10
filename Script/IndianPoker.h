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
    int money;
    bool isFolded;
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

    // [추가] .cpp에서 구현한 AI 교체 함수를 여기에 선언해야 합니다.
    void CheckAndReplaceAI();
};

#endif