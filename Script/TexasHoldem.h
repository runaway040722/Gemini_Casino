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
    bool isAllIn; // 올인 상태 추가
};

class TexasHoldem {
private:
    int& playerMoney;
    std::vector<PokerCard> deck;
    std::vector<PokerCard> communityCards;

    void initDeck();
    void shuffleDeck();
    std::string GetRankStr(int rank);
    // 7장 중 최선의 5장을 계산하는 로직 (기존 Poker.cpp 로직 활용 권장)
    long long evaluateHand(std::vector<PokerCard>& hand, std::vector<PokerCard>& community);

public:
    TexasHoldem(int& money) : playerMoney(money) {}
    void Play();
};

#endif