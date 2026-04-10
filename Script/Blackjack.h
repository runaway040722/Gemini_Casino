#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <vector>
#include <string>

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
    // 두 번째 인자(bool)를 반드시 추가해야 소스 파일과 호환됩니다.
    void showHand(std::string owner, bool hideFirstCard = false) const;
};

void PlayBlackjack(int& money);

#endif