#include "BombGame.h"
#include "System.h"   // 파산 시스템 연동
#include <iostream>
#include <conio.h>
#include <ctime>
#include <windows.h>

using namespace std;

void PlayBombGame(int& money) {
    // [연동] 게임 진입 전 파산 체크
    if (CheckBankruptcy("당신", money, 1, true)) return;

    // 1. 초기 설정
    srand((unsigned)time(NULL));

    while (true) {
        system("cls");
        SetColor(14);
        cout << "===============================================" << endl;
        cout << "                [ 7. BOMB GAME ]" << endl;
        cout << "===============================================" << endl;

        // 2. 베팅 단계
        int bet = GetBetAmount(money);
        if (bet <= 0) return;

        // 베팅 금액 선차감
        money -= bet;
        FlushBuffer();

        int stage = 1;
        double multiplier = 1.2;

        bool isBusted = false; // 폭탄 폭발 여부

        while (true) {
            system("cls");

            // 상단 정보창
            SetColor(11);
            cout << "================= BOMB PROGRESS =================" << endl;
            SetColor(15);
            cout << " [ 단계 ] : " << stage << endl;
            cout << " [ 배당 ] : x" << multiplier << endl;

            int currentReward = (int)(bet * multiplier + 0.5);
            cout << " [ 현금화 시 획득 ] : " << currentReward << "$" << endl;

            int bombRate = 10 + (stage * 7);
            if (bombRate > 80) bombRate = 80;

            SetColor(12);
            cout << " [ 폭탄 확률 ] : " << bombRate << "%" << endl;
            SetColor(15);
            cout << "-----------------------------------------------" << endl;
            cout << " [1] 다음 단계 도전 (배당 +0.5)" << endl;
            cout << " [2] 여기서 멈추고 현금화 (Cash Out)" << endl;
            cout << "-----------------------------------------------" << endl;
            cout << " 선택: ";

            char choice = (char)_getch();

            // [2] 현금화 선택 시
            if (choice == '2') {
                int finalReward = (int)(bet * multiplier + 0.5);
                PrintResult(money, bet, finalReward);
                break; // 게임 루프 종료 (다음 판 의사 확인으로 이동)
            }
            // [1] 도전 선택 시
            else if (choice == '1') {
                cout << "\n\n 진행 중... 과연 결과는?!";
                Sleep(800);

                int chance = rand() % 100;

                if (chance < bombRate) {
                    system("cls");
                    SetColor(12);
                    cout << "\n\n" << endl;
                    cout << "      _ ._  _ , _ ._" << endl;
                    cout << "    (_ ' ( `  )_  ._)" << endl;
                    cout << "   (_ (  _  )   (  _  ))" << endl;
                    cout << "  (__ (_   (_ . _) _) __)" << endl;
                    cout << "      `~~`\\ ' . /`~~`" << endl;
                    cout << "           ;   ;" << endl;
                    cout << "           /   \\" << endl;
                    cout << " [ BOMB!! ] 폭탄이 터졌습니다! 베팅금을 모두 잃었습니다." << endl;

                    PrintResult(money, bet, 0);
                    isBusted = true;
                    break;
                }
                else {
                    SetColor(10);
                    cout << "\n\n [ SAFE ] 통과했습니다! 배당이 상승합니다." << endl;
                    stage++;
                    multiplier += 0.5;
                    Sleep(1000);
                }
            }
        }

        // [연동] 한 판이 완전히 끝난 후 파산 체크 (0원이면 즉시 광산행)
        if (CheckBankruptcy("당신", money, 1, true)) return;

        // 다음 판 진행 여부
        SetColor(15);
        cout << "\n [1] 다시 도전 [0] 나가기 : ";
        int nextMove;
        if (!(cin >> nextMove)) { ClearBuffer(); break; }
        if (nextMove == 0) break;
    }
}