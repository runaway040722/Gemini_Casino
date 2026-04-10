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
    // [추가] 시작 전 플레이어 파산 체크 (입장 컷)
    if (CheckBankruptcy("당신", money, 1, true)) return;

    while (true) {
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
        cout << "               [ 8. NUMBER BASEBALL ]            " << endl;
        cout << "===============================================" << endl;

        // 2. 베팅
        int bet = GetBetAmount(money);
        if (bet <= 0) return;

        money -= bet;
        FlushBuffer();

        int chance = 8;
        bool isWin = false;
        cout << "\n [ 서로 다른 3자리 숫자를 맞추세요! (0~9) ]" << endl;
        cout << " 빨리 맞출수록 더 큰 보상을 받습니다." << endl;

        while (chance > 0) {
            SetColor(15);
            cout << "\n-----------------------------------------------" << endl;
            cout << " [남은 기회: " << chance << "]" << endl;
            cout << " 입력: ";

            string guess = Input3Digit();

            if (IsDuplicate(guess)) {
                SetColor(12);
                cout << " >> [경고] 중복된 숫자가 포함되어 있습니다!" << endl;
                continue;
            }

            int strike = 0;
            int ball = 0;

            for (int i = 0; i < 3; i++) {
                if (guess[i] == answer[i]) strike++;
                else {
                    for (int j = 0; j < 3; j++) {
                        if (guess[i] == answer[j]) ball++;
                    }
                }
            }

            if (strike == 3) {
                int winAmount;
                if (chance == 8) winAmount = bet * 10;
                else if (chance >= 6) winAmount = bet * 5;
                else if (chance >= 4) winAmount = bet * 3;
                else if (chance >= 2) winAmount = bet * 2;
                else winAmount = (int)(bet * 1.5);

                system("cls");
                SetColor(10);
                cout << "\n\n [ STRIKE OUT!! ] 정답을 맞췄습니다!" << endl;
                cout << " 정답: " << answer << endl;

                PrintResult(money, bet, winAmount);
                isWin = true;
                break;
            }

            if (strike == 0 && ball == 0) {
                SetColor(12);
                cout << " >> 결과: OUT (일치하는 숫자가 없습니다)" << endl;
            }
            else {
                SetColor(11);
                cout << " >> 결과: " << strike << " Strike, " << ball << " Ball" << endl;
            }
            chance--;
        }

        // 실패 시 처리
        if (!isWin) {
            system("cls");
            SetColor(12);
            cout << "\n\n [ GAME OVER ] 모든 기회를 소진했습니다." << endl;
            cout << " 정답은 [ " << answer << " ] 이었습니다." << endl;

            PrintResult(money, bet, 0);
        }

        // --- [핵심] 결과 후 파산 체크 시스템 ---
        // 게임 후 잔액이 0원이면 즉시 광산행
        if (CheckBankruptcy("당신", money, 1, true)) return;

        SetColor(15);
        cout << "\n [1] 다시하기 [0] 나가기 : ";
        int choice;
        if (!(cin >> choice)) { ClearBuffer(); break; }
        if (choice == 0) break;
    }
}