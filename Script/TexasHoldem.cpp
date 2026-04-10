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

// 외부 정의 변수 및 함수 (Main.cpp 등에서 선언됨)
extern vector<string> globalNamePool;
extern void SetColor(int color);

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

void DragToMine(string name, bool isPlayer) {
    system("cls");
    SetColor(12);

    if (isPlayer) {
        string pLines[] = {
            "\"아악! 한 번만 더 기회를!! 이번엔 진짜 딸 수 있다고!!\"",
            "\"이거 놔! 내 손목을 걸게! 한 판만 더 하게 해줘!!\"",
            "\"제발... 집에 가족들이 기다리고 있어요... 살려주세요!\"",
            "\"사기야! 이건 조작이야! 너희들 다 한통속이지!!\"",
            "\"으아아아아아악! 싫어! 지하로 가기 싫어!! 안 돼!!!\""
        };
        cout << "\n\n [!] 검은 양복 : \"광산에 신입이 필요한데, 자네가 딱이군.\"" << endl; Sleep(1000);
        cout << " [!] 당신 : " << pLines[rand() % 5] << endl; Sleep(1500);
        SetColor(8);
        cout << " [!] (검은 양복들이 당신의 뒷덜미를 낚아챕니다.)" << endl; Sleep(1000);
        cout << " [!] (질질 끌려가는 소리... 깡... 깡...)" << endl; Sleep(2000);
        cout << " [!] (철창문이 닫히는 소리... 쾅!)" << endl; Sleep(1500);
        SetColor(12);
        cout << "\n [!] 당신은 이제 빛이 들지 않는 막장의 '번호'가 되었습니다." << endl;
        cout << " [!] [GAME OVER - 평생 채굴형]" << endl; Sleep(3000);
    }
    else {
        string aiLines[] = {
            "\"으아아악! 안 돼! 난 못 가!!\"", "\"살려줘요! 저놈들이 날 죽이려 해!\"",
            "\"빌어먹을... 마지막에 하이 카드만 아니었어도!!\"", "\"내... 내 돈... 전 재산이...\"",
            "\"차라리 여기서 죽여라! 광산은 싫어!!\"", "\"너! 너 때문이야! 너만 아니었으면!!\"",
            "\"살려주세요... 시키는 건 뭐든 할게요...\"", "\"이건 꿈이야... 깨어나야 해!!\"",
            "\"내가 누군 줄 알아? 나 이 근처 유지라고!!\"", "\"돈은 더 있어요... 집에... 숨겨둔 금고가...\"",
            "\"옆 테이블이랑 짠 거지? 다 알고 있어!!\"", "\"내 계산은 완벽했어... 확률이 이럴 리 없어!\"",
            "\"제발... 칩 한 개만 빌려줘... 복구할 수 있어...\"", "\"아니야... 이건 무효야! 다시 해!!\"",
            "\"으아아악! 내 팔! 팔 부러지겠어!!\"", "\"나가면 너부터 죽여버릴 거야!!\"",
            "\"히히... 카드... 내 카드 어디 갔어?\"", "\"도박은 질병이라더니... 내가 미쳤지...\"",
            "\"마누라 미안해... 애들 학원비였는데...\"", "\"전부 다 가져가라! 하지만 목숨만은!!\"",
            "\"이 사기꾼 놈들! 카지노 통째로 불 질러 버릴 거야!\"", "\"악! 내 머리! 머리채 잡지 마!!\"",
            "\"어이 딜러! 아까 나랑 눈 마주쳤잖아! 도와달라고!!\"", "\"아직 100골드 남았어! 주머니에 있다고!!\"",
            "\"이거 놔! 내 발로 걸어갈 테니까!!\"", "\"으으... 내 장기... 장기만은 안 돼...\"",
            "\"내가 다시 돌아오면 너희들 다 끝장이야!!\"", "\"하늘이 무심하시지... 어떻게 나한테!!\"",
            "\"엄마... 엄마 보고 싶어... 으허헝...\"", "\"저 플레이어 녀석 밑장 뺐어! 내가 봤어!!\"",
            "\"한 판만... 딱 한 판만 더 하면 왕창 딸 수 있었는데!!\"", "\"이 지옥 같은 곳에서 나갈 거야!!\"",
            "\"숨이... 숨이 안 쉬어져... 살려줘!!\"", "\"당신들 이거 인권 침해야! 고소할 거야!!\"",
            "\"(바닥에 침을 뱉으며) 퉤! 더러운 놈들!!\"", "\"난 죄가 없어! 도박이 죄지!!\"",
            "\"내 인생 돌려내!! 이 개새끼들아!!\"", "\"으아아... 누가... 제발...\"",
            "\"어머니 수술비였단 말이야... 제발...\"", "\"저놈이 내 운을 다 뺏어갔어!!\"",
            "\"광산? 거기 가면 살아서 못 나온다며!!\"", "\"싫어! 어두운 곳은 무섭단 말이야!!\"",
            "\"(거품을 물며) 으버버... 으으...\"", "\"날 어디로 데려가는 거야!! 대답해!!\"",
            "\"판돈... 내 판돈... 돌려줘...\"", "\"너희들 천벌 받을 거야!! 반드시!!\"",
            "\"(허탈하게 웃으며) 하하하... 그래... 다 가져가라...\"", "\"내 목숨은 곡괭이보다 가벼운가 보군...\"",
            "\"아니야... 난 아직 패배하지 않았어!!\"", "\"으아아아아아아아아악!!! (비명)\""
        };
        SetColor(11);
        cout << "\n [!] " << name << " 파산! 검은 양복들이 그림자처럼 다가옵니다." << endl; Sleep(800);
        SetColor(14);
        cout << " [!] " << name << " : " << aiLines[rand() % 50] << endl; Sleep(1200);
        SetColor(8);
        cout << " [!] (멀어지는 비명소리...) \"으아아아아악! 살려줘!!\"" << endl;
        cout << " [!] (질질 끌려가는 소리가 복도 끝에서 사라집니다.)" << endl; Sleep(1500);
        SetColor(15);
        cout << " [!] 잠시 정적이 흐른 뒤, 새로운 게임이 준비됩니다." << endl; Sleep(1000);
    }
}

