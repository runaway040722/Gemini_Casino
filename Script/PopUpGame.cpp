#include "PopUpGame.h"
#include "System.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <windows.h>

using namespace std;

// 외부 이름 풀 참조 (System.h 또는 다른 곳에 정의되어 있어야 함)
extern vector<string> globalNamePool;

// 생성자
PopUpGame::PopUpGame(int& playerMoney) : playerRef(playerMoney) {}

void PopUpGame::drawBarrel(const vector<bool>& holes, bool exploded) {
    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 1:1 통아저씨 복불복 ]                  " << endl;
    cout << "==========================================================" << endl;

    if (!exploded) {
        SetColor(15);
        cout << "                          .-------. " << endl;
        cout << "                          |  o o  | " << endl;
        cout << "                          '-------' " << endl;
    }
    else {
        SetColor(12); // 폭발 시 빨간색
        cout << "                        ＼  O  ／  " << endl;
        cout << "                            |      " << endl;
        cout << "                         [ 펑!! ]" << endl;
    }

    SetColor(6); // 나무통 색상 (갈색 대용)
    cout << "                .--------------------------. " << endl;
    cout << "               /                            ＼ " << endl;
    cout << "              |          [ BARREL ]          | " << endl;
    cout << "              |-------------------------------| " << endl;

    // 1~15번 구멍 배치 출력 로직
    for (int row = 0; row < 3; ++row) {
        cout << "              |       ";
        for (int col = 1; col <= 5; ++col) {
            int i = row * 5 + col;
            if (holes[i]) {
                SetColor(8); cout << "X   "; // 찌른 곳은 회색 X
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
        system("cls");
        if (playerRef <= 0) {
            SetColor(12);
            cout << "\n [!] 베팅할 자금이 없습니다. 카지노에서 쫓겨납니다." << endl;
            Sleep(1500); return;
        }

        // 상대방 이름 랜덤 설정 (NamePool이 비어있을 경우 예외 처리)
        string enemyName = globalNamePool.empty() ? "도박꾼" : globalNamePool[g() % globalNamePool.size()];

        SetColor(14);
        cout << "\n [ 통아저씨 게임 ]" << endl;
        cout << " 이번 상대: " << enemyName << endl;
        cout << " 현재 자산: $" << playerRef << endl;

        // 베팅 및 판돈(Pot) 설정
        int bet = GetBetAmount(playerRef);
        playerRef -= bet;
        int pot = bet * 2;
        FlushBuffer();

        vector<bool> holes(16, false);
        uniform_int_distribution<int> bombDist(1, 15);
        uniform_int_distribution<int> diceDist(1, 6);
        int bomb = bombDist(g); // 이번 판의 꽝 번호
        int turn = 0;

        // 순서 정하기 (주사위 대결)
        while (true) {
            SetColor(15);
            cout << "\n [!] 순서를 정하기 위해 주사위를 굴립니다!" << endl;
            Sleep(800);

            int playerDice = diceDist(g);
            int enemyDice = diceDist(g);

            cout << " >> 당신의 주사위: [" << playerDice << "]" << endl;
            Sleep(600);
            cout << " >> " << enemyName << "의 주사위: [" << enemyDice << "]" << endl;
            Sleep(600);

            if (playerDice < enemyDice) {
                turn = 0; // 플레이어 선공
                SetColor(11);
                cout << " >>> 당신의 숫자가 더 낮습니다! 선공입니다." << endl;
                break;
            }
            else if (enemyDice < playerDice) {
                turn = 1; // 상대방 선공
                SetColor(13);
                cout << " >>> " << enemyName << "의 숫자가 더 낮습니다! 선공을 뺏겼습니다." << endl;
                break;
            }
            else {
                SetColor(14);
                cout << " >>> 무승부! 다시 굴립니다." << endl;
            }
        }
        Sleep(1000);

        // 게임 루프 시작
        bool gameOver = false;
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
                    cout << " 유효하지 않거나 이미 찌른 번호입니다!" << endl;
                    Sleep(800); continue;
                }
            }
            else {
                SetColor(13);
                cout << " [ " << enemyName << " ] 번호를 고르는 중...";
                Sleep(1500); // 심리적 긴장감을 위해 조금 더 대기
                do { currentChoice = bombDist(g); } while (holes[currentChoice]);
                cout << currentChoice << "번!" << endl;
                Sleep(1000);
            }

            holes[currentChoice] = true;

            if (currentChoice == bomb) {
                drawBarrel(holes, true); // 아저씨 팝업 연출
                if (turn == 0) {
                    SetColor(12);
                    cout << "\n [!] 펑!! 당신이 당첨되었습니다! 판돈 $" << pot << "을 잃었습니다." << endl;
                }
                else {
                    SetColor(10);
                    cout << "\n [!] 펑!! " << enemyName << "이(가) 당첨되었습니다!" << endl;
                    cout << " 상금 $" << pot << "를 획득했습니다! (수익: +$" << bet << ")" << endl;
                    playerRef += pot;
                }
                gameOver = true;
            }
            else {
                turn = 1 - turn; // 턴 교체
            }
        }

        SetColor(15);
        cout << "\n [1] 계속하기 [0] 퇴장 : ";
        int cont;
        if (!(cin >> cont)) { cin.clear(); cin.ignore(10000, '\n'); break; }
        if (cont == 0) break;
    }
}