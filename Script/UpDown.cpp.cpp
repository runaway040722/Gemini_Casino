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
    SetColor(15);

    // 1. 공통 시스템 함수로 베팅 받기
    cout << " 현재 자산: $" << money << endl;
    int bet = GetBetAmount(money);
    FlushBuffer();

    // 베팅금 선차감
    money -= bet;

    // 1~100 사이의 정답 생성
    int answer = rand() % 100 + 1;
    int chance = 6; // 6번의 기회 (이진 탐색 효율을 고려한 적절한 횟수)

    cout << "\n [ 1 ~ 100 사이 숫자를 맞추세요 ]" << endl;
    cout << " 빨리 맞출수록 높은 배당금을 획득합니다!" << endl;

    while (chance > 0) {
        SetColor(15);
        cout << "\n-----------------------------------------------" << endl;
        cout << " [남은 기회: " << chance << "]" << endl;
        cout << " 입력: ";

        int guess;
        // 숫자 입력 예외 처리
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

        // 정답 판정
        if (guess == answer) {
            int reward;

            // 남은 기회에 따른 전략적 차등 배당
            switch (chance) {
            case 6: reward = bet * 15; break; // 1트 (초대박)
            case 5: reward = bet * 7; break;  // 2트
            case 4: reward = bet * 4; break;  // 3트
            case 3: reward = bet * 2; break;  // 4트
            default: reward = (int)(bet * 1.2); break; // 나머지 (원금 보전 수준)
            }

            system("cls");
            SetColor(10);
            cout << "\n\n [ BINGO!! ] 정답입니다!" << endl;
            cout << " 정답: " << answer << " | 소모 기회: " << (7 - chance) << "회" << endl;
            cout << " 획득 금액: +$" << reward << endl;

            money += reward; // 배당 지급

            SetColor(15);
            cout << " 현재 자산: $" << money << endl;

            ClearBuffer();
            return;
        }
        else if (guess < answer) {
            SetColor(11); // UP은 하늘색
            cout << " >> [ UP ] 더 큰 숫자입니다." << endl;
        }
        else {
            SetColor(12); // DOWN은 빨간색
            cout << " >> [ DOWN ] 더 작은 숫자입니다." << endl;
        }

        chance--;
    }

    // 모든 기회 소진 (실패)
    system("cls");
    SetColor(12);
    cout << "\n\n [ FAIL ] 모든 기회를 잃었습니다." << endl;
    cout << " 정답은 [ " << answer << " ] 이었습니다." << endl;

    SetColor(15);
    cout << " 현재 자산: $" << money << endl;

    ClearBuffer();
}