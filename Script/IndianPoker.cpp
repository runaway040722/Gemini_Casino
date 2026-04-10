#include "IndianPoker.h"
#include "System.h" // 공통 시스템 헤더 추가
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

IndianPoker::IndianPoker(int& money) : playerRef(money), pot(0), currentDeckIdx(0) {
    players.clear();
    players.push_back({ "나 (Player)", {0, 0}, playerRef, false });

    random_device rd;
    mt19937 g(rd());
    vector<string> tempPool = globalNamePool;
    shuffle(tempPool.begin(), tempPool.end(), g);

    for (int i = 0; i < 2; i++) {
        players.push_back({ tempPool[i], {0, 0}, 100000, false });
    }

    InitDeck();
}

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

// AI가 파산했을 때 교체해주는 로직 추가
void IndianPoker::CheckAndReplaceAI() {
    for (int i = 1; i < (int)players.size(); i++) {
        if (players[i].money <= 0) {
            // AI 광산 연출
            DragToMine(players[i].name, false);

            // 새 NPC 입장
            string newName = globalNamePool[rand() % globalNamePool.size()];
            players[i].name = newName;
            players[i].money = 100000;

            SetColor(11);
            cout << "\n [★] 빈자리에 새로운 타짜 [" << newName << "]가 입장했습니다." << endl;
            Sleep(1000);
        }
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

void IndianPoker::ShowTable(bool revealPlayerCard) {
    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 3인 인디언 포커 : 1장 승부 ]                " << endl;
    cout << "          현재 누적 판돈(POT): " << pot << "$" << endl;
    cout << "==========================================================" << endl;
    SetColor(15);

    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) {
            SetColor(8);
            cout << left << setw(20) << players[i].name << " : [ FOLDED ]" << endl;
        }
        else {
            SetColor(i == 0 ? 11 : 15);
            cout << left << setw(20) << players[i].name << " : ";
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
    cout << " [ 남은 카드: " << (int)deck.size() - currentDeckIdx << "장 ]" << endl;
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

void IndianPoker::Play() {
    // 진입 시 파산 체크
    if (CheckBankruptcy("당신", playerRef, 1000, true)) return;

    while (true) {
        // AI 교체 및 카드 소진 체크
        CheckAndReplaceAI();

        if (currentDeckIdx + 3 > (int)deck.size()) {
            SetColor(11);
            cout << "\n [!] 카드를 모두 소진했습니다! 덱을 새로 섞습니다." << endl;
            InitDeck();
            Sleep(1500);
        }

        int currentBet = 0;
        int baseBet = 1000;
        pot = 0;

        // 앤티(기본금) 납부 및 카드 배분
        for (int i = 0; i < (int)players.size(); i++) {
            int& m = (i == 0) ? playerRef : players[i].money;
            m -= baseBet;
            pot += baseBet;
            players[i].isFolded = false;
            players[i].card = deck[currentDeckIdx++];
        }

        ShowTable(false);

        bool roundActive = true;
        while (roundActive) {
            for (int i = 0; i < (int)players.size(); i++) {
                if (players[i].isFolded) continue;

                if (i == 0) { // 플레이어 턴
                    cout << "\n [1]콜 [2]레이즈 [3]올인 [4]다이 : ";
                    char choice = (char)_getch();
                    if (choice == '2') {
                        cout << "\n 레이즈 금액: ";
                        int rAmt; cin >> rAmt;
                        if (rAmt > playerRef) rAmt = playerRef;
                        playerRef -= rAmt; pot += rAmt; currentBet += rAmt;
                    }
                    else if (choice == '3') {
                        currentBet += playerRef; pot += playerRef; playerRef = 0;
                    }
                    else if (choice == '4') players[0].isFolded = true;
                    else {
                        int callAmt = (playerRef < currentBet) ? playerRef : currentBet;
                        playerRef -= callAmt; pot += callAmt;
                    }
                }
                else { // AI 턴
                    cout << "\n " << players[i].name << " 생각 중..."; Sleep(400);
                    int maxVisible = 0;
                    for (int j = 0; j < 3; j++) {
                        if (i == j || players[j].isFolded) continue;
                        if (players[j].card.rank > maxVisible) maxVisible = players[j].card.rank;
                    }
                    int prob = rand() % 100;
                    if (maxVisible <= 4 && prob < 30) {
                        int r = 5000; if (r > players[i].money) r = players[i].money;
                        players[i].money -= r; pot += r; currentBet += r;
                        cout << " [ 레이즈! ]";
                    }
                    else if (maxVisible >= 9 && prob < 50) {
                        players[i].isFolded = true; cout << " [ 다이 ]";
                    }
                    else {
                        int callAmt = (players[i].money < currentBet) ? players[i].money : currentBet;
                        players[i].money -= callAmt; pot += callAmt;
                        cout << (currentBet == 0 ? " [ 체크 ]" : " [ 콜 ]");
                    }
                }

                int activeCount = 0;
                for (auto& p : players) if (!p.isFolded) activeCount++;
                if (activeCount <= 1) { roundActive = false; break; }
            }
            roundActive = false; // 1바퀴 돌면 종료 (단순화된 규칙)
        }

        ShowTable(true);
        int winner = EvaluateWinner();
        if (winner != -1) {
            SetColor(10);
            cout << "\n ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
            cout << "  승자: " << players[winner].name << " (+$" << pot << ")" << endl;
            cout << " ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
            if (winner == 0) playerRef += pot;
            else players[winner].money += pot;
        }

        // 판 종료 후 플레이어 파산 체크
        if (CheckBankruptcy("당신", playerRef, 1, true)) return;

        SetColor(15);
        cout << "\n [1] 계속하기 [0] 나가기 (Enter 입력 시 계속) : ";
        int key = _getch();
        if (key == '0') break;
    }
}