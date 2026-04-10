#include "TexasHoldem.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include <windows.h>
#include <conio.h>

using namespace std;

// 외부 전역 변수 및 함수 참조
extern vector<string> globalNamePool;
extern void SetColor(int color);

// --- 유틸리티 함수 ---

string GetRankName(int rank) {
    if (rank == 14 || rank == 1) return "A";
    if (rank == 11) return "J"; if (rank == 12) return "Q"; if (rank == 13) return "K";
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

// [광산 연출] 텍스트 하나씩 딜레이 출력
void DragToMine(string name, bool isPlayer) {
    system("cls");
    SetColor(12);

    string bossTalk[] = {
        "\"어이, 빈털터리. 여긴 구경하는 곳이 아니야.\"", "\"운이 없으면 몸이라도 튼튼해야지.\"",
        "\"저 친구 손바닥 좀 봐, 일하기 딱 좋아.\"", "\"어이 김씨! 여기 신참 하나 추가요!\"",
        "\"한 번 들어가면 최소 3년인 건 알지?\"", "\"빚은 몸으로 갚는 게 이 바닥 룰이다.\"",
        "\"걱정 마, 광산 안에도 도박판은 있으니까.\"", "\"그 곡괭이가 네 유일한 친구가 될 거다.\"",
        "\"입 닥치고 따라와. 말이 많으면 밥도 없어.\"", "\"인생 한 방 노리더니, 진짜 한 방에 가는구만.\""
    };

    string victimTalk[] = {
        "\"제발! 한 번만 더 기회를 주세요!\"", "\"내일이면 돈이 들어온단 말입니다!!\"",
        "\"이거 놔! 내 발로 갈 테니까!\"", "\"집에 아픈 자식이 있어요... 제발...\"",
        "\"그놈이 사기 친 거야! 난 억울하다고!\"", "\"안 돼... 다시 그 어둠 속으로...\"",
        "\"아아악! 손가락만은 살려줘!!\"", "\"너희들 내가 누군지 알아?\"",
        "\"누가... 누구 없어요? 사람 살려!!\"", "\"...차라리 잘 됐어. 좀 쉬고 싶다.\""
    };

    if (isPlayer) {
        cout << "\n [!] 화면이 어두워지며 붉은 조명이 비칩니다..." << endl; Sleep(1000);
        cout << " [!] 검은 양복 : " << bossTalk[rand() % 10] << endl; Sleep(1200);
        cout << " [!] 당신 : " << victimTalk[rand() % 10] << endl; Sleep(1200);
        SetColor(8);
        cout << "\n [!] (질질 끌려가는 소리... 스으윽...)" << endl; Sleep(1000);
        cout << " [!] 관리인 : \"3번 갱도로 던져버려.\"" << endl; Sleep(1200);
        cout << " [!] (텅-! 거대한 철문이 닫히는 소리)" << endl; Sleep(1500);
        for (int i = 0; i < 3; i++) { cout << " 깡... " << flush; Sleep(1000); }
        SetColor(12); cout << "\n\n [!] 당신은 광산의 노예가 되었습니다. GAME OVER." << endl; Sleep(2500);
    }
    else {
        cout << "\n [!] " << name << " 파산! 검은 양복들 등장." << endl; Sleep(800);
        cout << " [!] 검은 양복 : " << bossTalk[rand() % 10] << endl; Sleep(1000);
        cout << " [!] " << name << " : " << victimTalk[rand() % 10] << endl; Sleep(1200);
        SetColor(8); cout << " [!] (멀어지는 비명 소리...) \"으아아아아악!\"" << endl; Sleep(1500);
    }
}

// --- 멤버 함수 구현 ---

void TexasHoldem::initDeck() {
    deck.clear();
    for (int s = 0; s < 4; s++) for (int r = 1; r <= 13; r++) deck.push_back({ s, r });
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
    cout << "\n\n          +------------------------------------------+" << endl;
    cout << "          |          [ 텍사스 홀덤 입장 ]            |" << endl;
    cout << "          +------------------------------------------+" << endl;
    cout << "          |    1. [ 초보 ] 판돈 :  1,000             |" << endl;
    cout << "          |    2. [ 일반 ] 판돈 :  5,000             |" << endl;
    cout << "          |    3. [ 고수 ] 판돈 : 10,000             |" << endl;
    cout << "          |    4. [ 메이저] 판돈 : 20,000            |" << endl;
    cout << "          +------------------------------------------+" << endl;
    SetColor(15);
    cout << "\n            선택 (1~4): ";
    int menu, baseBet; cin >> menu;
    if (menu == 1) baseBet = 1000;
    else if (menu == 3) baseBet = 10000;
    else if (menu == 4) baseBet = 20000;
    else baseBet = 5000;

    if (playerMoney < baseBet) { DragToMine("플레이어", true); return; }

    vector<THPlayer> players;
    players.push_back({ "나 (플레이어)", {}, playerMoney, false, false });
    random_device rd; mt19937 g(rd());
    vector<string> pool = globalNamePool;
    shuffle(pool.begin(), pool.end(), g);
    // AI 기본 소지금 100,000으로 설정
    for (int i = 0; i < 3; i++) players.push_back({ pool[i], {}, 100000, false, false });

    while (true) {
        if (playerMoney < baseBet / 2) { DragToMine("플레이어", true); return; }

        for (int i = 1; i < (int)players.size(); i++) {
            if (players[i].money < baseBet / 2) {
                DragToMine(players[i].name, false);
                players[i].name = globalNamePool[rand() % globalNamePool.size()];
                players[i].money = 100000; // 교체되는 AI도 100,000 지참
                SetColor(11); cout << " [★] 새로운 참가자 [" << players[i].name << "] 입장." << endl; Sleep(1000);
            }
        }

        initDeck(); shuffleDeck(); communityCards.clear();
        int pot = 0; int dIdx = 0; int currentCall = baseBet; bool anyAllIn = false;

        int ante = baseBet / 2;
        for (int i = 0; i < 4; i++) {
            if (i == 0) playerMoney -= ante; else players[i].money -= ante;
            pot += ante;
        }

        for (auto& p : players) {
            p.hand.clear(); p.isFolded = false; p.isAllIn = false;
            p.hand.push_back(deck[dIdx++]); p.hand.push_back(deck[dIdx++]);
        }

        const char* stage[] = { "프리플랍", "플랍", "턴", "리버" };
        for (int sIdx = 0; sIdx < 4; sIdx++) {
            if (sIdx == 1) for (int i = 0; i < 3; i++) communityCards.push_back(deck[dIdx++]);
            else if (sIdx > 1) communityCards.push_back(deck[dIdx++]);

            system("cls");
            SetColor(14); cout << "==========================================================" << endl;
            cout << "              [ 텍사스 홀덤 - " << stage[sIdx] << " ]" << endl;
            cout << "   현재 총 판돈: " << pot << " | 콜 비용: " << currentCall << endl;
            cout << "   공용 카드: ";
            for (auto& c : communityCards) PrintCard(c.suit, c.rank);
            SetColor(14); cout << "\n==========================================================" << endl;

            for (int i = 0; i < (int)players.size(); i++) {
                if (players[i].isFolded) { SetColor(8); cout << left << setw(15) << players[i].name << ": [기권]" << endl; continue; }
                SetColor(i == 0 ? 11 : 15); cout << left << setw(15) << players[i].name << ": ";
                if (i == 0) {
                    for (auto& c : players[i].hand) PrintCard(c.suit, c.rank);
                    SetColor(10); cout << " [" << GetDetailedHandName(evaluateHand(players[i].hand, communityCards)) << "] ";
                }
                else cout << "?? ?? ";
                SetColor(i == 0 ? 11 : 15); cout << "(" << (i == 0 ? playerMoney : players[i].money) << ")" << endl;
            }

            if (!players[0].isFolded && !anyAllIn) {
                SetColor(15); cout << "\n [1]콜 [2]레이즈 [3]올인 [4]다이 : ";
                int choice; cin >> choice;
                if (choice == 1) { playerMoney -= currentCall; pot += currentCall; }
                else if (choice == 2) { currentCall *= 2; playerMoney -= currentCall; pot += currentCall; }
                else if (choice == 3) {
                    int maxOpp = 0; for (int k = 1; k < 4; k++) if (!players[k].isFolded) maxOpp = max(maxOpp, players[k].money);
                    int actualBet = min(playerMoney, maxOpp);
                    playerMoney -= actualBet; pot += actualBet; currentCall = actualBet; anyAllIn = true; players[0].isAllIn = true;
                }
                else { players[0].isFolded = true; break; }

                for (int j = 1; j < (int)players.size(); j++) {
                    if (players[j].isFolded) continue;
                    SetColor(14); cout << " [!] " << players[j].name << " 고민 중..."; Sleep(600);
                    long long aiS = evaluateHand(players[j].hand, communityCards);
                    bool aiCall = (anyAllIn ? (aiS >= 2000000LL || rand() % 100 < 15) : (choice == 2 ? (aiS >= 2000000LL || rand() % 100 < 35) : true));

                    if (aiCall) {
                        int b = min(players[j].money, currentCall);
                        players[j].money -= b; pot += b;
                        SetColor(11); cout << "\r [!] " << players[j].name << ": \"콜.\"                " << endl;
                    }
                    else {
                        players[j].isFolded = true;
                        SetColor(8); cout << "\r [!] " << players[j].name << ": \"다이.\"                " << endl;
                    }
                    Sleep(200);
                }
            }

            int alive = 0; for (auto& p : players) if (!p.isFolded) alive++;
            if (alive <= 1 || (players[0].isFolded && sIdx < 3)) break;

            // [자동 전환] 입력 대기 없이 0.5초 후 다음 라운드로
            if (sIdx < 3 && !anyAllIn) {
                SetColor(14); cout << "\n [ 잠시 후 다음 라운드가 시작됩니다... ]";
                Sleep(500);
            }
        }

        // --- 쇼다운 (결과 발표) ---
        system("cls"); SetColor(14);
        cout << "========================== 결과 (SHOWDOWN) ==========================" << endl;
        cout << "   공용 카드: ";
        for (auto& c : communityCards) PrintCard(c.suit, c.rank);
        cout << "\n---------------------------------------------------------------------" << endl;

        int winIdx = -1; long long bestS = -1;
        for (int i = 0; i < 4; i++) {
            if (players[i].isFolded) continue;
            long long s = evaluateHand(players[i].hand, communityCards);
            SetColor(i == 0 ? 11 : 15);
            cout << (i == 0 ? "> " : "  ") << left << setw(15) << players[i].name << ": ";
            for (auto& c : players[i].hand) PrintCard(c.suit, c.rank);
            SetColor(10); cout << " [" << GetDetailedHandName(s) << "]" << endl;
            if (s > bestS) { bestS = s; winIdx = i; }
        }
        if (winIdx != -1) {
            SetColor(10); cout << "\n ★ 승자: " << players[winIdx].name << " (" << pot << " 획득) ★" << endl;
            if (winIdx == 0) playerMoney += pot; else players[winIdx].money += pot;
        }
        SetColor(15); cout << "\n [1]한 판 더 [0]나가기 : "; int c; cin >> c; if (c == 0) break;
    }
}