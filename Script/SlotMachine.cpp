#include "SlotMachine.h"
#include "System.h"   
#include <iostream>
#include <random>
#include <windows.h>

using namespace std;

void PlaySlotMachine(int& money) {
    // [추가] 시작 전 파산 상태라면 입구 컷
    string pName = "당신";
    if (CheckBankruptcy(pName, money, 1, true)) return;

    string symbols[] = { "바나나", "  7   ", " 폭탄 ", " 체리 ", "다이아" };

    // 1. 베팅 단계
    int bet = GetBetAmount(money);
    if (bet <= 0) return; // 취소 시 복귀

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

    // 3. 결과 판정
    int winAmount = 0;
    string s1 = slot[0], s2 = slot[1], s3 = slot[2];

    if (s1 == "  7   " && s2 == "  7   " && s3 == "  7   ") {
        winAmount = bet * 50; // 잭팟
    }
    else if (s1 == s2 && s2 == s3) {
        if (s1 != " 폭탄 ") winAmount = bet * 10; // 3개 일치 (배당 상향)
        else winAmount = 0; // 폭탄 3개는 얄짤없음
    }
    else if (s1 == s2 || s2 == s3 || s1 == s3) {
        if (s1 == " 폭탄 " && s2 == " 폭탄 ") winAmount = 0; // 폭탄 2개도 꽝
        else winAmount = (int)(bet * 1.5);
    }

    // 4. 공통 정산 함수 호출
    PrintResult(money, bet, winAmount);

    // --- [핵심] 정산 후 파산 체크 ---
    // 잭팟에 실패하고 돈이 다 떨어졌다면 바로 광산행
    if (CheckBankruptcy(pName, money, 1, true)) {
        return;
    }

    ClearBuffer();
}