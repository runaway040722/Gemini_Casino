#include "Poker.h"
#include "System.h"
#include <windows.h>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

using namespace std;

extern vector<string> globalNamePool;

int GetCardPower(int rank) {
    return (rank == 1) ? 14 : rank;
}

int GetCardScore(int rank, int suit) {
    int score = GetCardPower(rank) * 10;
    if (suit == 0) score += 3;      // ♠
    else if (suit == 2) score += 2; // ◆
    else if (suit == 1) score += 1; // ♥
    else if (suit == 3) score += 0; // ♣
    return score;
}

Poker::Poker(int& playerMoney) : playerRef(playerMoney), pot(0), currentBet(0) {
    players.clear();
    players.push_back({ "나 (Player)", {}, playerRef, false });
    random_device rd;
    mt19937 g(rd());
    vector<string> tempPool = globalNamePool;
    shuffle(tempPool.begin(), tempPool.end(), g);
    for (int i = 0; i < 3; i++) players.push_back({ tempPool[i], {}, 100000, false });
    initDeck();
}

void Poker::initDeck() {
    deck.clear();
    for (int s = 0; s < 4; s++) {
        for (int r = 1; r <= 13; r++) deck.push_back({ s, r });
    }
}

void Poker::shuffleDeck() {
    random_device rd;
    mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

string Poker::GetRankStr(int rank) {
    if (rank == 1) return "A";
    if (rank == 11) return "J";
    if (rank == 12) return "Q";
    if (rank == 13) return "K";
    return to_string(rank);
}

long long GetHandStrength(vector<PokerCard>& hand) {
    if (hand.empty()) return 0;
    int counts[15] = { 0 };
    for (auto& c : hand) counts[c.rank]++;
    int fours = 0, triples = 0, high = 0;
    vector<int> pairs;
    for (int r : {1, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2}) {
        int p = GetCardPower(r);
        if (counts[r] == 4) fours = p;
        else if (counts[r] == 3) triples = p;
        else if (counts[r] == 2) pairs.push_back(p);
        if (counts[r] > 0 && p > high) high = p;
    }
    if (fours > 0) return 8000000LL + fours;
    if (triples > 0 && !pairs.empty()) return 7000000LL + triples;
    if (triples > 0) return 4000000LL + triples;
    if (pairs.size() >= 2) return 3000000LL + (pairs[0] * 100) + pairs[1];
    if (!pairs.empty()) return 2000000LL + pairs[0];
    return 1000000LL + high;
}

string Poker::GetDetailedRankName(vector<PokerCard>& hand) {
    if (hand.empty()) return "";
    int counts[15] = { 0 };
    for (auto& c : hand) counts[c.rank]++;
    int fours = 0, triples = 0, high = 0;
    vector<int> pairs;
    for (int r : {1, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2}) {
        if (counts[r] == 4) fours = r;
        else if (counts[r] == 3) triples = r;
        else if (counts[r] == 2) pairs.push_back(r);
        if (counts[r] > 0 && (high == 0 || GetCardPower(r) > GetCardPower(high))) high = r;
    }
    if (fours > 0) return GetRankStr(fours) + " 포카드";
    if (triples > 0 && !pairs.empty()) return GetRankStr(triples) + " 풀하우스";
    if (triples > 0) return GetRankStr(triples) + " 트리플";
    if (pairs.size() >= 2) return GetRankStr(pairs[0]) + " " + GetRankStr(pairs[1]) + " 투 페어";
    if (pairs.size() == 1) return GetRankStr(pairs[0]) + " 원 페어";
    return GetRankStr(high) + " 하이";
}

void Poker::showTable(bool revealAll) {
    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 4인 포커 - 5카드 하이 ]                " << endl;
    cout << "          현재 총 판돈(POT): " << pot << " 달러" << endl;
    cout << "==========================================================" << endl;
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) {
            SetColor(8);
            cout << left << setw(20) << players[i].name << " : [ 기권(FOLDED) ]" << endl;
        }
        else {
            SetColor(i == 0 ? 11 : 15);
            cout << left << setw(20) << players[i].name << " : ";
            for (auto& card : players[i].hand) {
                if (i == 0 || revealAll) {
                    string s_mark;
                    if (card.suit == 0) { SetColor(15); s_mark = "♠"; }
                    else if (card.suit == 1) { SetColor(12); s_mark = "♥"; }
                    else if (card.suit == 2) { SetColor(12); s_mark = "◆"; }
                    else { SetColor(15); s_mark = "♣"; }
                    cout << s_mark << GetRankStr(card.rank) << " ";
                    SetColor(i == 0 ? 11 : 15);
                }
                else cout << "?? ";
            }
            cout << " (잔고: " << (i == 0 ? playerRef : players[i].money) << "$)";
            if ((i == 0 || revealAll) && !players[i].hand.empty()) {
                SetColor(10);
                cout << " (" << GetDetailedRankName(players[i].hand) << ")";
            }
            cout << endl;
        }
    }
    SetColor(15);
    cout << "==========================================================" << endl;
}

