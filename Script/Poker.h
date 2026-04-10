#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "System.h"

using namespace std;

struct PokerCard {
    int suit; // 0:♠, 1:♥, 2:◆, 3:♣
    int rank;
};

enum class HandRank {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};

struct PokerPlayer {
    string name;
    vector<PokerCard> hand;
    int money;
    bool isFolded;
};

class Poker {
private:
    vector<PokerCard> deck;
    vector<PokerPlayer> players;
    int& playerRef;
    int pot;
    int currentBet;

    void initDeck();
    void shuffleDeck();
    void showTable(bool revealAll);
    bool bettingRound();
    void CheckAndReplaceAI(); // 광산 연출 및 AI 교체

    // 보조 함수들
    string GetRankStr(int rank);
    string GetDetailedRankName(vector<PokerCard>& hand);

public:
    Poker(int& playerMoney);
    void play();
    string rankToString(HandRank rank);
    int getBalance() const { return playerRef; }
};