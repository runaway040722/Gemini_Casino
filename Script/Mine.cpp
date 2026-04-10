#include "Mine.h"
#include "System.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

Mine::Mine(int& playerMoney) : playerRef(playerMoney) {}

void Mine::StartMining() {
    ::system("cls");
    srand((unsigned)time(NULL));

    int minedAmount = 0;
    const int targetAmount = 100000; // 목표 상향

    SetColor(12);
    cout << " [파산] 채무 이행을 위해 탄광으로 강제 압송되었습니다! " << endl;
    SetColor(15);
    cout << " --------------------------------------------------" << endl;
    cout << "  상환 목표 : " << targetAmount << " $" << endl;
    cout << " --------------------------------------------------" << endl;

    int charX = 5;
    int charY = 6;

    while (minedAmount < targetAmount) {
        gotoxy(0, 3);
        SetColor(10);
        // 퍼센트 계산식 수정 (100을 곱함)
        cout << "  현재 채굴액: " << minedAmount << " $ ("
            << (minedAmount * 100 / targetAmount) << "%)    " << endl;

        // 캐릭터 기본 자세
        SetColor(8);
        gotoxy(charX, charY);     cout << "      O      ";
        gotoxy(charX, charY + 1); cout << "     /|\\_   ";
        gotoxy(charX, charY + 2); cout << "      |  \\  ";
        gotoxy(charX, charY + 3); cout << "     / \\    ";

        SetColor(15);
        gotoxy(0, charY + 5);
        cout << " [1] 곡괭이질 하기 (노동) ";

        int choice = _getch();
        while (_kbhit()) _getch(); // 입력 버퍼 비우기

        if (choice == '1') {
            // 애니메이션 1
            gotoxy(charX, charY - 1); cout << "            ";
            gotoxy(charX, charY);     cout << "  O  /       ";
            gotoxy(charX, charY + 1); cout << " /|/         ";
            gotoxy(charX, charY + 2); cout << "  |          ";
            gotoxy(charX, charY + 3); cout << " / \\         ";
            gotoxy(0, charY + 6);     cout << " [ 영차! ]      ";
            Sleep(120);

            // 애니메이션 2
            gotoxy(charX, charY - 1); cout << "            ";
            gotoxy(charX, charY);     cout << "             ";
            gotoxy(charX, charY + 1); cout << "  O __       ";
            gotoxy(charX, charY + 2); cout << " /|/  \\      ";
            gotoxy(charX, charY + 3); cout << "  |    * ";
            gotoxy(charX, charY + 4); cout << " / \\         ";

            gotoxy(0, charY + 6);
            SetColor(14);
            cout << " [ 콰과광!!! ]  ";
            Beep(600, 100);

            // 채굴량: 5,000 ~ 15,000 사이로 랜덤 (너무 금방 끝나지 않게)
            int gain = rand() % 10001 + 5000;
            minedAmount += gain;

            gotoxy(0, charY + 8);
            SetColor(15);
            cout << " >> " << gain << "$ 상당의 고순도 원석을 캤습니다!      ";

            // 보너스 다이아몬드 (10% 확률)
            if (rand() % 10 == 0) {
                int bonus = 20000;
                minedAmount += bonus;
                SetColor(11);
                gotoxy(0, charY + 9);
                cout << " [전설!] 다이아몬드 발견!! (+" << bonus << "$)      ";
            }
            else {
                gotoxy(0, charY + 9);
                cout << "                                             ";
            }
            Sleep(300);
            gotoxy(0, charY + 6); cout << "                     ";
        }
    }

    ::system("cls");
    SetColor(11);
    cout << "\n\n  ===============================================" << endl;
    cout << "              [ 채 무 이 행 완 료 ]" << endl;
    cout << "  ===============================================" << endl;
    SetColor(15);
    cout << "\n  관리자: \"엄청난 실력이군! 약속대로 50,000달러를 주지.\"" << endl;

    playerRef = 50000; // 부활 자금

    cout << "\n  아무 키나 누르면 카지노 메인으로 돌아갑니다...";
    _getch();
    while (_kbhit()) _getch();
    ::system("cls");
}

void PlayMine(int& money) {
    Mine mineGame(money);
    mineGame.StartMining();
}