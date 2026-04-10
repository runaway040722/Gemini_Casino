#include "TexasHoldem.h"
#include "System.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include <windows.h>
#include <conio.h>

using namespace std;

// --- [유틸리티 함수] ---
string GetRankName(int rank) {
    if (rank == 14 || rank == 1) return "A";
    if (rank == 11) return "J";
    if (rank == 12) return "Q";
    if (rank == 13) return "K";
    return to_string(rank);
}

string GetDetailedHandName(long long score) {
    long long type = score / 1000000LL;
    int mainRank = (score % 1000000LL) / 10000;
    int subRank = (score % 10000LL) / 100;
    switch (type) {
    case 9: return "스트레이트 플러시";
    case 8: return GetRankName(mainRank) + " 포카드";
    case 7: return GetRankName(mainRank) + " " + GetRankName(subRank) + " 풀하우스";
    case 6: return "플러시";
    case 5: return "스트레이트";
    case 4: return GetRankName(mainRank) + " 트리플";
    case 3: return GetRankName(mainRank) + " " + GetRankName(subRank) + " 투 페어";
    case 2: return GetRankName(mainRank) + " 원 페어";
    case 1: return GetRankName(mainRank) + " 하이 카드";
    default: return "데이터 없음";
    }
}

void PrintCard(int suit, int rank) {
    if (suit == 1 || suit == 2) SetColor(12); else SetColor(15);
    string s = (suit == 0 ? "♠" : suit == 1 ? "♥" : suit == 2 ? "◆" : "♣");
    int rNum = (rank == 14 || rank == 1) ? 1 : rank;
    string rStr = (rNum == 1 ? "A" : rNum == 11 ? "J" : rNum == 12 ? "Q" : rNum == 13 ? "K" : to_string(rNum));
    cout << s << rStr << " ";
}

// --- [핵심 로직 구현] ---

void TexasHoldem::initDeck() {
    deck.clear();
    for (int s = 0; s < 4; s++)
        for (int r = 1; r <= 13; r++)
            deck.push_back({ s, r });
}

