#include "SlotMachine.h"
#include "System.h"   
#include <iostream>
#include <random>
#include <windows.h>

using namespace std;

void PlaySlotMachine(int& money) {
    string symbols[] = { "바나나", "  7   ", " 폭탄 ", " 체리 ", "다이아" };

    // 1. 베팅 단계 (GetBetAmount 내부에서 화면을 지우고 시작함)
    int bet = GetBetAmount(money);
    FlushBuffer();

    money -= bet; // 베팅금 선차감

    // 2. 슬롯 회전 연출
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 4);
    string slot[3] = { " ??? ", " ??? ", " ??? " };

    for (int i = 0; i <= 35; i++) {
        gotoxy(0, 5);
        cout << "\n      ┌────────┬────────┬────────┐" << endl;
        cout << "      │";

        if (i < 15) slot[0] = symbols[dist(gen)];
        if (i < 25) slot[1] = symbols[dist(gen)];
        if (i < 35) slot[2] = symbols[dist(gen)];

        for (int j = 0; j < 3; j++) {
            if (slot[j] == "  7   ") SetColor(14);
            else if (slot[j] == " 폭탄 ") SetColor(12);
            else if (slot[j] == "다이아") SetColor(11);
            else SetColor(15);

            cout << " " << slot[j] << " ";
            SetColor(15);
            cout << "│";
        }
        cout << "\n      └────────┴────────┴────────┘" << endl;

        if (i == 15 || i == 25 || i == 35) Sleep(500);
        else Sleep(40);
    }

    // 3. 결과 판정 (당첨금 winAmount만 계산)
    int winAmount = 0;
    string s1 = slot[0], s2 = slot[1], s3 = slot[2];

    if (s1 == "  7   " && s2 == "  7   " && s3 == "  7   ") {
        winAmount = bet * 50; // 잭팟
    }
    else if (s1 == s2 && s2 == s3) {
        if (s1 != " 폭탄 ") winAmount = bet * 5; // 3개 일치
    }
    else if (s1 == s2 || s2 == s3 || s1 == s3) {
        winAmount = (int)(bet * 1.5); // 2개 일치
    }

    // 4. 공통 정산 함수 호출 (순수익 계산 및 출력)
    PrintResult(money, bet, winAmount);

    ClearBuffer();
}