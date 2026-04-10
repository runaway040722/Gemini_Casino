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
    cout << "              [ 6. UP & DOWN GAME ]             " << endl;
    cout << "===============================================" << endl;

    // 1. 베팅 (최소 판돈을 1000$ 정도로 가정)
    int minBet = 1000;

    // 시작 전 이미 돈이 부족하면 입구 컷 (광산행)
    string pName = "당신";
    if (CheckBankruptcy(pName, money, 1, true)) return;

    int bet = GetBetAmount(money);
    if (bet <= 0) return;

    // 베팅금 선차감
    money -= bet;
    FlushBuffer();

    int answer = rand() % 100 + 1;
    int chance = 6;

    cout << "\n [ 1 ~ 100 사이 숫자를 맞추세요 ]" << endl;
    cout << " 빨리 맞출수록 높은 배당금을 획득합니다!" << endl;

    bool isWin = false; // 승리 여부 플래그

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

        if (guess == answer) {
            int winAmount;
            switch (chance) {
            case 6: winAmount = bet * 15; break;
            case 5: winAmount = bet * 7; break;
            case 4: winAmount = bet * 4; break;
            case 3: winAmount = bet * 2; break;
            default: winAmount = (int)(bet * 1.2); break;
            }

            system("cls");
            SetColor(10);
            cout << "\n\n [ BINGO!! ] 정답입니다!" << endl;
            cout << " 정답: " << answer << " | 소모 기회: " << (7 - chance) << "회" << endl;

            PrintResult(money, bet, winAmount);
            isWin = true;
            break;
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

    if (!isWin) {
        system("cls");
        SetColor(12);
        cout << "\n\n [ FAIL ] 모든 기회를 잃었습니다." << endl;
        cout << " 정답은 [ " << answer << " ] 이었습니다." << endl;
        PrintResult(money, bet, 0);
    }

    // --- [핵심] 게임 종료 후 파산 체크 ---
    // 결과 확인을 위해 잠시 대기 후 체크
    cout << "\n [!] 상태를 확인합니다...";
    Sleep(1000);

    // money가 0원이거나 다음 게임 최소 베팅금보다 적으면 광산행
    if (CheckBankruptcy(pName, money, 1, true)) {
        return; // 광산 연출 후 메인 메뉴로 강제 종료
    }

    ClearBuffer();
}