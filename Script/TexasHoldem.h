#ifndef TEXAS_HOLDEM_H
#define TEXAS_HOLDEM_H

#include <vector>
#include <string>
#include "Poker.h" // PokerCard 구조체 공유

// 1. THPlayer 구조체를 클래스 외부(위쪽)에 선언해야 cpp에서 인식합니다.
struct THPlayer {
    std::string name;
    std::vector<PokerCard> hand;
    int money;
    bool isFolded;
};

class TexasHoldem {
private:
    int& playerMoney;
    std::vector<PokerCard> deck;
    std::vector<PokerCard> communityCards;
    // 기존 개별 핸드 변수 대신 cpp에서 사용하는 방식에 맞게 정리해도 되지만, 
    // 일단 에러 방지를 위해 그대로 두거나 삭제하셔도 무방합니다.

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