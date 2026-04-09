#include "System.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <string>

using namespace std;

// 숫자 입력 (_getch 기반)
int InputNumber() {
    string input = "";
    char ch;

    while (true) {
        ch = _getch();

        if (ch == '\r') {
            if (!input.empty()) break;
        }
        else if (isdigit(ch)) {
            input += ch;
            cout << ch;
        }
        else if (ch == '\b' && !input.empty()) {
            input.pop_back();
            cout << "\b \b";
        }
    }
    return stoi(input);
}

// 업다운 게임
void PlayUpDown(int& money) {
    system("cls");
    srand((unsigned)time(NULL));

    SetColor(14);
    cout << "===============================================" << endl;
    cout << "              [ UP & DOWN GAME ]" << endl;
    cout << "===============================================" << endl;
    SetColor(15);

    cout << "현재 자산: $" << money << endl;
    cout << "배팅 금액: ";
    int bet = InputNumber();

    if (bet <= 0 || bet > money) {
        cout << "\n잘못된 배팅!" << endl;
        ClearBuffer();
        return;
    }

    // 베팅 선차감
    money -= bet;

    int answer = rand() % 100 + 1;
    int chance = 5;

    cout << "\n[ 1 ~ 100 사이 숫자를 맞추세요 ]" << endl;

    while (chance > 0) {
        cout << "\n남은 기회: " << chance;
        cout << "\n입력: ";

        int guess = InputNumber();

        if (guess < 1 || guess > 100) {
            cout << "\n범위 밖입니다!";
            continue;
        }

        if (guess == answer) {
            int reward;

            // 배당
            switch (chance) {
            case 5: reward = bet * 5; break;
            case 4: reward = bet * 4; break;
            case 3: reward = bet * 3; break;
            case 2: reward = bet * 2; break;
            default: reward = bet; break;
            }

            SetColor(10);
            cout << "\n[ SUCCESS ] 정답!! +" << reward << "$" << endl;

            // ?? 배당 지급
            money += reward;

            SetColor(15);
            cout << "현재 자산: $" << money << endl;

            ClearBuffer();
            return;
        }
        else if (guess < answer) {
            SetColor(11);
            cout << " >> UP" << endl;
        }
        else {
            SetColor(12);
            cout << " >> DOWN" << endl;
        }

        SetColor(15);
        chance--;
    }

    // 실패 (이미 돈 빠졌음)
    SetColor(12);
    cout << "\n[ FAIL ] 실패! 정답: " << answer << endl;

    SetColor(15);
    cout << "현재 자산: $" << money << endl;

    ClearBuffer();
}