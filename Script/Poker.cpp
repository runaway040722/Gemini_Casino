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

int GetCardScore(int rank, int suit) {
    int power = (rank == 1) ? 14 : rank;
    int score = power * 10;
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

    for (int i = 0; i < 3; i++) {
        players.push_back({ tempPool[i], {}, 100000, false });
    }
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

string Poker::GetDetailedRankName(vector<PokerCard>& hand) {
    if (hand.empty()) return "";
    int counts[15] = { 0 };
    for (auto& c : hand) counts[c.rank]++;
    vector<int> fours, triples, pairs;
    int priority[] = { 1, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 };

    for (int r : priority) {
        if (counts[r] == 4) fours.push_back(r);
        else if (counts[r] == 3) triples.push_back(r);
        else if (counts[r] == 2) pairs.push_back(r);
    }

    if (!fours.empty()) return GetRankStr(fours[0]) + " 포카드";
    if (!triples.empty() && !pairs.empty()) return GetRankStr(triples[0]) + " 풀하우스";
    if (!triples.empty()) return GetRankStr(triples[0]) + " 트리플";
    if (pairs.size() >= 2) return GetRankStr(pairs[0]) + " " + GetRankStr(pairs[1]) + " 투 페어";
    if (pairs.size() == 1) return GetRankStr(pairs[0]) + " 원 페어";
    return "하이 카드";
}

void Poker::showTable(bool revealAll) {
    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 4인 포커 - 5카드 하이 ]                " << endl;
    cout << "          현재 총 판돈(POT): " << pot << " 달러" << endl;
    cout << "==========================================================" << endl;
    SetColor(15);

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
    SetColor(11);
    cout << "\n  [ 포커 테이블 입장 ]" << endl;
    SetColor(15);
    cout << "  베팅 단위: 1. 1,000 | 2. 5,000 | 3. 10,000 | 4. 20,000\n  선택: ";

    int menu, baseBet = 5000;
    cin >> menu;
    if (menu == 1) baseBet = 1000;
    else if (menu == 2) baseBet = 5000;
    else if (menu == 3) baseBet = 10000;
    else if (menu == 4) baseBet = 20000;

    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<int> dist(0, (int)globalNamePool.size() - 1);

    while (true) {
        if (playerRef < baseBet) {
            system("cls");
            SetColor(12);
            cout << "\n [!] 자산 부족으로 퇴장당했습니다." << endl;
            Sleep(1500); return;
        }

        for (int i = 1; i < (int)players.size(); i++) {
            if (players[i].money < baseBet) {
                SetColor(11);
                string newName = globalNamePool[dist(g)];
                cout << "\n [!] " << players[i].name << " 파산! [" << newName << "] 입장!" << endl;
                players[i].name = newName;
                players[i].money = 100000;
                Sleep(1000);
            }
        }

        int ante = baseBet / 2;
        currentBet = 0;
        initDeck();
        shuffleDeck();
        int deckIdx = 0;
        pot = 0;

        for (int i = 0; i < (int)players.size(); i++) {
            int& m = (i == 0) ? playerRef : players[i].money;
            m -= ante; pot += ante;
            players[i].hand.clear();
            players[i].isFolded = false;
        }

        // 1. 초기 2장 배분
        for (int i = 0; i < 2; i++) {
            for (auto& p : players) p.hand.push_back(deck[deckIdx++]);
        }

        // 2. 4번의 베팅 라운드 (2->3->4->5장 순서)
        for (int round = 1; round <= 4; round++) {
            showTable(false);
            bettingRound();

            int active = 0;
            for (auto& p : players) if (!p.isFolded) active++;
            if (active <= 1) break;

            if (round < 4) {
                cout << "\n [ " << round + 2 << "번째 카드를 배분합니다... ]" << endl;
                Sleep(800);
                for (auto& p : players) {
                    if (!p.isFolded) p.hand.push_back(deck[deckIdx++]);
                }
            }
            else {
                cout << "\n 모든 카드가 배분되었습니다. 결과를 확인하려면 아무 키나 누르세요..." << endl;
                _getch();
            }
        }

        showTable(true);
        int winnerIdx = evaluateWinner();

        SetColor(10);
        cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
        cout << " 승자: " << players[winnerIdx].name << endl;
        cout << " 결과: " << GetDetailedRankName(players[winnerIdx].hand) << endl;
        cout << " 상금: " << pot << "$" << endl;
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;

        if (winnerIdx == 0) playerRef += pot;
        else players[winnerIdx].money += pot;

        SetColor(15);
        cout << "\n [1] 계속 [0] 퇴장 : ";
        int cont; cin >> cont;
        if (cont == 0) break;
    }
}