void TexasHoldem::shuffleDeck() {
    random_device rd; mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

long long TexasHoldem::evaluateHand(vector<PokerCard>& hand, vector<PokerCard>& community) {
    vector<PokerCard> all = hand;
    all.insert(all.end(), community.begin(), community.end());
    int counts[15] = { 0 };
    vector<int> suitCards[4];
    for (auto& c : all) {
        int r = (c.rank == 1) ? 14 : c.rank;
        counts[r]++; suitCards[c.suit].push_back(r);
    }
    int flushSuit = -1;
    for (int i = 0; i < 4; i++) {
        sort(suitCards[i].rbegin(), suitCards[i].rend());
        if (suitCards[i].size() >= 5) flushSuit = i;
    }
    int straightHigh = 0, consecutive = 0;
    for (int i = 14; i >= 2; i--) {
        if (counts[i] > 0) { consecutive++; if (consecutive >= 5) { straightHigh = i; break; } }
        else consecutive = 0;
    }
    if (straightHigh == 0 && counts[14] && counts[2] && counts[3] && counts[4] && counts[5]) straightHigh = 5;

    int four = 0, triple = 0; vector<int> pairs, singles;
    for (int i = 14; i >= 2; i--) {
        if (counts[i] == 4) four = i;
        else if (counts[i] == 3) { if (triple == 0) triple = i; else pairs.push_back(i); }
        else if (counts[i] == 2) pairs.push_back(i);
        else if (counts[i] == 1) singles.push_back(i);
    }
    if (flushSuit != -1 && straightHigh > 0) return 9000000LL + straightHigh;
    if (four > 0) return 8000000LL + (four * 10000);
    if (triple > 0 && !pairs.empty()) return 7000000LL + (triple * 10000) + (pairs[0] * 100);
    if (flushSuit != -1) return 6000000LL + (suitCards[flushSuit][0] * 10000);
    if (straightHigh > 0) return 5000000LL + (straightHigh * 10000);
    if (triple > 0) return 4000000LL + (triple * 10000) + (singles.empty() ? 0 : singles[0] * 100);
    if (pairs.size() >= 2) return 3000000LL + (pairs[0] * 10000) + (pairs[1] * 100);
    if (!pairs.empty()) return 2000000LL + (pairs[0] * 10000) + (singles.empty() ? 0 : singles[0] * 100);
    return 1000000LL + (singles.empty() ? 0 : singles[0] * 10000);
}

void TexasHoldem::Play() {
    system("cls");
    SetColor(11);
    cout << "\n\n          [ 텍사스 홀덤 입장 ]" << endl;
    cout << "          1.초보(1k) 2.일반(5k) 3.고수(10k) 4.메이저(20k)" << endl;
    cout << "\n            선택: ";
    int menu, baseBet; cin >> menu;
    if (menu == 1) baseBet = 1000;
    else if (menu == 3) baseBet = 10000;
    else if (menu == 4) baseBet = 20000;
    else baseBet = 5000;

    vector<THPlayer> players;
    players.push_back({ "나 (플레이어)", {}, playerMoney, false, false, 0 });

    for (int i = 0; i < 3; i++) {
        players.push_back({ globalNamePool[rand() % globalNamePool.size()], {}, 100000, false, false, 0 });
    }

    while (true) {
        if (CheckBankruptcy(players[0].name, playerMoney, baseBet, true)) return;

        initDeck(); shuffleDeck(); communityCards.clear();
        int pot = 0; int dIdx = 0;

        for (auto& p : players) {
            p.hand.clear(); p.isFolded = false; p.isAllIn = false; p.totalBetThisRound = 0;
            p.hand.push_back(deck[dIdx++]); p.hand.push_back(deck[dIdx++]);
        }

        const char* stageNames[] = { "프리플랍", "플랍", "턴", "리버" };
        for (int sIdx = 0; sIdx < 4; sIdx++) {
            if (sIdx == 1) for (int i = 0; i < 3; i++) communityCards.push_back(deck[dIdx++]);
            else if (sIdx > 1) communityCards.push_back(deck[dIdx++]);

            for (auto& p : players) p.totalBetThisRound = 0;
            int currentCall = (sIdx == 0 ? baseBet : 0);

            // 앤티 자동 지불
            if (sIdx == 0) {
                for (int i = 0; i < (int)players.size(); i++) {
                    int& curM = (i == 0 ? playerMoney : players[i].money);
                    int ante = min(curM, baseBet);
                    curM -= ante; pot += ante;
                    players[i].totalBetThisRound = ante;
                    if (curM <= 0) players[i].isAllIn = true;
                }
            }

            vector<bool> hasActed(players.size(), false);
            int currentTurn = 0;

            while (true) {
                bool allMatched = true;
                int activeCount = 0; int playingCount = 0;

                for (int i = 0; i < (int)players.size(); i++) {
                    if (players[i].isFolded) continue;
                    playingCount++;
                    if (!players[i].isAllIn) {
                        activeCount++;
                        if (!hasActed[i] || players[i].totalBetThisRound < currentCall) allMatched = false;
                    }
                }

                if (allMatched || playingCount <= 1 || activeCount == 0) break;

                int i = currentTurn;
                if (players[i].isFolded || players[i].isAllIn) {
                    currentTurn = (currentTurn + 1) % players.size();
                    continue;
                }

                system("cls");
                SetColor(14); cout << "\n==================== [ " << stageNames[sIdx] << " ] ====================" << endl;
                cout << " 현재 판돈(Pot): " << pot << " | 콜 금액: " << currentCall << endl;
                cout << " 공용 카드: "; for (auto& c : communityCards) PrintCard(c.suit, c.rank);
                cout << "\n----------------------------------------------------------" << endl;
                for (int j = 0; j < (int)players.size(); j++) {
                    SetColor(j == 0 ? 11 : 15);
                    cout << (j == i ? "▶ " : "  ") << left << setw(15) << players[j].name;
                    if (players[j].isFolded) cout << "[F] 다이";
                    else if (players[j].isAllIn) cout << "[A] 올인";
                    else cout << "잔액: " << (j == 0 ? playerMoney : players[j].money);
                    cout << " (베팅: " << players[j].totalBetThisRound << ")" << endl;
                }
                SetColor(10); cout << "\n [내 핸드]: "; for (auto& c : players[0].hand) PrintCard(c.suit, c.rank);
                cout << "(" << GetDetailedHandName(evaluateHand(players[0].hand, communityCards)) << ")" << endl;

                int amountNeeded = currentCall - players[i].totalBetThisRound;

                if (i == 0) { // [플레이어]
                    while (_kbhit()) _getch();
                    SetColor(11); cout << "\n >> 내 차례 (필요액: " << (amountNeeded > 0 ? amountNeeded : 0) << ")" << endl;
                    cout << " [1]콜 [2]레이즈 [3]올인 [4]다이 : ";
                    int choice; cin >> choice;

                    if (choice == 1) {
                        int bet = min(playerMoney, amountNeeded);
                        playerMoney -= bet; pot += bet; players[i].totalBetThisRound += bet;
                        if (playerMoney <= 0) players[i].isAllIn = true;
                    }
                    else if (choice == 2) {
                        int rAmount; cout << " 추가 레이즈액: "; cin >> rAmount;
                        int maxOp = 0;
                        for (int j = 1; j < (int)players.size(); j++) {
                            if (!players[j].isFolded) maxOp = max(maxOp, players[j].money + players[j].totalBetThisRound);
                        }
                        int needed = (amountNeeded > 0 ? amountNeeded : 0) + rAmount;
                        if (players[i].totalBetThisRound + needed > maxOp) needed = maxOp - players[i].totalBetThisRound;

                        int finalBet = min(playerMoney, max(0, needed));
                        playerMoney -= finalBet; pot += finalBet; players[i].totalBetThisRound += finalBet;
                        if (playerMoney <= 0 || players[i].totalBetThisRound >= maxOp) players[i].isAllIn = true;

                        if (players[i].totalBetThisRound > currentCall) {
                            currentCall = players[i].totalBetThisRound;
                            for (int k = 0; k < players.size(); k++) if (!players[k].isFolded) hasActed[k] = false;
                        }
                    }
                    else if (choice == 3) { // [전략적 올인]
                        int maxOp = 0;
                        for (int j = 1; j < (int)players.size(); j++) {
                            if (!players[j].isFolded) maxOp = max(maxOp, players[j].money + players[j].totalBetThisRound);
                        }
                        int needed = maxOp - players[i].totalBetThisRound;
                        int finalBet = min(playerMoney, max(amountNeeded, needed));
                        playerMoney -= finalBet; pot += finalBet; players[i].totalBetThisRound += finalBet;
                        players[i].isAllIn = true;

                        if (players[i].totalBetThisRound > currentCall) {
                            currentCall = players[i].totalBetThisRound;
                            for (int k = 0; k < players.size(); k++) if (!players[k].isFolded) hasActed[k] = false;
                        }
                    }
                    else players[i].isFolded = true;
                    hasActed[i] = true;
                }
                else { // [AI]
                    long long aiS = evaluateHand(players[i].hand, communityCards);
                    int bluff = rand() % 100;
                    if (amountNeeded > 0) {
                        if (aiS >= 2000000LL || (aiS >= 1000000LL && bluff < 30)) {
                            int bet = min(players[i].money, amountNeeded);
                            players[i].money -= bet; pot += bet; players[i].totalBetThisRound += bet;
                            if (players[i].money <= 0) players[i].isAllIn = true;
                            PrintActionLog(players[i].name, (players[i].isAllIn ? "올인 콜!" : "콜(Call)"), 10);
                        }
                        else {
                            players[i].isFolded = true; PrintActionLog(players[i].name, "다이(Fold)", 8);
                        }
                    }
                    else {
                        PrintActionLog(players[i].name, "체크(Check)", 10);
                    }
                    hasActed[i] = true;
                    Sleep(800);
                }
                currentTurn = (currentTurn + 1) % players.size();
            }
            if (count_if(players.begin(), players.end(), [](const THPlayer& p) {return !p.isFolded; }) <= 1) break;
        }

        // 쇼다운
        system("cls");
        SetColor(14); cout << "==================== [ 최종 결과 ] ====================" << endl;
        cout << " [ 공 유 카 드 ] : "; for (auto& c : communityCards) PrintCard(c.suit, c.rank);
        cout << "\n--------------------------------------------------------------" << endl;
        int winIdx = -1; long long bestS = -1;
        for (int i = 0; i < (int)players.size(); i++) {
            if (players[i].isFolded) { SetColor(8); cout << " [" << left << setw(15) << players[i].name << "]: (다이)" << endl; continue; }
            long long s = evaluateHand(players[i].hand, communityCards);
            SetColor(i == 0 ? 11 : 15); cout << " [" << left << setw(15) << players[i].name << "]: ";
            for (auto& c : players[i].hand) PrintCard(c.suit, c.rank);
            SetColor(13); cout << " -> " << GetDetailedHandName(s) << endl;
            if (s > bestS) { bestS = s; winIdx = i; }
        }
        if (winIdx != -1) {
            SetColor(10); cout << "\n ★ 승자: " << players[winIdx].name << " [$" << pot << " 획득!]" << endl;
            if (winIdx == 0) playerMoney += pot; else players[winIdx].money += pot;
        }

        SetColor(15); cout << "\n [!] Enter: 다음 판 / 0: 종료";
        int next = _getch();
        if (next == '0') break;

        for (int i = 1; i < (int)players.size(); i++) {
            if (players[i].money <= 0) {
                players[i].name = globalNamePool[rand() % globalNamePool.size()];
                players[i].money = 100000;
            }
        }
    }
}