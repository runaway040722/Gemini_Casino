#include "IndianPoker.h"
#include "System.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <windows.h>
#include <conio.h>
#include <iomanip>

using namespace std;

extern vector<string> globalNamePool;

// --- [생성자] ---
IndianPoker::IndianPoker(int& money) : playerRef(money), pot(0), currentDeckIdx(0) {
    players.clear();
    // { 이름, 카드, 잔액, 다이여부, 이번판총베팅 }
    players.push_back({ "나 (Player)", {0, 0}, playerRef, false, 0 });

    random_device rd;
    mt19937 g(rd());
    vector<string> tempPool = globalNamePool;
    shuffle(tempPool.begin(), tempPool.end(), g);

    for (int i = 0; i < 2; i++) {
        players.push_back({ tempPool[i], {0, 0}, 100000, false, 0 });
    }
    InitDeck();
}

// --- [덱 초기화] ---
void IndianPoker::InitDeck() {
    deck.clear();
    for (int r = 2; r <= 10; r++) {
        for (int s = 0; s < 4; s++) {
            deck.push_back({ s, r });
        }
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
    currentDeckIdx = 0;
}

// --- [AI 파산 시 교체] ---
void IndianPoker::CheckAndReplaceAI() {
    bool replaced = false;
    for (int i = 1; i < (int)players.size(); i++) {
        if (players[i].money <= 0) {
            DragToMine(players[i].name, false);
            string newName = globalNamePool[rand() % globalNamePool.size()];
            players[i].name = newName;
            players[i].money = 100000;
            SetColor(11);
            cout << "\n [★] 빈자리에 새로운 참가자 [" << newName << "]가 입장했습니다." << endl;
            replaced = true;
        }
    }
    if (replaced) {
        SetColor(15);
        cout << " [!] 아무 키나 누르면 게임을 재개합니다..." << endl;
        while (_kbhit()) _getch();
        _getch();
    }
}

string IndianPoker::GetCardStr(IndianCard c) {
    string s_mark;
    if (c.suit == 0) s_mark = "♠";
    else if (c.suit == 1) s_mark = "◆";
    else if (c.suit == 2) s_mark = "♥";
    else s_mark = "♣";
    return s_mark + to_string(c.rank);
}

// --- [테이블 출력] ---
void IndianPoker::ShowTable(bool revealPlayerCard) {
    system("cls"); // 호출될 때마다 화면을 깨끗이 지움
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 3인 인디언 포커 : 진검 승부 ]                " << endl;
    cout << "          현재 누적 판돈(POT): " << pot << "$" << endl;
    cout << "==========================================================" << endl;

    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded && !revealPlayerCard) {
            SetColor(8);
            cout << left << setw(20) << players[i].name << " : [ FOLDED ]" << endl;
        }
        else {
            SetColor(i == 0 ? 11 : 15);
            string displayName = players[i].name + (players[i].isFolded ? " (FOLD)" : "");
            cout << (i == 0 ? "▶ " : "   ") << left << setw(17) << displayName << " : ";

            if (i != 0 || revealPlayerCard) {
                if (players[i].card.suit == 1 || players[i].card.suit == 2) SetColor(12);
                cout << GetCardStr(players[i].card);
                SetColor(i == 0 ? 11 : 15);
            }
            else cout << "?? (이마에 붙음)";
            cout << "  (잔고: " << (i == 0 ? playerRef : players[i].money) << "$)" << endl;
        }
    }
    SetColor(15);
    cout << "----------------------------------------------------------" << endl;
    cout << " [ 덱 상황 ] 남은 카드: " << (int)deck.size() - currentDeckIdx << "장" << endl;
}

int IndianPoker::EvaluateWinner() {
    int winnerIdx = -1;
    int maxRank = -1;
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) continue;
        if (players[i].card.rank > maxRank) {
            maxRank = players[i].card.rank;
            winnerIdx = i;
        }
    }
    return winnerIdx;
}

