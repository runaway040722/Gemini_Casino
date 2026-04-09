#include "System.h"
#include <iostream>
#include <conio.h>
#include <ctime>

using namespace std;

void PlayBombGame(int& money) {
    system("cls");
    srand((unsigned)time(NULL));

    SetColor(14);
    cout << "===============================================" << endl;
    cout << "                [ BOMB GAME ]" << endl;
    cout << "===============================================" << endl;
    SetColor(15);

    cout << "현재 자산: $" << money << endl;
    cout << "배팅 금액: ";

    int bet;
    cin >> bet;

    if (!cin || bet <= 0 || bet > money) {
        cout << "\n잘못된 배팅!" << endl;
        ClearBuffer();
        return;
    }
    FlushBuffer();

    // 베팅 선차감
    money -= bet;

    int stage = 0;
    double multiplier = 1.0;

    while (true) {
        system("cls");

        SetColor(11);
        cout << "================= 진행 상황 =================" << endl;
        SetColor(15);

        cout << "현재 단계: " << stage << endl;
        cout << "현재 배당: x" << multiplier << endl;
        cout << "예상 획득 금액: " << (int)(bet * multiplier) << "$" << endl;

        int bombRate = 10 + (stage * 8);
        if (bombRate > 70) bombRate = 70;

        cout << "폭탄 확률: " << bombRate << "%" << endl;

        cout << "\n[1] 계속 진행";
        cout << "\n[2] 멈추고 수령";
        cout << "\n선택: ";

        char choice = _getch();

        // 버퍼 제거
        while (_kbhit()) _getch();

        if (choice == '2') {
            int reward = (int)(bet * multiplier);

            SetColor(10);
            cout << "\n[ CASH OUT ] +" << reward << "$" << endl;

            money += reward;

            SetColor(15);
            cout << "현재 자산: $" << money << endl;

            ClearBuffer();
            return;
        }

        else if (choice == '1') {
            int chance = rand() % 100;

            if (chance < bombRate) {
                SetColor(12);
                cout << "\n[ BOMB ] 터졌습니다. 전부 잃었습니다." << endl;

                SetColor(15);
                cout << "현재 자산: $" << money << endl;

                ClearBuffer();
                return;
            }
            else {
                stage++;
                multiplier += 0.5;

                SetColor(10);
                cout << "\n[ SAFE ] 성공!" << endl;

                Sleep(500);
            }
        }
    }
}