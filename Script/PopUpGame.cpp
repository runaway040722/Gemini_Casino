#include "PopUpGame.h"
#include "System.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <windows.h>

using namespace std;

extern vector<string> globalNamePool;

PopUpGame::PopUpGame(int& playerMoney) : playerRef(playerMoney) {}

void PopUpGame::drawBarrel(const vector<bool>& holes, bool exploded) {
    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 1:1 통아저씨 복불복 ]                 " << endl;
    cout << "==========================================================" << endl;
    
    if (!exploded) {
        SetColor(15);
        cout << "                          .-------. " << endl;
        cout << "                          |  o o  | " << endl;
        cout << "                          '-------' " << endl;
    } else {
        SetColor(12);
        cout << "                        ＼  O  ／  " << endl;
        cout << "                            |    " << endl;
        cout << "                         [ 펑!! ]" << endl;
    }

    SetColor(6); 
    cout << "                .--------------------------. " << endl;
    cout << "               /                            ＼ " << endl;
    cout << "              |           [ BARREL ]          | " << endl;
    cout << "              |-------------------------------| " << endl;

    // --- 숫자 영역 (숫자 길이에 상관없이 벽 위치를 수동으로 노가다 정렬) ---

    // 1층 (1~5번)
    cout << "              |       "; 
    for (int i = 1; i <= 5; i++) {
        if (holes[i]) { SetColor(8); cout << "X   "; }
        else { SetColor(15); cout << i << "   "; }
    }
    SetColor(6);
    cout << "    |" << endl; // 오른쪽 벽 위치 직접 고정

    // 2층 (6~10번)
    cout << "              |       ";
    for (int i = 6; i <= 10; i++) {
        if (holes[i]) { SetColor(8); cout << "X   "; }
        else { 
            SetColor(15); 
            if (i < 10) cout << i << "   "; 
            else cout << i << "  "; // 10번은 두 자릿수라 공백 1칸 줄임
        }
    }
    SetColor(6);
    cout << "    |" << endl; // 위 줄과 동일한 위치에 직접 고정

    // 3층 (11~15번)
    cout << "              |       ";
    for (int i = 11; i <= 15; i++) {
        if (holes[i]) { SetColor(8); cout << "X   "; }
        else { SetColor(15); cout << i << "  "; } // 모두 두 자릿수라 공백 2칸
    }
    SetColor(6);
    cout << "    |" << endl; // 위 줄과 동일한 위치에 직접 고정

    // -------------------------------------------------------------------

    cout << "              |-------------------------------| " << endl;
    cout << "               ＼                             / " << endl;
    cout << "                 '---------------------------' " << endl;
    SetColor(15);
    cout << "==========================================================" << endl;
}
void PopUpGame::play() {
    random_device rd;
    mt19937 g(rd());

    while (true) {
        system("cls");
        uniform_int_distribution<int> nameDist(0, (int)globalNamePool.size() - 1);
        string enemyName = globalNamePool[nameDist(g)];

        SetColor(14);
        cout << "\n [ 통아저씨 게임 ]" << endl;
        cout << " 이번 상대: " << enemyName << endl;
        cout << " 현재 자산: " << playerRef << "$" << endl;

        if (playerRef <= 0) {
            SetColor(12);
            cout << " [!] 베팅할 자금이 전무합니다. 퇴장 처리됩니다." << endl;
            Sleep(1500); return;
        }

        // 1. bet 변수를 먼저 선언하고 입력을 받습니다.
        int bet = 0;
        while (true) {
            SetColor(15);
            cout << " 베팅 금액을 입력하세요: ";
            if (!(cin >> bet)) {
                cin.clear(); cin.ignore(10000, '\n'); continue;
            }
            if (bet > playerRef) {
                SetColor(12);
                cout << " 보유하신 금액보다 더 베팅할 수 없습니다. (MAX: " << playerRef << "$)" << endl;
                continue;
            }
            if (bet <= 0) {
                SetColor(12);
                cout << " 1$ 이상 베팅해야 게임이 시작됩니다." << endl;
                continue;
            }
            break;
        }

        playerRef -= bet;
        int pot = bet * 2; // bet이 결정된 후 pot 초기화

        vector<bool> holes(16, false);
        uniform_int_distribution<int> bombDist(1, 15);
        uniform_int_distribution<int> diceDist(1, 6); // 주사위 범위
        int bomb = bombDist(g);
        int turn = 0;

        // 2. 주사위 굴리기 (낮은 사람이 먼저 찌르기)
        while (true) {
            SetColor(15);
            cout << "\n [!] 순서를 정하기 위해 주사위를 굴립니다!" << endl;
            Sleep(800);

            int playerDice = diceDist(g);
            int enemyDice = diceDist(g);

            cout << " >> 당신의 주사위: [" << playerDice << "]" << endl;
            Sleep(800);
            cout << " >> " << enemyName << "의 주사위: [" << enemyDice << "]" << endl;
            Sleep(800);

            if (playerDice < enemyDice) {
                turn = 0; // 플레이어 선공
                SetColor(11);
                cout << " >>> 당신의 숫자가 더 낮습니다! 당신부터 시작합니다." << endl;
                break;
            }
            else if (enemyDice < playerDice) {
                turn = 1; // 상대방 선공
                SetColor(13);
                cout << " >>> " << enemyName << "의 숫자가 더 낮습니다! " << enemyName << "부터 시작합니다." << endl;
                break;
            }
            else {
                SetColor(14);
                cout << " >>> 무승부! 다시 굴립니다." << endl;
                Sleep(800);
            }
        }
        Sleep(1500);

        // 3. 실제 게임 루프
        bool gameOver = false;
        while (!gameOver) {
            drawBarrel(holes, false);
            SetColor(14);
            cout << " 상대: " << enemyName << " | 현재 판돈: " << pot << "$" << endl;

            int currentChoice = 0;
            if (turn == 0) {
                SetColor(11);
                cout << " [ 당신의 차례 ] 번호 입력: ";
                if (!(cin >> currentChoice) || currentChoice < 1 || currentChoice > 15 || holes[currentChoice]) {
                    cin.clear(); cin.ignore(10000, '\n');
                    SetColor(12);
                    cout << " 유효한 번호를 입력하세요." << endl;
                    Sleep(800); continue;
                }
            }
            else {
                SetColor(13);
                cout << " [ " << enemyName << " ] 번호를 고르는 중...";
                Sleep(1200);
                do { currentChoice = bombDist(g); } while (holes[currentChoice]);
                cout << currentChoice << "번!" << endl;
                Sleep(800);
            }

            holes[currentChoice] = true;

            if (currentChoice == bomb) {
                drawBarrel(holes, true);
                if (turn == 0) {
                    SetColor(12);
                    cout << "\n [!] 펑!! 당신이 " << enemyName << " 앞에서 튀어 올랐습니다. 패배!" << endl;
                }
                else {
                    SetColor(10);
                    cout << "\n [!] 펑!! " << enemyName << "이(가) 당첨되었습니다! 승리!" << endl;
                    cout << " 상금 " << pot << "$를 지급받습니다." << endl;
                    playerRef += pot;
                }
                gameOver = true;
            }
            else {
                turn = 1 - turn;
            }
        }

        SetColor(15);
        cout << "\n [1] 계속하기 [0] 퇴장 : ";
        int cont; cin >> cont;
        if (cont == 0) break;
    }
}