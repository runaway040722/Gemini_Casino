#include "Card.h"
#include "System.h" // SetColor 사용을 위해 포함
#include <algorithm>
#include <random>
#include <ctime>

std::string Card::toString() const {
    std::string s_suit;
    if (suit == 0) s_suit = "♠";
    else if (suit == 1) s_suit = "♥";
    else if (suit == 2) s_suit = "◆";
    else s_suit = "♣";

    std::string s_rank;
    if (rank == 1) s_rank = "A";
    else if (rank == 11) s_rank = "J";
    else if (rank == 12) s_rank = "Q";
    else if (rank == 13) s_rank = "K";
    else s_rank = std::to_string(rank);

    return s_suit + s_rank;
}

int Card::getColor() const {
    // 하트(1)와 다이아(2)는 빨간색(12), 나머지는 흰색(15)
    return (suit == 1 || suit == 2) ? 12 : 15;
}

void InitDeck(std::vector<Card>& deck) {
    deck.clear();
    for (int s = 0; s < 4; s++) {
        for (int r = 1; r <= 13; r++) {
            deck.push_back({ s, r });
        }
    }
}

void ShuffleDeck(std::vector<Card>& deck) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}