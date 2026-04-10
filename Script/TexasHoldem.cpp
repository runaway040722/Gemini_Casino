#include "TexasHoldem.h"
#include "System.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <random>
#include <vector>
#include <string>

using namespace std;

extern vector<string> globalNamePool;

struct THPlayer {
    string name;
    vector<PokerCard> hand;
    int money;
    bool isFolded;
};

// 숫자를 문자열로 변환 (14->A, 11->J 등)
string GetRankName(int rank) {
    if (rank == 14 || rank == 1) return "A";
    if (rank == 11) return "J";
    if (rank == 12) return "Q";
    if (rank == 13) return "K";
    return to_string(rank);
}

string TexasHoldem::getCardName(PokerCard c) {
    string s_mark;
    if (c.suit == 0) s_mark = "♠";
    else if (c.suit == 1) s_mark = "♥";
    else if (c.suit == 2) s_mark = "◆";
    else s_mark = "♣";
    return s_mark + GetRankName(c.rank);
}

void TexasHoldem::initDeck() {
    deck.clear();
    for (int s = 0; s < 4; s++) {
        for (int r = 1; r <= 13; r++) deck.push_back({ s, r });
    }
}

void TexasHoldem::shuffleDeck() {
    random_device rd;
    mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

PokerCard TexasHoldem::drawCard() {
    if (deck.empty()) return { 0, 0 };
    PokerCard c = deck.back();
    deck.pop_back();
    return c;
}

long long TexasHoldem::evaluateHand(vector<PokerCard>& hand, vector<PokerCard>& community) {
    vector<PokerCard> all = community;
    all.insert(all.end(), hand.begin(), hand.end());

    int counts[15] = { 0 };
    int suits[4] = { 0 };
    for (auto& c : all) {
        int r = (c.rank == 1) ? 14 : c.rank;
        counts[r]++;
        suits[c.suit]++;
    }

    // 1. 플러시 체크
    int flushSuit = -1;
    for (int i = 0; i < 4; i++) { if (suits[i] >= 5) flushSuit = i; }

    // 2. 스트레이트 체크 (가장 높은 카드 기준)
    int straightHigh = 0;
    int consecutive = 0;
    for (int i = 14; i >= 2; i--) {
        if (counts[i] > 0) {
            consecutive++;
            if (consecutive >= 5) { straightHigh = i + 4; break; }
        }
        else consecutive = 0;
    }
    // 백스트레이트 특수 판정 (A, 2, 3, 4, 5)
    if (straightHigh == 0 && counts[14] > 0 && counts[2] > 0 && counts[3] > 0 && counts[4] > 0 && counts[5] > 0) {
        straightHigh = 5;
    }

    // 3. 포카드, 트리플, 페어 체크
    int fours = 0, triples = 0;
    vector<int> pairs;
    int high = 0;
    for (int i = 14; i >= 2; i--) {
        if (counts[i] == 4) fours = i;
        else if (counts[i] == 3) { if (triples == 0) triples = i; else pairs.push_back(i); }
        else if (counts[i] == 2) { pairs.push_back(i); }
        if (counts[i] > 0 && high == 0) high = i;
    }

    // 4. 점수 반환 (족보 우선순위)
    if (flushSuit != -1 && straightHigh > 0) return 9000000LL + straightHigh;
    if (fours > 0) return 8000000LL + fours;
    if (triples > 0 && !pairs.empty()) return 7000000LL + (triples * 100) + pairs[0];
    if (flushSuit != -1) return 6000000LL + high;
    if (straightHigh > 0) return 5000000LL + straightHigh;
    if (triples > 0) return 4000000LL + triples;
    if (pairs.size() >= 2) return 3000000LL + (pairs[0] * 100) + pairs[1];
    if (pairs.size() == 1) return 2000000LL + pairs[0];
    return 1000000LL + high;
}

void TexasHoldem::Play() {
    system("cls");

    vector<THPlayer> players;
    players.push_back({ "나 (Player)", {}, playerMoney, false });

    random_device rd;
    mt19937 g(rd());
    vector<string> tempPool = globalNamePool;
    std::shuffle(tempPool.begin(), tempPool.end(), g);
    for (int i = 0; i < 3; i++) players.push_back({ tempPool[i], {}, 100000, false });

    initDeck();
    shuffleDeck();
    communityCards.clear();
    int pot = 0;
    int baseBet = 5000;

    for (int i = 0; i < 2; i++) {
        for (auto& p : players) p.hand.push_back(drawCard());
    }

    int rounds[] = { 0, 3, 1, 1 };
    string roundNames[] = { "Pre-Flop", "Flop", "Turn", "River" };

    for (int r = 0; r < 4; r++) {
        for (int i = 0; i < rounds[r]; i++) communityCards.push_back(drawCard());

        system("cls");
        SetColor(14);
        cout << "==========================================================" << endl;
        cout << "                [ 4인 텍사스 홀덤 - " << roundNames[r] << " ]" << endl;
        cout << "          현재 총 판돈(POT): " << pot << " 달러" << endl;
        cout << "==========================================================" << endl;

        SetColor(15);
        cout << " 공유 카드 : ";
        if (communityCards.empty()) cout << "(공개 전)";
        else { for (auto& c : communityCards) cout << getCardName(c) << " "; }
        cout << "\n----------------------------------------------------------" << endl;

        for (int i = 0; i < 4; i++) {
            if (players[i].isFolded) {
                SetColor(8); cout << left << setw(20) << players[i].name << " : [ 기권(FOLDED) ]" << endl;
            }
            else {
                SetColor(i == 0 ? 11 : 15);
                cout << left << setw(20) << players[i].name << " : ";
                if (i == 0) cout << getCardName(players[0].hand[0]) << " " << getCardName(players[0].hand[1]);
                else cout << "?? ?? ";
                int m = (i == 0) ? playerMoney : players[i].money;
                cout << " (잔고: " << m << "$)" << endl;
            }
        }
        SetColor(14); cout << "==========================================================" << endl;
        SetColor(15);

        for (int i = 0; i < 4; i++) {
            if (players[i].isFolded) continue;
            if (i == 0) {
                cout << "\n [1]콜(" << baseBet << "$) [2]레이즈(" << baseBet * 2 << "$) [3]올인 [4]다이 : ";
                int choice; cin >> choice;
                if (choice == 4) { players[0].isFolded = true; break; }
                int bet = (choice == 2) ? baseBet * 2 : (choice == 3 ? playerMoney : baseBet);
                pot += bet; playerMoney -= bet;
            }
            else {
                cout << " " << players[i].name << " 생각 중..."; Sleep(400);
                long long s = evaluateHand(players[i].hand, communityCards);
                if (s < 2000000 && r >= 3 && (rand() % 10 < 4)) {
                    players[i].isFolded = true; cout << "[다이]" << endl;
                }
                else {
                    int ab = baseBet; pot += ab; players[i].money -= ab;
                    cout << "[콜]" << endl;
                }
            }
        }
        int active = 0;
        for (auto& p : players) if (!p.isFolded) active++;
        if (active <= 1) break;
    }

    system("cls");
    SetColor(14);
    cout << "\n==========================================================" << endl;
    cout << "                    [ 최 종 결 과 ]" << endl;
    cout << "==========================================================" << endl;
    SetColor(15);
    cout << " [공유 패] : ";
    for (auto& c : communityCards) cout << getCardName(c) << " ";
    cout << "\n----------------------------------------------------------" << endl;

    long long bestScore = -1; int winIdx = -1;
    for (int i = 0; i < 4; i++) {
        cout << " " << left << setw(18) << players[i].name << " : ";
        if (players[i].isFolded) {
            SetColor(8); cout << "[ 기권(FOLDED) ]" << endl; SetColor(15); continue;
        }
        cout << getCardName(players[i].hand[0]) << " " << getCardName(players[i].hand[1]);

        long long s = evaluateHand(players[i].hand, communityCards);
        string handName = "";

        if (s >= 9000000LL) handName = "스트레이트 플러시!";
        else if (s >= 8000000LL) handName = GetRankName(s % 100) + " 포카드";
        else if (s >= 7000000LL) handName = GetRankName((s % 10000) / 100) + " & " + GetRankName(s % 100) + " 풀하우스";
        else if (s >= 6000000LL) handName = "플러시(Flush)";
        else if (s >= 5000000LL) {
            int sh = s % 100;
            if (sh == 14) handName = "마운틴 (A 스트레이트)";
            else if (sh == 5) handName = "백스트레이트 (5 스트레이트)";
            else handName = GetRankName(sh) + " 하이 스트레이트";
        }
        else if (s >= 4000000LL) handName = GetRankName(s % 100) + " 트리플";
        else if (s >= 3000000LL) handName = GetRankName((s % 10000) / 100) + ", " + GetRankName(s % 100) + " 투 페어";
        else if (s >= 2000000LL) handName = GetRankName(s % 100) + " 원 페어";
        else handName = GetRankName(s % 100) + " 하이 카드";

        cout << " -> " << handName << endl;
        if (s > bestScore) { bestScore = s; winIdx = i; }
    }

    cout << "----------------------------------------------------------" << endl;
    if (winIdx != -1) {
        SetColor(10);
        cout << " ★ 승자: " << players[winIdx].name << " (" << pot << "$ 획득!) ★" << endl;
        if (winIdx == 0) playerMoney += pot;
    }
    SetColor(15);
    cout << "\n 아무 키나 누르면 메뉴로 돌아갑니다.";
    (void)_getch();
}