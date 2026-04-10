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

// [보조 함수] 카드의 숫자 위력 계산 (A는 14)
int GetCardPower(int rank) {
    return (rank == 1) ? 14 : rank;
}

// [보조 함수] 무늬 점수 포함 (동점자 처리용)
int GetCardScore(int rank, int suit) {
    int score = GetCardPower(rank) * 10;
    if (suit == 0) score += 3;      // ♠
    else if (suit == 2) score += 2; // ◆
    else if (suit == 1) score += 1; // ♥
    else if (suit == 3) score += 0; // ♣
    return score;
}

// [보조 함수] 족보 강도 계산
long long GetHandStrength(vector<PokerCard>& hand) {
    if (hand.empty()) return 0;
    int counts[15] = { 0 };
    int suits[4] = { 0 };
    for (auto& c : hand) {
        int r = GetCardPower(c.rank);
        counts[r]++;
        suits[c.suit]++;
    }
    bool isFlush = false;
    for (int i = 0; i < 4; i++) { if (suits[i] >= 5) isFlush = true; }
    int straightHigh = 0, consecutive = 0;
    for (int i = 14; i >= 2; i--) {
        if (counts[i] > 0) {
            consecutive++;
            if (consecutive >= 5) { straightHigh = i + 4; break; }
        }
        else consecutive = 0;
    }
    if (straightHigh == 0 && counts[14] && counts[2] && counts[3] && counts[4] && counts[5]) straightHigh = 5;

    int fours = 0, triples = 0, high = 0;
    vector<int> pairs;
    for (int i = 14; i >= 2; i--) {
        if (counts[i] == 4) fours = i;
        else if (counts[i] == 3) triples = i;
        else if (counts[i] == 2) pairs.push_back(i);
        if (counts[i] > 0 && i > high) high = i;
    }

    if (isFlush && straightHigh > 0) return 9000000LL + straightHigh;
    if (fours > 0) return 8000000LL + fours;
    if (triples > 0 && !pairs.empty()) return 7000000LL + (triples * 100) + pairs[0];
    if (isFlush) return 6000000LL + high;
    if (straightHigh > 0) return 5000000LL + straightHigh;
    if (triples > 0) return 4000000LL + triples;
    if (pairs.size() >= 2) return 3000000LL + (pairs[0] * 100) + pairs[1];
    if (!pairs.empty()) return 2000000LL + pairs[0];
    return 1000000LL + high;
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
    if (rank == 14 || rank == 1) return "A";
    if (rank == 11) return "J";
    if (rank == 12) return "Q";
    if (rank == 13) return "K";
    return to_string(rank);
}

string Poker::GetDetailedRankName(vector<PokerCard>& hand) {
    long long s = GetHandStrength(hand);
    int mainValue = s % 100;
    if (s >= 9000000LL) return "스트레이트 플러시!";
    if (s >= 8000000LL) return GetRankStr(mainValue) + " 포카드";
    if (s >= 7000000LL) return GetRankStr((s % 10000) / 100) + " 풀하우스";
    if (s >= 6000000LL) return "플러시(Flush)";
    if (s >= 5000000LL) {
        if (mainValue == 14) return "마운틴";
        if (mainValue == 5) return "백스트레이트";
        return GetRankStr(mainValue) + " 스트레이트";
    }
    if (s >= 4000000LL) return GetRankStr(mainValue) + " 트리플";
    if (s >= 3000000LL) return GetRankStr((s % 10000) / 100) + " 투 페어";
    if (s >= 2000000LL) return GetRankStr(mainValue) + " 원 페어";
    return GetRankStr(mainValue) + " 하이";
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
            SetColor(8); cout << left << setw(20) << players[i].name << " : [ 기권(FOLDED) ]" << endl;
        }
        else {
            SetColor(i == 0 ? 11 : 15);
            cout << left << setw(20) << players[i].name << " : ";
            for (auto& card : players[i].hand) {
                if (i == 0 || revealAll) {
                    if (card.suit == 1 || card.suit == 2) SetColor(12); else SetColor(15);
                    string sm; if (card.suit == 0)sm = "♠"; else if (card.suit == 1)sm = "♥"; else if (card.suit == 2)sm = "◆"; else sm = "♣";
                    cout << sm << GetRankStr(card.rank) << " ";
                    SetColor(i == 0 ? 11 : 15);
                }
                else cout << "?? ";
            }
            cout << " (잔고: " << (i == 0 ? playerRef : players[i].money) << "$)";
            if ((i == 0 || revealAll) && !players[i].hand.empty()) {
                SetColor(10); cout << " (" << GetDetailedRankName(players[i].hand) << ")";
            }
            cout << endl;
        }
    }
    SetColor(15); cout << "==========================================================" << endl;
}

