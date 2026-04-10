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
        // 상대방 이름 랜덤 설정
        string enemyName = globalNamePool.empty() ? "도박꾼" : globalNamePool[g() % globalNamePool.size()];

        system("cls");
        SetColor(14);
        cout << "\n [ 통아저씨 게임 ]" << endl;
        cout << " 이번 상대: " << enemyName << endl;

        // 1. 베팅 (System.cpp의 GetBetAmount 활용)
        int bet = GetBetAmount(playerRef);
        if (bet <= 0) return;

        // 베팅 및 판돈(Pot) 설정
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
                turn = 0; // 플레이어 선공
                SetColor(11);
                cout << " >>> 당신이 선공입니다!" << endl;
                break;
            }
            else if (enemyDice < playerDice) {
                turn = 1; // 상대방 선공
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
                    // 내가 찌른 게 터짐 (패배)
                    SetColor(12);
                    cout << "\n [!] 펑!! 당신이 당첨되었습니다!" << endl;
                    winAmount = 0;
                }
                else {
                    // 상대가 찌른 게 터짐 (승리)
                    SetColor(10);
                    cout << "\n [!] 펑!! " << enemyName << "이(가) 당첨되었습니다!" << endl;
                    winAmount = pot;
                }
                gameOver = true;
            }
            else {
                turn = 1 - turn; // 턴 교체
            }
        }

        // 4. 공통 정산 함수 호출
        PrintResult(playerRef, bet, winAmount);

        SetColor(15);
        cout << "\n [1] 계속하기 [0] 퇴장 : ";
        int cont;
        if (!(cin >> cont)) { cin.clear(); cin.ignore(10000, '\n'); break; }
        if (cont == 0) break;
    }
}