bool Poker::bettingRound() {
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) continue;
        if (i == 0) {
            int halfBet = pot / 2;
            cout << "\n베팅 (현재 베팅액: " << currentBet << "$)";
            cout << "\n[1]콜/체크 [2]하프(" << halfBet << ") [3]올인 [4]다이 : ";
            int choice; cin >> choice;
            if (choice == 2) {
                int betAmount = (playerRef < halfBet) ? playerRef : halfBet;
                playerRef -= betAmount; pot += betAmount; currentBet = betAmount;
            }
            else if (choice == 3) {
                pot += playerRef; currentBet += playerRef; playerRef = 0;
            }
            else if (choice == 4) players[i].isFolded = true;
            else {
                int pay = (currentBet > playerRef) ? playerRef : currentBet;
                playerRef -= pay; pot += pay;
            }
        }
        else {
            cout << players[i].name << " 생각 중..."; Sleep(500);
            HandRank aiRank = checkHandForPlayer(players[i].hand);
            int prob = rand() % 100;

            if (currentBet > players[i].money * 0.4 && aiRank < HandRank::ONE_PAIR) {
                players[i].isFolded = true; cout << " [다이]" << endl;
            }
            else if (aiRank >= HandRank::THREE_OF_A_KIND && prob < 30) {
                cout << " [올인]" << endl;
                pot += players[i].money; currentBet += players[i].money; players[i].money = 0;
            }
            else {
                int pay = (currentBet > players[i].money) ? players[i].money : currentBet;
                players[i].money -= pay; pot += pay;
                cout << (currentBet == 0 ? " [체크]" : " [콜]") << endl;
            }
        }
    }
    return true;
}

HandRank Poker::checkHandForPlayer(vector<PokerCard>& hand) {
    if (hand.empty()) return HandRank::HIGH_CARD;
    int counts[15] = { 0 };
    for (auto& c : hand) counts[c.rank]++;
    int pairs = 0, triples = 0, fours = 0;
    for (int i = 1; i <= 14; i++) {
        if (counts[i] == 4) fours++;
        else if (counts[i] == 3) triples++;
        else if (counts[i] == 2) pairs++;
    }
    if (fours >= 1) return HandRank::FOUR_OF_A_KIND;
    if (triples >= 1 && pairs >= 1) return HandRank::FULL_HOUSE;
    if (triples >= 1) return HandRank::THREE_OF_A_KIND;
    if (pairs >= 2) return HandRank::TWO_PAIR;
    if (pairs == 1) return HandRank::ONE_PAIR;
    return HandRank::HIGH_CARD;
}

int Poker::evaluateWinner() {
    int winnerIdx = -1;
    HandRank bestRank = (HandRank)-1;
    int bestHighCardScore = -1;

    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) continue;
        HandRank currentRank = checkHandForPlayer(players[i].hand);
        int currentMaxScore = -1;
        for (auto& c : players[i].hand) {
            int s = GetCardScore(c.rank, c.suit);
            if (s > currentMaxScore) currentMaxScore = s;
        }
        if (winnerIdx == -1 || (int)currentRank > (int)bestRank) {
            bestRank = currentRank;
            winnerIdx = i;
            bestHighCardScore = currentMaxScore;
        }
        else if (currentRank == bestRank) {
            if (currentMaxScore > bestHighCardScore) {
                winnerIdx = i;
                bestHighCardScore = currentMaxScore;
            }
        }
    }
    return (winnerIdx == -1) ? 0 : winnerIdx;
}