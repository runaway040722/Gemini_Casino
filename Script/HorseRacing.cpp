#include "HorseRacing.h"
#include "System.h"   // 파산 시스템 연동
#include <iostream>
#include <vector>
#include <random>
#include <conio.h>
#include <iomanip>
#include <windows.h> 

using namespace std;

struct Horse {
    int id;
    int position;
    int color;
    double winWeight;
    double dividend;
};

void PlayHorseRacing(int& money) {
    // [연동] 게임 진입 전 파산 체크
    if (CheckBankruptcy("당신", money, 1, true)) return;

    while (true) {
        random_device rd;
        mt19937 gen(rd());

        vector<Horse> horses = {
            {1, 0, 12, 35.0, 1.8},
            {2, 0, 11, 25.0, 2.8},
            {3, 0, 14, 20.0, 4.0},
            {4, 0, 13, 12.0, 8.5},
            {5, 0, 10, 8.0, 15.0}
        };

        system("cls");
        SetColor(14);
        cout << "==========================================================" << endl;
        cout << "                [ 4. 대 박 경 마 스 타 킹 ]                " << endl;
        cout << "==========================================================" << endl;
        SetColor(15);

        // 1. 경주마 정보 출력
        cout << " [번호]   [승리 가중치]   [배당률]   [말 상태]" << endl;
        for (const auto& h : horses) {
            SetColor(h.color);
            cout << "  (" << h.id << ")번마      " << setw(3) << (int)h.winWeight << "%";
            cout << "         x" << fixed << setprecision(1) << h.dividend << "      ";
            if (h.winWeight >= 30) cout << "최상";
            else if (h.winWeight >= 20) cout << "보통";
            else cout << "부진";
            cout << endl;
        }
        SetColor(15);
        cout << "----------------------------------------------------------" << endl;

        // [연동] 베팅 금액 입력 (돈 없으면 여기서 컷)
        int bet = GetBetAmount(money);
        if (bet <= 0) return;

        // 2. 우승 예상마 선택
        system("cls");
        SetColor(14);
        cout << "==================== [ 우승마를 선택하세요 ] ====================" << endl;
        SetColor(15);
        cout << " [번호]   [승리 확률]   [배당률]" << endl;
        for (const auto& h : horses) {
            SetColor(h.color);
            cout << "  (" << h.id << ")번마      " << (int)h.winWeight << "%         x" << h.dividend << endl;
        }
        SetColor(15);
        cout << "----------------------------------------------------------" << endl;
        cout << " * 베팅 금액: $" << bet << endl;

        int pick;
        while (true) {
            SetColor(14);
            cout << " >> 몇 번 말에 거시겠습니까? (1-5): ";
            SetColor(15);
            if (cin >> pick && pick >= 1 && pick <= 5) break;
            else {
                SetColor(12);
                cout << " [!] 1~5 사이의 번호만 입력 가능합니다." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
            }
        }
        FlushBuffer();

        // 베팅금 선차감
        money -= bet;

        // 3. 경기 시작 연출
        system("cls");
        int finish = 60;
        int winner = -1;

        for (int i = 0; i < 5; i++) {
            gotoxy(5, i + 3);
            for (int j = 0; j < finish; j++) cout << "-";
            cout << "| GOAL";
        }

        uniform_real_distribution<double> dist(0.0, 100.0);
        uniform_int_distribution<int> boost(1, 3);

        while (winner == -1) {
            for (int i = 0; i < 5; i++) {
                gotoxy(horses[i].position + 5, i + 3);
                cout << "--"; // 잔상 제거용

                double chance = dist(gen);
                if (chance < 15.0) {
                    horses[i].position += 1;
                }
                else if (chance < 15.0 + (horses[i].winWeight * 0.4)) {
                    horses[i].position += boost(gen);
                }

                if (horses[i].position >= finish) {
                    horses[i].position = finish;
                    if (winner == -1) winner = horses[i].id;
                }

                gotoxy(horses[i].position + 5, i + 3);
                SetColor(horses[i].color);
                cout << horses[i].id << ">";
                SetColor(15);
            }
            Sleep(60);
        }

        // 4. 결과 발표
        gotoxy(0, 10);
        int winAmount = 0;

        if (pick == winner) {
            winAmount = (int)(bet * horses[winner - 1].dividend + 0.5);
            SetColor(10);
            cout << "\n [★적중★] " << winner << "번마가 우승했습니다!" << endl;
        }
        else {
            SetColor(12);
            cout << "\n [꽝] 우승마는 " << winner << "번마였습니다." << endl;
            winAmount = 0;
        }

        // [연동] 결과 출력 및 정산
        PrintResult(money, bet, winAmount);

        // [연동] 경기 후 파산 체크 (0원이면 즉시 광산행)
        if (CheckBankruptcy("당신", money, 1, true)) return;

        SetColor(15);
        cout << "\n [1] 다음 경기 관람 [0] 나가기 : ";
        int choice;
        cin >> choice;
        if (choice == 0) break;
    }
    ClearBuffer();
}