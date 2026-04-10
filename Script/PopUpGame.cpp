#include "PopUpGame.h"
#include "System.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <windows.h>
#include <conio.h>

using namespace std;

extern vector<string> globalNamePool;

PopUpGame::PopUpGame(int& playerMoney) : playerRef(playerMoney) {}

void PopUpGame::drawBarrel(const vector<bool>& holes, bool exploded) {
    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 1:1 통아저씨 복불복 ]                   " << endl;
    cout << "==========================================================" << endl;

    if (!exploded) {
        SetColor(15);
        cout << "                          .-------. " << endl;
        cout << "                          |  o o  | " << endl;
        cout << "                          '-------' " << endl;
    }
    else {
        SetColor(12);
        cout << "                        ＼  O  ／  " << endl;
        cout << "                            |      " << endl;
        cout << "                          [ 펑!! ]" << endl;
    }

    SetColor(6);
    cout << "                .--------------------------. " << endl;
    cout << "               /                            ＼ " << endl;
    cout << "              |          [ BARREL ]           | " << endl;
    cout << "              |-------------------------------| " << endl;

    for (int row = 0; row < 3; ++row) {
        cout << "              |       ";
        for (int col = 1; col <= 5; ++col) {
            int i = row * 5 + col;
            if (holes[i]) {
                SetColor(8); cout << "X   ";
            }
            else {
                SetColor(15);
                cout << i << (i < 10 ? "   " : "  ");
            }
            SetColor(6);
        }
        cout << "    |" << endl;
    }

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
        // [추가] 시작 전 플레이어 파산 체크
        string pName = "당신";
        if (CheckBankruptcy(pName, playerRef, 1, true)) return;

        // 상대방 이름 랜덤 설정
        string enemyName = globalNamePool.empty() ? "도박꾼" : globalNamePool[g() % globalNamePool.size()];
        int enemyMoney = 100000; // 상대방 가상 자금

        system("cls");
        SetColor(14);
        cout << "\n [ 통아저씨 게임 ]" << endl;
        cout << " 이번 상대: " << enemyName << endl;

        // 1. 베팅
        int bet = GetBetAmount(playerRef);
        if (bet <= 0) return;

        playerRef -= bet;
        int pot = bet * 2;
        FlushBuffer();

        vector<bool> holes(16, false);
        uniform_int_distribution<int> bombDist(1, 15);
        uniform_int_distribution<int> diceDist(1, 6);
        int bomb = bombDist(g);
        int turn = 0;

        // 2. 순서 정하기
        while (true) {
            SetColor(15);
            cout << "\n [!] 순서를 정하기 위해 주사위를 굴립니다!" << endl;
            Sleep(500);

            int playerDice = diceDist(g);
            int enemyDice = diceDist(g);

            cout << " >> 당신의 주사위: [" << playerDice << "]" << endl;
            cout << " >> " << enemyName << "의 주사위: [" << enemyDice << "]" << endl;

            if (playerDice < enemyDice) {
                turn = 0;
                SetColor(11);
                cout << " >>> 당신이 선공입니다!" << endl;
                break;
            }
            else if (enemyDice < playerDice) {
                turn = 1;
                SetColor(13);
                cout << " >>> " << enemyName << "이(가) 선공입니다!" << endl;
                break;
            }
            else {
                cout << " >>> 무승부! 다시 굴립니다." << endl;
            }
        }
        Sleep(1000);

        // 3. 게임 루프 시작
        bool gameOver = false;
        int winAmount = 0;

        while (!gameOver) {
            drawBarrel(holes, false);
            SetColor(14);
            cout << " 상대: " << enemyName << " | 현재 판돈: $" << pot << endl;

            int currentChoice = 0;
            if (turn == 0) {
                SetColor(11);
                cout << " [ 당신의 차례 ] 번호 입력 (1~15): ";
                if (!(cin >> currentChoice) || currentChoice < 1 || currentChoice > 15 || holes[currentChoice]) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    SetColor(12);
                    cout << " 유효하지 않은 번호입니다!" << endl;
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
                    cout << "\n [!] 펑!! 당신이 당첨되었습니다!" << endl;
                    winAmount = 0;
                }
                else {
                    SetColor(10);
                    cout << "\n [!] 펑!! " << enemyName << "이(가) 당첨되었습니다!" << endl;
                    winAmount = pot;
                }
                gameOver = true;
                Sleep(1500);
            }
            else {
                turn = 1 - turn;
            }
        }

        // 4. 공통 정산 함수 호출
        PrintResult(playerRef, bet, winAmount);

        // --- [핵심] 결과 후 파산 체크 시스템 ---
        // 내가 졌다면 나의 파산을 체크
        if (winAmount == 0) {
            if (CheckBankruptcy(pName, playerRef, 1, true)) return;
        }
        // 내가 이겼다면 상대방(AI)의 파산을 연출 (이 게임은 판돈을 다 잃은 것으로 간주)
        else {
            enemyMoney -= bet; // 가상 자금 차감
            if (enemyMoney <= 95000) { // 예시: 일정 판돈 이하가 되면 상대가 끌려감
                DragToMine(enemyName, false);
                cout << " [!] 새로운 도전자가 대기실에서 들어옵니다..." << endl;
                Sleep(1000);
            }
        }

        SetColor(15);
        cout << "\n [1] 계속하기 [0] 퇴장 : ";
        int cont;
        if (!(cin >> cont)) { cin.clear(); cin.ignore(10000, '\n'); break; }
        if (cont == 0) break;
    }
}