void Poker::play() {
    system("cls");

    SetColor(11);
    cout << "\n\n";
    cout << "          +------------------------------------------+" << endl;
    cout << "          |          [ 포커 테이블 입장 ]            |" << endl;
    cout << "          +------------------------------------------+" << endl;
    cout << "          |                                          |" << endl;
    cout << "          |    원하시는 베팅 한도를 선택해주세요.    |" << endl;
    cout << "          |                                          |" << endl;
    cout << "          |    1. [ 초보 ] 판돈 :  1,000 $           |" << endl;
    cout << "          |    2. [ 일반 ] 판돈 :  5,000 $           |" << endl;
    cout << "          |    3. [ 고수 ] 판돈 : 10,000 $           |" << endl;
    cout << "          |    4. [ 메이저] 판돈 : 20,000 $          |" << endl;
    cout << "          |                                          |" << endl;
    cout << "          +------------------------------------------+" << endl;
    SetColor(15);
    cout << "\n           선택 (1~4): ";

    int menu, baseBet = 5000;
    if (!(cin >> menu)) { cin.clear(); cin.ignore(1000, '\n'); menu = 2; }

    if (menu == 1) baseBet = 1000;
    else if (menu == 3) baseBet = 10000;
    else if (menu == 4) baseBet = 20000;
    else baseBet = 5000;

    cout << "\n           [ " << baseBet << "$ ] 테이블로 이동 중입니다...";
    Sleep(1000);

    random_device rd; mt19937 g(rd());
    uniform_int_distribution<int> dist(0, (int)globalNamePool.size() - 1);

    while (true) {
        if (playerRef < baseBet) {
            system("cls"); SetColor(12);
            cout << "\n\n [!] 자산 부족으로 퇴장당했습니다. (보유: " << playerRef << "$)" << endl;
            Sleep(1500); return;
        }

        bool replaced = false;
        for (int i = 1; i < (int)players.size(); i++) {
            if (players[i].money < baseBet) {
                SetColor(12);
                cout << "\n [!] " << players[i].name << "님이 파산하여 광산으로 끌려갑니다..." << endl;
                Sleep(1200);

                players[i].name = globalNamePool[dist(g)];
                players[i].money = 100000;

                SetColor(10);
                cout << " [*] 새로운 참가자 " << players[i].name << "님이 테이블에 앉았습니다." << endl;
                replaced = true;
            }
        }
        if (replaced) { cout << "\n 잠시 후 게임을 다시 시작합니다..."; Sleep(1500); }

        int ante = baseBet / 2; currentBet = 0; initDeck(); shuffleDeck();
        int deckIdx = 0; pot = 0;

        for (int i = 0; i < (int)players.size(); i++) {
            int& m = (i == 0) ? playerRef : players[i].money;
            m -= ante; pot += ante;
            players[i].hand.clear(); players[i].isFolded = false;
        }

        for (int i = 0; i < 2; i++) { for (auto& p : players) p.hand.push_back(deck[deckIdx++]); }

        for (int round = 1; round <= 4; round++) {
            showTable(false);
            bool isAllInCalled = bettingRound();
            int active = 0; for (auto& p : players) if (!p.isFolded) active++;
            if (active <= 1) break;

            if (isAllInCalled) {
                SetColor(12); cout << "\n [ 올인 발생! 카드 공개! ]" << endl; Sleep(1000);
                while (players[0].hand.size() < 5) { for (auto& p : players) if (!p.isFolded) p.hand.push_back(deck[deckIdx++]); }
                break;
            }
            if (round < 4) {
                cout << "\n [ 카드 배분 중... ]" << endl; Sleep(600);
                for (auto& p : players) if (!p.isFolded) p.hand.push_back(deck[deckIdx++]);
            }
        }

        showTable(true);
        int winnerIdx = evaluateWinner();
        SetColor(10);
        cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
        cout << " 승자: " << players[winnerIdx].name << " (" << GetDetailedRankName(players[winnerIdx].hand) << ")" << endl;
        cout << " 상금: " << pot << "$" << endl;
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;

        if (winnerIdx == 0) playerRef += pot; else players[winnerIdx].money += pot;

        cout << "\n [1] 다음 판 [0] 나가기 : ";
        int cont; cin >> cont; if (cont == 0) break;
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
                int bet = min(curM, (pot / 2) + currentBet);
                curM -= bet; pot += bet; currentBet = bet;
            }
            else if (choice == 3) {
                // [올인 최적화] 살아있는 상대방 중 가장 큰 자산가 기준 배팅
                int maxOpponentMoney = 0;
                for (int j = 1; j < (int)players.size(); j++) {
                    if (!players[j].isFolded) maxOpponentMoney = max(maxOpponentMoney, players[j].money);
                }
                int actualAllIn = min(curM, maxOpponentMoney + currentBet);
                pot += actualAllIn; currentBet = max(currentBet, actualAllIn); curM -= actualAllIn;
                callAgainstAllIn = true;
            }
            else if (choice == 4) players[i].isFolded = true;
            else { int pay = min(currentBet, curM); curM -= pay; pot += pay; }
        }
        else {
            cout << " " << players[i].name << " 베팅 중..."; Sleep(500);
            long long strength = GetHandStrength(players[i].hand);
            if (currentBet > curM * 0.4) {
                if (strength >= 2000000 || (rand() % 100 < 20)) {
                    int pay = min(currentBet, curM); curM -= pay; pot += pay;
                    if (curM == 0) callAgainstAllIn = true;
                    cout << "[콜]" << endl;
                }
                else { players[i].isFolded = true; cout << "[다이]" << endl; }
            }
            else { int pay = min(currentBet, curM); curM -= pay; pot += pay; cout << "[콜]" << endl; }
        }
    }
    return callAgainstAllIn;
}

int Poker::evaluateWinner() {
    int winnerIdx = -1; long long bestStrength = -1; int bestSuitScore = -1;
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) continue;
        long long curStrength = GetHandStrength(players[i].hand);
        int curMaxSuitScore = -1;
        for (auto& c : players[i].hand) {
            int s = GetCardScore(c.rank, c.suit);
            if (s > curMaxSuitScore) curMaxSuitScore = s;
        }
        if (winnerIdx == -1 || curStrength > bestStrength) {
            bestStrength = curStrength; winnerIdx = i; bestSuitScore = curMaxSuitScore;
        }
        else if (curStrength == bestStrength && curMaxSuitScore > bestSuitScore) {
            winnerIdx = i; bestSuitScore = curMaxSuitScore;
        }
    }
    return (winnerIdx == -1) ? 0 : winnerIdx;
}