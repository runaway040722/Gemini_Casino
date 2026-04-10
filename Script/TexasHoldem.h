#ifndef TEXAS_HOLDEM_H
#define TEXAS_HOLDEM_H

#include <vector>
#include <string>
#include "Poker.h" // PokerCard 구조체 공유

class TexasHoldem {
private:
    int& playerMoney;
    std::vector<PokerCard> deck;
    std::vector<PokerCard> communityCards;
    std::vector<PokerCard> playerHand;
    std::vector<PokerCard> cpuHand;

    // 내부 유틸리티 함수
    void initDeck();
    void shuffleDeck();
    PokerCard drawCard();
    std::string getCardName(PokerCard c);
    long long evaluateHand(std::vector<PokerCard>& hand, std::vector<PokerCard>& community);

public:
    TexasHoldem(int& money) : playerMoney(money) {}
    void Play();
};

#endif