// --- [TexasHoldem 클래스 구현] ---

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
    cout << "\n           선택: ";
    int menu, baseBet; cin >> menu;
    if (menu == 1) baseBet = 1000;
    else if (menu == 3) baseBet = 10000;
    else if (menu == 4) baseBet = 20000;
    else baseBet = 5000;

    vector<THPlayer> players;
    players.push_back({ "나 (플레이어)", {}, playerMoney, false, false, 0 });
    random_device rd; mt19937 g(rd());
    vector<string> pool = globalNamePool;
    shuffle(pool.begin(), pool.end(), g);
    for (int i = 0; i < 3; i++) players.push_back({ pool[i], {}, 100000, false, false, 0 });

    while (true) {
        if (playerMoney < baseBet) { DragToMine("플레이어", true); return; }

        initDeck(); shuffleDeck(); communityCards.clear();
        int pot = 0; int dIdx = 0;
        for (auto& p : players) {
            p.hand.clear(); p.isFolded = false; p.isAllIn = false; p.totalBetThisRound = 0;
            p.hand.push_back(deck[dIdx++]); p.hand.push_back(deck[dIdx++]);
        }

        const char* stage[] = { "프리플랍", "플랍", "턴", "리버" };
        for (int sIdx = 0; sIdx < 4; sIdx++) {
            if (sIdx == 1) for (int i = 0; i < 3; i++) communityCards.push_back(deck[dIdx++]);
            else if (sIdx > 1) communityCards.push_back(deck[dIdx++]);

            for (auto& p : players) p.totalBetThisRound = 0;
            int currentCall = (sIdx == 0 ? baseBet : 0);

            if (sIdx == 0) {
                for (auto& p : players) {
                    int ante = min(p.money, baseBet);
                    p.money -= ante; pot += ante; p.totalBetThisRound = ante;
                    if (p.money <= 0) p.isAllIn = true;
                }
            }

            bool bettingFinished = false;
            vector<bool> hasActed(players.size(), false);

            while (!bettingFinished) {
                bettingFinished = true;
                system("cls");
                SetColor(14); cout << "\n==================== [ " << stage[sIdx] << " ] ====================" << endl;
                cout << " 현재 판돈(Pot): " << pot << " | 콜 금액: " << currentCall << endl;
                cout << " 공용 카드: "; for (auto& c : communityCards) PrintCard(c.suit, c.rank);
                cout << "\n----------------------------------------------------------" << endl;
                for (int j = 0; j < (int)players.size(); j++) {
                    SetColor(j == 0 ? 11 : 15);
                    cout << (j == 0 ? "> " : "  ") << left << setw(15) << players[j].name;
                    if (players[j].isFolded) cout << "[F] 다이";
                    else if (players[j].isAllIn) cout << "[A] 올인";
                    else cout << "잔액: " << (j == 0 ? playerMoney : players[j].money);
                    cout << " (베팅: " << players[j].totalBetThisRound << ")" << endl;
                }
                SetColor(10); cout << "\n [내 핸드]: "; for (auto& c : players[0].hand) PrintCard(c.suit, c.rank);
                cout << "(" << GetDetailedHandName(evaluateHand(players[0].hand, communityCards)) << ")" << endl;
                SetColor(14); cout << "----------------------------------------------------------\n" << endl;

                for (int i = 0; i < (int)players.size(); i++) {
                    if (players[i].isFolded || players[i].isAllIn) continue;

                    int amountNeeded = currentCall - players[i].totalBetThisRound;
                    if (amountNeeded <= 0 && hasActed[i]) continue;

                    if (i == 0) { // [플레이어 차례]
                        SetColor(11); cout << " >> 내 차례 (콜 금액: " << (amountNeeded > 0 ? amountNeeded : 0) << ")" << endl;
                        cout << " [1]콜 [2]레이즈 [3]올인 [4]다이 : ";
                        int choice; cin >> choice;

                        if (choice == 1) {
                            int bet = min(playerMoney, amountNeeded);
                            playerMoney -= bet; pot += bet; players[i].totalBetThisRound += bet;
                            if (playerMoney <= 0) players[i].isAllIn = true;
                            cout << " >> [결정] 당신: " << (amountNeeded == 0 ? "체크" : "콜") << endl;
                        }
                        else if (choice == 2) {
                            int rAmount; cout << " 추가 레이즈액: "; cin >> rAmount;
                            int total = (amountNeeded > 0 ? amountNeeded : 0) + rAmount;
                            if (total >= playerMoney) { total = playerMoney; players[i].isAllIn = true; }
                            playerMoney -= total; pot += total; players[i].totalBetThisRound += total;
                            currentCall = players[i].totalBetThisRound;
                            fill(hasActed.begin(), hasActed.end(), false);
                            bettingFinished = false;
                            cout << " >> [결정] 당신: 레이즈! (현재 총 베팅: " << currentCall << ")" << endl;
                        }
                        else if (choice == 3) {
                            int maxOpponentMoney = 0;
                            for (int k = 1; k < (int)players.size(); k++) if (!players[k].isFolded) maxOpponentMoney = max(maxOpponentMoney, players[k].money);
                            int safeAllIn = min(playerMoney, maxOpponentMoney + amountNeeded + baseBet * 5);
                            playerMoney -= safeAllIn; pot += safeAllIn;
                            players[i].totalBetThisRound += safeAllIn;
                            players[i].isAllIn = true;
                            if (players[i].totalBetThisRound > currentCall) {
                                currentCall = players[i].totalBetThisRound;
                                fill(hasActed.begin(), hasActed.end(), false);
                                bettingFinished = false;
                            }
                            cout << " >> [결정] 당신: 전 재산 올인!!" << endl;
                        }
                        else {
                            players[i].isFolded = true;
                            cout << " >> [결정] 당신: 다이(Fold)" << endl;
                        }
                    }
                    else { // [AI 차례 - 로그가 쌓이도록 수정됨]
                        SetColor(15);
                        cout << " [!] " << players[i].name << "가 카드를 보며 고민 중..."; Sleep(600);

                        long long aiS = evaluateHand(players[i].hand, communityCards);

                        // 다이 결정
                        if (amountNeeded > 0 && aiS < 2000000LL && (rand() % 100 < 30)) {
                            players[i].isFolded = true;
                            cout << " -> [다이]" << endl; // 같은 줄에 결과 출력
                        }
                        // 레이즈/올인 결정
                        else if (aiS >= 5000000LL && (rand() % 100 < 10)) {
                            int raise = baseBet * 2;
                            int total = (amountNeeded > 0 ? amountNeeded : 0) + raise;
                            if (players[i].money <= total) {
                                int all = players[i].money; players[i].money = 0; pot += all;
                                players[i].totalBetThisRound += all; players[i].isAllIn = true;
                                if (players[i].totalBetThisRound > currentCall) {
                                    currentCall = players[i].totalBetThisRound;
                                    fill(hasActed.begin(), hasActed.end(), false); bettingFinished = false;
                                }
                                cout << " -> [올인!!]" << endl;
                            }
                            else {
                                players[i].money -= total; pot += total; players[i].totalBetThisRound += total;
                                currentCall = players[i].totalBetThisRound;
                                fill(hasActed.begin(), hasActed.end(), false); bettingFinished = false;
                                cout << " -> [레이즈! (" << currentCall << ")]" << endl;
                            }
                        }
                        // 콜/체크 결정
                        else {
                            int bet = min(players[i].money, (amountNeeded > 0 ? amountNeeded : 0));
                            players[i].money -= bet; pot += bet; players[i].totalBetThisRound += bet;
                            if (players[i].money <= 0) players[i].isAllIn = true;

                            if (amountNeeded <= 0) cout << " -> [체크]" << endl;
                            else cout << " -> [콜]" << endl;
                        }
                        Sleep(400); // 다음 AI 행동 전 약간의 대기
                    }
                    hasActed[i] = true;
                }
            }
            if (count_if(players.begin(), players.end(), [](const THPlayer& p) {return !p.isFolded; }) <= 1) break;
        }

        // --- 최종 결과 쇼다운 ---
        system("cls"); SetColor(14);
        cout << "==================== [ 최종 쇼다운 결과 ] ====================" << endl;
        int winIdx = -1; long long bestS = -1;
        for (int i = 0; i < (int)players.size(); i++) {
            if (players[i].isFolded) { SetColor(8); cout << " [" << players[i].name << "]: (다이)" << endl; continue; }
            long long s = evaluateHand(players[i].hand, communityCards);
            SetColor(i == 0 ? 11 : 15);
            cout << " [" << players[i].name << "]: "; for (auto& c : players[i].hand) PrintCard(c.suit, c.rank);
            cout << " -> " << GetDetailedHandName(s) << endl;
            if (s > bestS) { bestS = s; winIdx = i; }
        }
        if (winIdx != -1) {
            SetColor(10); cout << "\n ★ 승자: " << players[winIdx].name << " [" << pot << " 획득!]" << endl;
            if (winIdx == 0) playerMoney += pot; else players[winIdx].money += pot;
        }
        SetColor(14); cout << "==============================================================" << endl;
        SetColor(15); cout << " [!] 결과를 확인하셨으면 아무 키나 누르세요..."; _getch();

        // --- 파산 체크 및 대체 ---
        for (int i = 0; i < (int)players.size(); i++) {
            if (i == 0 && playerMoney < baseBet) { DragToMine("당신", true); return; }
            else if (i > 0 && players[i].money < baseBet) {
                DragToMine(players[i].name, false);
                players[i].name = globalNamePool[rand() % globalNamePool.size()];
                players[i].money = 100000;
                players[i].isFolded = false; players[i].isAllIn = false;
                SetColor(11); cout << " [★] 새로운 참가자 [" << players[i].name << "] 입장!" << endl; Sleep(1000);
            }
        }
        SetColor(15); cout << "\n [1]계속 [0]종료 : "; int c; cin >> c; if (c == 0) break;
    }
}