// --- [메인 게임 루프] ---
void IndianPoker::Play() {
    if (CheckBankruptcy("당신", playerRef, 1000, true)) return;

    bool exitGame = false;
    while (!exitGame) {
        system("cls");
        SetColor(11);
        cout << "\n\n\n\n          ▶ 카드를 수거하고 새로운 덱을 셔플합니다..." << endl;
        cout << "          ";
        for (int i = 0; i < 20; i++) { cout << "■"; Sleep(30); }
        InitDeck();

        while (currentDeckIdx + 3 <= (int)deck.size()) {
            CheckAndReplaceAI();
            int currentCall = 0, baseBet = 1000; pot = 0;

            // 앤티(기본 판돈) 지불
            for (int i = 0; i < (int)players.size(); i++) {
                int& m = (i == 0 ? playerRef : players[i].money);
                int ante = min(m, baseBet);
                m -= ante; pot += ante;
                players[i].isFolded = false;
                players[i].totalBet = ante;
                players[i].card = deck[currentDeckIdx++];
            }
            currentCall = baseBet;

            vector<bool> hasActed(players.size(), false);
            int currentTurn = 0;

            // [베팅 루프 시작]
            while (true) {
                int activeCount = 0;
                bool allMatched = true;

                for (int i = 0; i < (int)players.size(); i++) {
                    if (players[i].isFolded) continue;
                    activeCount++;

                    int m = (i == 0 ? playerRef : players[i].money);

                    // 핵심 수정: 행동을 안 했거나, 돈이 있는데 콜 금액보다 적게 냈으면 루프 유지
                    if (!hasActed[i]) {
                        allMatched = false;
                    }
                    else if (players[i].totalBet < currentCall && m > 0) {
                        allMatched = false;
                    }
                }

                // 기권자 제외 1명 남았거나 모두 배팅액 맞췄으면 종료
                if (allMatched || activeCount <= 1) break;

                int i = currentTurn;
                int& m = (i == 0 ? playerRef : players[i].money);

                // 기권했거나 돈이 없는(올인) 사람은 턴 자동 스킵
                if (players[i].isFolded || m <= 0) {
                    hasActed[i] = true;
                    currentTurn = (currentTurn + 1) % players.size();
                    continue;
                }

                ShowTable(false);
                int amountNeeded = currentCall - players[i].totalBet;

                if (i == 0) { // [ 플레이어 차례 ]
                    while (_kbhit()) _getch();
                    SetColor(11); cout << "\n >> 내 차례 (콜 필요액: " << amountNeeded << "$)" << endl;
                    cout << " [1]콜 [2]레이즈 [3]올인 [4]다이 : ";

                    char choice = (char)_getch();
                    if (choice == '1') {
                        int bet = min(playerRef, amountNeeded);
                        playerRef -= bet; pot += bet; players[i].totalBet += bet;
                    }
                    else if (choice == '2') {
                        cout << "\n 레이즈액(추가): "; int rAmt;
                        if (!(cin >> rAmt)) { cin.clear(); }
                        cin.ignore(1000, '\n');
                        int total = amountNeeded + (rAmt < 0 ? 0 : rAmt);
                        if (total > playerRef) total = playerRef;
                        playerRef -= total; pot += total; players[i].totalBet += total;
                        if (players[i].totalBet > currentCall) {
                            currentCall = players[i].totalBet;
                            for (int k = 0; k < (int)players.size(); k++) if (k != i) hasActed[k] = false;
                        }
                    }
                    else if (choice == '3') {
                        int finalAllIn = playerRef;
                        playerRef -= finalAllIn; pot += finalAllIn; players[i].totalBet += finalAllIn;
                        if (players[i].totalBet > currentCall) {
                            currentCall = players[i].totalBet;
                            for (int k = 0; k < (int)players.size(); k++) if (k != i) hasActed[k] = false;
                        }
                        SetColor(12); cout << "\n [!] 전재산 올인!!"; Sleep(800);
                    }
                    else if (choice == '4') players[i].isFolded = true;
                    hasActed[i] = true;
                }
                else { // [ AI 차례 ]
                    cout << "\n " << players[i].name << " 상황 분석 중..."; Sleep(600);
                    int maxOpponentCard = 0;
                    for (int j = 0; j < (int)players.size(); j++) {
                        if (i == j || players[j].isFolded) continue;
                        maxOpponentCard = max(maxOpponentCard, players[j].card.rank);
                    }

                    int prob = rand() % 100;
                    // AI 로직: 상대 카드 낮으면 레이즈, 높으면 다이 고려
                    if (maxOpponentCard <= 4) {
                        if (prob < 30 && players[i].money > 10000) {
                            int rAmt = 5000;
                            int total = min(players[i].money, amountNeeded + rAmt);
                            players[i].money -= total; pot += total; players[i].totalBet += total;
                            currentCall = players[i].totalBet;
                            for (int k = 0; k < (int)players.size(); k++) if (k != i) hasActed[k] = false;
                            SetColor(12); cout << " [ 레이즈!! ]"; SetColor(15);
                        }
                        else {
                            int bet = min(players[i].money, amountNeeded);
                            players[i].money -= bet; pot += bet; players[i].totalBet += bet;
                            cout << (amountNeeded <= 0 ? " [ 체크 ]" : " [ 콜 ]");
                        }
                    }
                    else if (maxOpponentCard >= 9 && prob < 70 && amountNeeded > 5000) {
                        players[i].isFolded = true; cout << " [ 다이 ]";
                    }
                    else {
                        int bet = min(players[i].money, amountNeeded);
                        players[i].money -= bet; pot += bet; players[i].totalBet += bet;
                        cout << (amountNeeded <= 0 ? " [ 체크 ]" : " [ 콜 ]");
                    }
                    hasActed[i] = true; Sleep(600);
                }
                currentTurn = (currentTurn + 1) % players.size();
            }

            // --- [결과 공개] ---
            ShowTable(true);

            // 10 카드 패널티 체크
            for (int i = 0; i < (int)players.size(); i++) {
                if (players[i].isFolded && players[i].card.rank == 10) {
                    SetColor(12); cout << "\n [!] 10 카드 기권 패널티! " << players[i].name << "가 10,000$를 지불합니다." << endl;
                    int& giver = (i == 0 ? playerRef : players[i].money);
                    int penalty = min(giver, 10000);
                    giver -= penalty;
                    // 패널티는 판돈에 추가하거나 상대에게 분배 (여기선 판돈 추가)
                    pot += penalty;
                    Sleep(1000);
                }
            }

            int winner = EvaluateWinner();
            if (winner != -1) {
                SetColor(10); cout << "\n ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
                cout << "  승자: " << players[winner].name << " (+$" << pot << ")" << endl;
                cout << " ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
                if (winner == 0) playerRef += pot; else players[winner].money += pot;
            }

            SetColor(15); cout << "\n ▶ [Enter] 다음 판으로..." << endl;
            while (_kbhit()) _getch();
            while (true) { if (_kbhit() && _getch() == 13) break; }
            if (CheckBankruptcy("당신", playerRef, 1, true)) return;
        }

        system("cls"); SetColor(14);
        cout << "\n 카드 소진! 새로운 덱을 셔플할까요? " << endl;
        cout << " [1]계속 [0]종료 : ";
        while (_kbhit()) _getch();
        if (_getch() == '0') exitGame = true;
    }
}