void Poker::play() {
    system("cls");
    SetColor(11); cout << "\n  [ 포커 테이블 입장 ]" << endl;
    SetColor(15); cout << "  베팅 단위: 1. 1,000 | 2. 5,000 | 3. 10,000 | 4. 20,000\n  선택: ";
    int menu, baseBet = 5000; cin >> menu;
    if (menu == 1) baseBet = 1000;
    else if (menu == 3) baseBet = 10000;
    else if (menu == 4) baseBet = 20000;

    random_device rd; mt19937 g(rd());
    uniform_int_distribution<int> dist(0, (int)globalNamePool.size() - 1);

    while (true) {
        if (playerRef < baseBet) {
            system("cls"); SetColor(12); cout << "\n [!] 자산 부족으로 퇴장당했습니다." << endl;
            Sleep(1500); return;
        }
        for (int i = 1; i < (int)players.size(); i++) {
            if (players[i].money < baseBet) {
                players[i].name = globalNamePool[dist(g)];
                players[i].money = 100000;
            }
        }

        int ante = baseBet / 2; currentBet = 0; initDeck(); shuffleDeck();
        int deckIdx = 0; pot = 0;

        for (int i = 0; i < (int)players.size(); i++) {
            int& m = (i == 0) ? playerRef : players[i].money;
            m -= ante; pot += ante;
            players[i].hand.clear(); players[i].isFolded = false;
        }

        for (int i = 0; i < 2; i++) {
            for (auto& p : players) p.hand.push_back(deck[deckIdx++]);
        }

        for (int round = 1; round <= 4; round++) {
            showTable(false);
            bool isAllInCalled = bettingRound();

            int active = 0;
            for (auto& p : players) if (!p.isFolded) active++;
            if (active <= 1) break;

            if (isAllInCalled) {
                SetColor(12); cout << "\n [ 올인 승부 발생! ]" << endl; Sleep(1000);
                while (players[0].hand.size() < 5) {
                    for (auto& p : players) if (!p.isFolded) p.hand.push_back(deck[deckIdx++]);
                }
                break;
            }
            if (round < 4) {
                cout << "\n [ " << round + 2 << "번째 카드를 배분합니다... ]" << endl;
                Sleep(800);
                for (auto& p : players) if (!p.isFolded) p.hand.push_back(deck[deckIdx++]);
            }
        }

        showTable(true);
        int winnerIdx = evaluateWinner();
        SetColor(10);
        cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
        cout << " 승자: " << players[winnerIdx].name << endl;
        cout << " 상금: " << pot << "$" << endl;
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;

        if (winnerIdx == 0) playerRef += pot;
        else players[winnerIdx].money += pot;

        cout << "\n [1] 계속 [0] 퇴장 : ";
        int cont; cin >> cont;
        if (cont == 0) break;
    }
}

bool Poker::bettingRound() {
    bool callAgainstAllIn = false;
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) continue;
        int& curM = (i == 0) ? playerRef : players[i].money;
        if (curM <= 0) continue;

        if (i == 0) {
            cout << "\n[1]콜/체크 [2]하프 [3]올인 [4]다이 : ";
            int choice; cin >> choice;
            if (choice == 2) {
                int bet = min(curM, pot / 2);
                curM -= bet; pot += bet; currentBet = bet;
                cout << " [ 당신의 하프 베팅! ]" << endl;
            }
            else if (choice == 3) {
                cout << " [ 당신의 올인!!! ]" << endl;
                pot += curM; currentBet = max(currentBet, curM); curM = 0;
            }
            else if (choice == 4) {
                players[i].isFolded = true; cout << " [ 당신은 다이했습니다. ]" << endl;
            }
            else {
                int pay = min(currentBet, curM);
                curM -= pay; pot += pay;
                cout << (currentBet == 0 ? " [ 체크 ]" : " [ 콜 ]") << endl;
            }
        }
        else {
            cout << players[i].name << " 생각 중..."; Sleep(600);
            long long strength = GetHandStrength(players[i].hand);
            int prob = rand() % 100;

            if (currentBet > curM * 0.4) {
                if (strength >= 2000000 || prob < 25) {
                    int pay = min(currentBet, curM);
                    curM -= pay; pot += pay;
                    cout << " [ 콜! ]" << endl;
                    for (int j = 0; j < 4; j++) {
                        int m = (j == 0 ? playerRef : players[j].money);
                        if (j != i && !players[j].isFolded && m == 0) callAgainstAllIn = true;
                    }
                    if (curM == 0) callAgainstAllIn = true;
                }
                else {
                    players[i].isFolded = true; cout << " [ 다이 ]" << endl;
                }
            }
            else {
                int pay = min(currentBet, curM);
                curM -= pay; pot += pay;
                cout << (currentBet == 0 ? " [ 체크 ]" : " [ 콜 ]") << endl;
            }
        }
    }
    return callAgainstAllIn;
}

int Poker::evaluateWinner() {
    int winnerIdx = -1;
    long long bestStrength = -1;
    int bestSuitScore = -1;

    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) continue;
        long long curStrength = GetHandStrength(players[i].hand);

        int curMaxSuit = -1;
        for (auto& c : players[i].hand) {
            int s = GetCardScore(c.rank, c.suit);
            if (s > curMaxSuit) curMaxSuit = s;
        }

        if (winnerIdx == -1 || curStrength > bestStrength) {
            bestStrength = curStrength; winnerIdx = i; bestSuitScore = curMaxSuit;
        }
        else if (curStrength == bestStrength && curMaxSuit > bestSuitScore) {
            winnerIdx = i; bestSuitScore = curMaxSuit;
        }
    }
    return winnerIdx == -1 ? 0 : winnerIdx;
}

HandRank Poker::checkHandForPlayer(vector<PokerCard>& hand) {
    long long s = GetHandStrength(hand);
    if (s >= 8000000) return HandRank::FOUR_OF_A_KIND;
    if (s >= 7000000) return HandRank::FULL_HOUSE;
    if (s >= 4000000) return HandRank::THREE_OF_A_KIND;
    if (s >= 3000000) return HandRank::TWO_PAIR;
    if (s >= 2000000) return HandRank::ONE_PAIR;
    return HandRank::HIGH_CARD;
}