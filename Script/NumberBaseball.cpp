#include "NumberBaseball.h"
#include "System.h"
#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cctype>

using namespace std;

// 숫자 입력 (3자리)
string Input3Digit() {
    string input = "";
    char ch;

    while (true) {
        ch = _getch();

        if (ch == '\r') {
            if (input.length() == 3) break;
        }
        else if (isdigit(ch) && input.length() < 3) {
            input += ch;
            cout << ch;
        }
        else if (ch == '\b' && !input.empty()) {
            input.pop_back();
            cout << "\b \b";
        }
    }
    return input;
}

// 중복 체크
bool IsDuplicate(const string& s) {
    return (s[0] == s[1] || s[1] == s[2] || s[0] == s[2]);
}

void PlayNumberBaseball(int& money) {
    system("cls");

    random_device rd;
    mt19937 gen(rd());

    // 정답 생성 (중복 없는 3자리)
    vector<int> nums = { 0,1,2,3,4,5,6,7,8,9 };
    shuffle(nums.begin(), nums.end(), gen);

    string answer = to_string(nums[0]) + to_string(nums[1]) + to_string(nums[2]);

    SetColor(14);
    cout << "===============================================" << endl;
    cout << "            [ NUMBER BASEBALL ]" << endl;
    cout << "===============================================" << endl;
    SetColor(15);

    cout << "현재 자산: $" << money << endl;
    cout << "배팅 금액: ";

    int bet;
    cin >> bet;

    if (bet <= 0 || bet > money) {
        cout << "\n잘못된 배팅!" << endl;
        ClearBuffer();
        return;
    }

    FlushBuffer();

    // 선차감
    money -= bet;

    int chance = 8;

    cout << "\n[ 서로 다른 3자리 숫자를 맞추세요 ]" << endl;

    while (chance > 0) {
        cout << "\n남은 기회: " << chance;
        cout << "\n입력 (3자리): ";

        string guess = Input3Digit();

        if (IsDuplicate(guess)) {
            cout << "\n중복 숫자 금지!";
            continue;
        }

        int strike = 0;
        int ball = 0;

        for (int i = 0; i < 3; i++) {
            if (guess[i] == answer[i]) strike++;
            else if (answer.find(guess[i]) != string::npos) ball++;
        }

        if (strike == 3) {
            int reward;

            switch (chance) {
            case 8: reward = bet * 8; break;
            case 7: reward = bet * 7; break;
            case 6: reward = bet * 6; break;
            case 5: reward = bet * 5; break;
            case 4: reward = bet * 4; break;
            case 3: reward = bet * 3; break;
            case 2: reward = bet * 2; break;
            default: reward = bet; break;
            }

            SetColor(10);
            cout << "\n[ SUCCESS ] 정답!" << endl;
            cout << "획득 금액: +" << reward << "$" << endl;

            money += reward;

            SetColor(15);
            cout << "현재 자산: $" << money << endl;

            ClearBuffer();
            return;
        }

        SetColor(11);
        cout << " -> " << strike << "S " << ball << "B" << endl;
        SetColor(15);

        chance--;
    }

    // 실패
    SetColor(12);
    cout << "\n[ FAIL ] 정답: " << answer << endl;

    SetColor(15);
    cout << "현재 자산: $" << money << endl;

    ClearBuffer();
}