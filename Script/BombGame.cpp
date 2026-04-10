#include "BombGame.h"
#include "System.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <windows.h>

using namespace std;

void PlayBombGame(int& money) {
    // 1. 초기 설정
    srand((unsigned)time(NULL));
    system("cls");

    SetColor(14);
    cout << "===============================================" << endl;
    cout << "                [ 7. BOMB GAME ]" << endl;
    cout << "===============================================" << endl;

    // 2. 베팅 단계 (System.cpp의 GetBetAmount 사용)
    int bet = GetBetAmount(money);
    if (bet <= 0) return;

    // 베팅 금액 선차감 (자산에서 판돈을 먼저 뺌)
    money -= bet;
    FlushBuffer();

    int stage = 1;
    double multiplier = 1.2; // 1단계 성공 시 기본 배당 (x1.2)

    while (true) {
        system("cls");

        // 상단 정보창
        SetColor(11);
        cout << "================= BOMB PROGRESS =================" << endl;
        SetColor(15);
        cout << " [ 단계 ] : " << stage << endl;
        cout << " [ 배당 ] : x" << multiplier << endl;

        // 현재 시점의 예상 당첨금 (반올림 처리)
        int currentReward = (int)(bet * multiplier + 0.5);
        cout << " [ 현금화 시 획득 ] : " << currentReward << "$" << endl;

        // 단계가 올라갈수록 폭탄 확률 증가 (최대 80%)
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

            // 공통 정산 함수 호출 (돈을 벌었으므로 finalReward 전달)
            PrintResult(money, bet, finalReward);

            ClearBuffer();
            return;
        }
        // [1] 도전 선택 시
        else if (choice == '1') {
            cout << "\n\n 진행 중... 과연 결과는?!";
            Sleep(800);

            int chance = rand() % 100;

            // 폭탄이 터졌을 때
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

                // 공통 정산 함수 호출 (꽝이므로 당첨금 0)
                PrintResult(money, bet, 0);

                ClearBuffer();
                return;
            }
            // 안전하게 통과했을 때
            else {
                SetColor(10);
                cout << "\n\n [ SAFE ] 통과했습니다! 배당이 상승합니다." << endl;

                stage++;
                multiplier += 0.5; // 상승폭 0.8 -> 0.5로 밸런스 조정

                Sleep(1000);
            }
        }
    }
}