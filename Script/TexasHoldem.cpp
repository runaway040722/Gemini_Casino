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

// [보조] 숫자 변환
static string GetRankNameLocal(int rank) {
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
    int r = (c.rank == 1) ? 14 : c.rank;
    return s_mark + GetRankNameLocal(r);
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
    int counts[15] = { 0 }, suits[4] = { 0 };
    for (auto& c : all) {
        int r = (c.rank == 1) ? 14 : c.rank;
        counts[r]++; suits[c.suit]++;
    }
    int flushSuit = -1;
    for (int i = 0; i < 4; i++) { if (suits[i] >= 5) flushSuit = i; }
    int straightHigh = 0, consecutive = 0;
    for (int i = 14; i >= 2; i--) {
        if (counts[i] > 0) {
            consecutive++;
            if (consecutive >= 5) { straightHigh = i + 4; break; }
        }
        else consecutive = 0;
    }
    if (straightHigh == 0 && counts[14] && counts[2] && counts[3] && counts[4] && counts[5]) straightHigh = 5;
    int fours = 0, triples = 0;
    vector<int> pairs;
    int high = 0;
    for (int i = 14; i >= 2; i--) {
        if (counts[i] == 4) fours = i;
        else if (counts[i] == 3) { if (triples == 0) triples = i; else pairs.push_back(i); }
        else if (counts[i] == 2) { pairs.push_back(i); }
        if (counts[i] > 0 && high == 0) high = i;
    }
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
    SetColor(11);
    cout << "\n\n          +------------------------------------------+" << endl;
    cout << "          |          [ 홀덤 테이블 입장 ]            |" << endl;
    cout << "          +------------------------------------------+" << endl;
    cout << "          |    1. [ 초보 ] 판돈 :  1,000 $           |" << endl;
    cout << "          |    2. [ 일반 ] 판돈 :  5,000 $           |" << endl;
    cout << "          |    3. [ 고수 ] 판돈 : 10,000 $           |" << endl;
    cout << "          |    4. [ 메이저] 판돈 : 20,000 $          |" << endl;
    cout << "          +------------------------------------------+" << endl;
    SetColor(15);
    cout << "\n           선택 (1~4): ";
    int menu, baseBet;
    if (!(cin >> menu)) { cin.clear(); cin.ignore(1000, '\n'); menu = 2; }
    if (menu == 1) baseBet = 1000; else if (menu == 3) baseBet = 10000; else if (menu == 4) baseBet = 20000; else baseBet = 5000;

    vector<THPlayer> players;
    players.push_back({ "나 (Player)", {}, playerMoney, false });
    random_device rd; mt19937 g(rd());
    vector<string> tempPool = globalNamePool;
    std::shuffle(tempPool.begin(), tempPool.end(), g);
    for (int i = 0; i < 3; i++) players.push_back({ tempPool[i], {}, 100000, false });

    while (true) {
        if (playerMoney < baseBet) {
            cout << "\n [!] 자산 부족으로 퇴장당했습니다." << endl; Sleep(1500); return;
        }

        // 파산 체크
        for (int i = 1; i < 4; i++) {
            if (players[i].money < baseBet) {
                SetColor(12); cout << "\n [!] " << players[i].name << " 파산! 광산행.." << endl;
                players[i].name = globalNamePool[rand() % globalNamePool.size()];
                players[i].money = 100000;
                SetColor(10); cout << " [*] 새로운 참가자 " << players[i].name << " 입장." << endl;
                Sleep(1000);
            }
        }

        initDeck(); shuffleDeck();
        communityCards.clear();
        int pot = 0; int currentRoundBet = 0;
        for (auto& p : players) { p.hand.clear(); p.isFolded = false; }

        // 앤티
        int ante = baseBet / 2;
        for (int i = 0; i < 4; i++) {
            int& m = (i == 0) ? playerMoney : players[i].money;
            m -= ante; pot += ante;
        }

        for (int i = 0; i < 2; i++) { for (auto& p : players) p.hand.push_back(drawCard()); }

        int roundsAdd[] = { 0, 3, 1, 1 };
        string roundNames[] = { "Pre-Flop", "Flop", "Turn", "River" };

        for (int r = 0; r < 4; r++) {
            for (int i = 0; i < roundsAdd[r]; i++) communityCards.push_back(drawCard());
            currentRoundBet = baseBet; // 라운드마다 기본 콜 금액 설정

            while (true) { // 베팅 루프
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
                    if (players[i].isFolded) { SetColor(8); cout << left << setw(20) << players[i].name << " : [ FOLD ]" << endl; }
                    else {
                        SetColor(i == 0 ? 11 : 15);
                        cout << left << setw(20) << players[i].name << " : ";
                        if (i == 0) cout << getCardName(players[0].hand[0]) << " " << getCardName(players[0].hand[1]);
                        else cout << "?? ?? ";
                        cout << " (잔고: " << (i == 0 ? playerMoney : players[i].money) << "$)" << endl;
                    }
                }
                SetColor(14); cout << "==========================================================" << endl; SetColor(15);

                bool aiRaised = false;
                for (int i = 1; i < 4; i++) {
                    if (players[i].isFolded || players[i].money <= 0) continue;
                    long long s = evaluateHand(players[i].hand, communityCards);
                    cout << " " << players[i].name << " 생각 중..."; Sleep(300);

                    if (s >= 4000000 && rand() % 10 < 3) { // AI 올인 시도
                        int allIn = players[i].money;
                        pot += allIn; players[i].money = 0;
                        currentRoundBet = max(currentRoundBet, allIn);
                        SetColor(12); cout << "[ ALL-IN! ]" << endl; SetColor(15);
                        aiRaised = true;
                    }
                    else if (s < 2000000 && r >= 2 && rand() % 10 < 2) {
                        players[i].isFolded = true; cout << "[ FOLD ]" << endl;
                    }
                    else {
                        int callAmt = min(players[i].money, currentRoundBet);
                        pot += callAmt; players[i].money -= callAmt; cout << "[ CALL ]" << endl;
                    }
                }

                // 플레이어 응답
                if (!players[0].isFolded) {
                    cout << "\n [현재 콜 금액: " << currentRoundBet << "$]";
                    cout << "\n [1]콜 [2]올인 [3]다이 : ";
                    int choice; cin >> choice;
                    if (choice == 3) { players[0].isFolded = true; }
                    else if (choice == 2) {
                        pot += playerMoney; currentRoundBet = max(currentRoundBet, playerMoney); playerMoney = 0;
                    }
                    else {
                        int pay = min(playerMoney, currentRoundBet);
                        playerMoney -= pay; pot += pay;
                    }
                }
                break; // 한 바퀴 돌면 다음 라운드
            }
            int active = 0; for (auto& p : players) if (!p.isFolded) active++;
            if (active <= 1) break;
        }

        // 결과
        system("cls");
        SetColor(14); cout << "\n [ 최 종 결 과 ] \n"; SetColor(15);
        long long bestScore = -1; int winIdx = -1;
        for (int i = 0; i < 4; i++) {
            if (players[i].isFolded) continue;
            long long s = evaluateHand(players[i].hand, communityCards);
            string hName;
            int m = s % 100, sub = (s % 10000) / 100;
            if (s >= 9000000) hName = GetRankNameLocal(m) + " 스트레이트 플러시";
            else if (s >= 8000000) hName = GetRankNameLocal(m) + " 포카드";
            else if (s >= 7000000) hName = GetRankNameLocal(sub) + " & " + GetRankNameLocal(m) + " 풀하우스";
            else if (s >= 6000000) hName = GetRankNameLocal(m) + " 플러시";
            else if (s >= 5000000) hName = GetRankNameLocal(m) + " 스트레이트";
            else if (s >= 4000000) hName = GetRankNameLocal(m) + " 트리플";
            else if (s >= 3000000) hName = GetRankNameLocal(sub) + " & " + GetRankNameLocal(m) + " 투 페어";
            else if (s >= 2000000) hName = GetRankNameLocal(m) + " 원 페어";
            else hName = GetRankNameLocal(m) + " 하이카드";

            cout << " " << players[i].name << " : " << getCardName(players[i].hand[0]) << " " << getCardName(players[i].hand[1]) << " -> " << hName << endl;
            if (s > bestScore) { bestScore = s; winIdx = i; }
        }
        if (winIdx != -1) {
            SetColor(10); cout << "\n ★ 승자: " << players[winIdx].name << " (" << pot << "$ 획득) ★\n";
            if (winIdx == 0) playerMoney += pot; else players[winIdx].money += pot;
        }
        SetColor(15); cout << "\n [1]다음 [0]중단 : ";
        int c; cin >> c; if (c == 0) break;
    }
}