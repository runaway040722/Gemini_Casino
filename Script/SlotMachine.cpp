#include "SlotMachine.h"
#include "System.h"   // SetColor, FlushBuffer, ClearBuffer 사용

using namespace std;

void PlaySlotMachine(int& money) {
    string symbols[] = { "바나나", "  7   ", " 폭탄 ", " 체리 ", "다이아" };

    system("cls"); SetColor(14);
    cout << "===============================================" << endl;
    cout << "          [ 1. 777 LUCKY SLOT ]" << endl;
    cout << "===============================================" << endl; SetColor(15);

    int bet; cout << "배팅 금액: ";
    if (!(cin >> bet) || bet <= 0 || bet > money) {
        cout << "잘못된 배팅!";
        ClearBuffer(); // 청소 + 대기 후 바로 종료
        return;
    }

    // [개선] 배팅 후 엔터를 막 쳤거나 문자를 섞었어도 여기서 싹 비움
    FlushBuffer();

    random_device rd; mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 4);

    string slot[3] = { " ??? ", " ??? ", " ??? " };

    for (int i = 0; i <= 35; i++) {
        system("cls"); SetColor(14);
        cout << "===============================================" << endl;
        cout << "          [ 1. 777 LUCKY SLOT ]" << endl;
        cout << "===============================================" << endl; SetColor(15);

        cout << "\n      ┌────────┬────────┬────────┐" << endl;
        cout << "      │";

        if (i < 15) slot[0] = symbols[dist(gen)];
        if (i < 25) slot[1] = symbols[dist(gen)];
        if (i < 35) slot[2] = symbols[dist(gen)];

        for (int j = 0; j < 3; j++) {
            cout << " " << slot[j] << " │";
        }
        cout << "\n      └────────┴────────┴────────┘" << endl;

        if (i == 15 || i == 25 || i == 35) {
            Sleep(400);
        }
        else {
            Sleep(40);
        }
    }

    // --- 결과 판정 ---
    int winAmount = 0;
    if (slot[0] == "  7   " && slot[1] == "  7   " && slot[2] == "  7   ") {
        SetColor(10); cout << "\n*** JACKPOT!!! 7-7-7 (50배) ***" << endl;
        winAmount = bet * 50;
    }
    else if (slot[0] == slot[1] && slot[1] == slot[2]) {
        if (slot[0] == " 폭탄 ") { SetColor(12); cout << "\n폭탄 3개! 꽝입니다." << endl; }
        else { SetColor(11); cout << "\n그림 일치! (5배)" << endl; winAmount = bet * 5; }
    }
    else {
        SetColor(8); cout << "\n아쉽게도 꽝입니다." << endl;
    }

    // --- 정산 ---
    if (winAmount > 0) {
        money += (winAmount - bet);
        SetColor(10); cout << "획득 금액: +$" << winAmount << endl;
    }
    else {
        money -= bet;
        SetColor(12); cout << "손실 금액: -$" << bet << endl;
    }

    SetColor(15);
    cout << "-----------------------------------------------" << endl;
    cout << "현재 보유 자산: $" << money << endl;

    // [개선] 게임 끝! 청소하고 "아무 키나 누르세요" 대기
    ClearBuffer();
}