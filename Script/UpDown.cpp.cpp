#include "UpDown.h"
#include "System.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <string>
#include <windows.h>

using namespace std;

void PlayUpDown(int& money) {
    system("cls");
    srand((unsigned)time(NULL));

    SetColor(14);
    cout << "===============================================" << endl;
    cout << "              [ 6. UP & DOWN GAME ]            " << endl;
    cout << "===============================================" << endl;

    // 1. 베팅 (System.cpp의 GetBetAmount 활용)
    int bet = GetBetAmount(money);
    if (bet <= 0) return;

    // 베팅금 선차감
    money -= bet;
    FlushBuffer();

    // 1~100 사이의 정답 생성
    int answer = rand() % 100 + 1;
    int chance = 6;

    cout << "\n [ 1 ~ 100 사이 숫자를 맞추세요 ]" << endl;
    cout << " 빨리 맞출수록 높은 배당금을 획득합니다!" << endl;

    while (chance > 0) {
        SetColor(15);
        cout << "\n-----------------------------------------------" << endl;
        cout << " [남은 기회: " << chance << "]" << endl;
        cout << " 입력: ";

        int guess;
        if (!(cin >> guess)) {
            cin.clear();
            cin.ignore(1000, '\n');
            SetColor(12);
            cout << " 숫자를 입력해주세요!" << endl;
            continue;
        }

        if (guess < 1 || guess > 100) {
            SetColor(12);
            cout << " 범위(1~100)를 벗어났습니다!" << endl;
            continue;
        }

        // --- 정답 판정 ---
        if (guess == answer) {
            int winAmount;

            // 남은 기회에 따른 차등 배당 설정
            switch (chance) {
            case 6: winAmount = bet * 15; break; // 1트 (초대박)
            case 5: winAmount = bet * 7; break;  // 2트
            case 4: winAmount = bet * 4; break;  // 3트
            case 3: winAmount = bet * 2; break;  // 4트
            default: winAmount = (int)(bet * 1.2); break; // 나머지
            }

            system("cls");
            SetColor(10);
            cout << "\n\n [ BINGO!! ] 정답입니다!" << endl;
            cout << " 정답: " << answer << " | 소모 기회: " << (7 - chance) << "회" << endl;

            // ★ 공통 정산 함수 호출 (당첨)
            PrintResult(money, bet, winAmount);

            ClearBuffer();
            return;
        }
        else if (guess < answer) {
            SetColor(11);
            cout << " >> [ UP ] 더 큰 숫자입니다." << endl;
        }
        else {
            SetColor(12);
            cout << " >> [ DOWN ] 더 작은 숫자입니다." << endl;
        }

        chance--;
    }

    // --- 실패 로직 ---
    system("cls");
    SetColor(12);
    cout << "\n\n [ FAIL ] 모든 기회를 잃었습니다." << endl;
    cout << " 정답은 [ " << answer << " ] 이었습니다." << endl;

    // ★ 공통 정산 함수 호출 (꽝: winAmount = 0)
    PrintResult(money, bet, 0);

    ClearBuffer();
}