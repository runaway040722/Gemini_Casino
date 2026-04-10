#ifndef TEXAS_HOLDEM_H
#define TEXAS_HOLDEM_H

#include <vector>
#include <string>
#include "Poker.h"

struct THPlayer {
    std::string name;
    std::vector<PokerCard> hand;
    int money;
    bool isFolded;
    bool isAllIn;
    int totalBetThisRound; // <-- 이 줄이 추가되어야 합니다!
};

class TexasHoldem {
private:
    int& playerMoney;
    std::vector<PokerCard> deck;
    std::vector<PokerCard> communityCards;

    void initDeck();
    void shuffleDeck();
    std::string GetRankStr(int rank);
    long long evaluateHand(std::vector<PokerCard>& hand, std::vector<PokerCard>& community);

public:
    TexasHoldem(int& money) : playerMoney(money) {}
    void Play();
};

#endif