#include "NumberBaseball.h"
#include "System.h"   
#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cctype>
#include <windows.h>

using namespace std;

// 숫자 입력 (3자리 전용 인터페이스 - 실시간 피드백)
string Input3Digit() {
    string input = "";
    char ch;

    while (true) {
        ch = (char)_getch();

        // 엔터 키: 3자리가 다 찼을 때만 허용
        if (ch == '\r') {
            if (input.length() == 3) break;
        }
        // 숫자 키: 3자리 미만일 때만 추가
        else if (isdigit(ch) && input.length() < 3) {
            input += ch;
            cout << ch;
        }
        // 백스페이스 키: 글자 삭제
        else if (ch == '\b' && !input.empty()) {
            input.pop_back();
            cout << "\b \b";
        }
    }
    cout << endl;
    return input;
}

// 중복 체크 로직
bool IsDuplicate(const string& s) {
    if (s.length() < 3) return false;
    return (s[0] == s[1] || s[1] == s[2] || s[0] == s[2]);
}

void PlayNumberBaseball(int& money) {
    system("cls");

    random_device rd;
    mt19937 gen(rd());

    // 1. 정답 생성 (중복 없는 3자리)
    vector<int> nums = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    shuffle(nums.begin(), nums.end(), gen);

    string answer = "";
    for (int i = 0; i < 3; i++) {
        answer += to_string(nums[i]);
    }

    SetColor(14);
    cout << "===============================================" << endl;
    cout << "             [ 8. NUMBER BASEBALL ]            " << endl;
    cout << "===============================================" << endl;
    SetColor(15);

    // 2. 공통 시스템 함수로 베팅 받기
    cout << " 현재 자산: $" << money << endl;
    int bet = GetBetAmount(money);
    FlushBuffer();

    // 베팅금 선차감
    money -= bet;

    int chance = 8;
    cout << "\n [ 서로 다른 3자리 숫자를 맞추세요! (0~9) ]" << endl;
    cout << " 빨리 맞출수록 더 큰 보상을 받습니다." << endl;

    while (chance > 0) {
        SetColor(15);
        cout << "\n-----------------------------------------------" << endl;
        cout << " [남은 기회: " << chance << "]" << endl;
        cout << " 입력: ";

        string guess = Input3Digit();

        // 중복 입력 방어
        if (IsDuplicate(guess)) {
            SetColor(12);
            cout << " >> [경고] 중복된 숫자가 포함되어 있습니다!" << endl;
            continue;
        }

        int strike = 0;
        int ball = 0;

        // 판정 로직
        for (int i = 0; i < 3; i++) {
            if (guess[i] == answer[i]) {
                strike++;
            }
            else {
                for (int j = 0; j < 3; j++) {
                    if (guess[i] == answer[j]) ball++;
                }
            }
        }

        // 승리 조건
        if (strike == 3) {
            int reward;
            // 남은 기회에 따른 배당 (전략적 보상)
            if (chance == 8) reward = bet * 10;      // 1트 (천운)
            else if (chance >= 6) reward = bet * 5;  // 2~3트
            else if (chance >= 4) reward = bet * 3;  // 4~5트
            else if (chance >= 2) reward = bet * 2;  // 6~7트
            else reward = (int)(bet * 1.5);          // 8트

            system("cls");
            SetColor(10);
            cout << "\n\n [ STRIKE OUT!! ] 정답을 맞췄습니다!" << endl;
            cout << " 정답: " << answer << endl;
            cout << " 보상: +$" << reward << endl;

            money += reward;
            SetColor(15);
            cout << " 현재 자산: $" << money << endl;

            ClearBuffer();
            return;
        }

        // 힌트 출력 (색상 대비)
        if (strike == 0 && ball == 0) {
            SetColor(12); // 아웃은 빨간색 계열
            cout << " >> 결과: OUT (일치하는 숫자가 없습니다)" << endl;
        }
        else {
            SetColor(11); // 힌트는 하늘색
            cout << " >> 결과: " << strike << " Strike, " << ball << " Ball" << endl;
        }

        chance--;
    }

    // 실패 로직
    system("cls");
    SetColor(12);
    cout << "\n\n [ GAME OVER ] 모든 기회를 소진했습니다." << endl;
    cout << " 정답은 [ " << answer << " ] 이었습니다." << endl;

    SetColor(15);
    cout << " 현재 자산: $" << money << endl;

    ClearBuffer();
}