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
    int currentDeckIdx; // <-- 이 줄이 누락되어 에러가 난 것입니다.

public:
    IndianPoker(int& money);
    void InitDeck();
    void Play();
    void ShowTable(bool revealPlayerCard);
    int EvaluateWinner();
    std::string GetCardStr(IndianCard c);
};

#endif