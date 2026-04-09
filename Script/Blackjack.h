#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "System.h"
#include <vector>
#include <string>

// 이름 충돌 방지를 위한 enum class
enum class CardRank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };
enum class CardSuit { SPADES, HEARTS, DIAMONDS, CLUBS };

struct Card {
    CardRank rank;
    CardSuit suit;
    int getValue() const;
};

class Deck {
private:
    std::vector<Card> cards;
public:
    Deck();
    void shuffleDeck();
    Card drawCard();
};

class Hand {
public:
    std::vector<Card> cards;
    void addCard(Card card);
    int getTotal() const;
    void showHand(std::string owner) const;
};

// 메인에서 호출할 함수
void PlayBlackjack(int& money);

#endif