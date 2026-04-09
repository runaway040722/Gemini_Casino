#include "HorseRacing.h"
#include "System.h"
#include <iostream>
#include <vector>
#include <random>
#include <conio.h>
#include <iomanip>

using namespace std;

struct Horse {
    int id;
    int position;
    int color;
    double winWeight; // 이 숫자가 높을수록 '가속'이 붙을 확률이 높음
    double dividend;
};

void PlayHorseRacing(int& money) {
    random_device rd;
    mt19937 gen(rd());

    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 4. 대 박 경 마 스 타 킹 ]               " << endl;
    cout << "==========================================================" << endl;
    SetColor(15);

    // 확률 합계는 100%로 유지하되, 배당률 밸런스 조정
    vector<Horse> horses = {
        {1, 0, 12, 35.0, 1.8}, // 1번: 안정적인 우승후보
        {2, 0, 11, 25.0, 2.8},
        {3, 0, 14, 20.0, 4.0},
        {4, 0, 13, 12.0, 8.5},
        {5, 0, 10, 8.0, 15.0}  // 5번: 확률은 낮지만 터지면 대박
    };

    cout << " [번호]   [승리 가중치]   [배당률]   [말 상태]" << endl;
    for (const auto& h : horses) {
        SetColor(h.color);
        cout << "  (" << h.id << ")번마    " << setw(3) << (int)h.winWeight << "%";
        cout << "          x" << fixed << setprecision(1) << h.dividend << "      ";
        if (h.winWeight >= 30) cout << "최상";
        else if (h.winWeight >= 20) cout << "보통";
        else cout << "부진";
        cout << endl;
    }
    SetColor(15);
    cout << "----------------------------------------------------------" << endl;

    int bet;
    cout << "현재 자산: $" << money << endl;
    cout << " 배팅 금액: ";
    if (!(cin >> bet) || bet <= 0 || bet > money) {
        cout << " 잘못된 금액입니다!" << endl;
        ClearBuffer(); return;
    }
    FlushBuffer();

    int pick;
    cout << " 우승 예상마 (1-5): ";
    if (!(cin >> pick) || pick < 1 || pick > 5) {
        cout << " 잘못된 번호입니다!" << endl;
        ClearBuffer(); return;
    }
    FlushBuffer();

    system("cls");
    int finish = 60; // 거리를 조금 늘려 역전 드라마 유도
    int winner = -1;

    for (int i = 0; i < 5; i++) {
        gotoxy(5, i + 3); cout << "------------------------------------------------------------| GOAL";
    }

    // 전진 로직용 분포
    uniform_real_distribution<double> dist(0.0, 100.0);
    uniform_int_distribution<int> boost(1, 3); // 가속 시 전진 칸 수

    while (winner == -1) {
        for (int i = 0; i < 5; i++) {
            gotoxy(horses[i].position + 5, i + 3); cout << "-";

            // [개선된 로직]
            // 1. 모든 말은 15%의 확률로 '기본 전진'을 함 (기초 체력)
            // 2. 추가로 각자의 winWeight에 따라 '가속'을 함
            double chance = dist(gen);

            if (chance < 15.0) { // 기본 전진
                horses[i].position += 1;
            }
            else if (chance < 15.0 + (horses[i].winWeight * 0.3)) { // 확률적 가속
                // 가속 시 1~3칸을 랜덤하게 전진해서 변수 창출
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
        Sleep(40);
    }

    gotoxy(0, 12);
    cout << "----------------------------------------------------------" << endl;

    if (pick == winner) {
        double div = horses[winner - 1].dividend;
        int reward = (int)(bet * div);
        SetColor(10);
        cout << " [적중!] " << winner << "번마 우승! (배당 x" << div << ")" << endl;
        money += (reward - bet);
    }
    else {
        SetColor(12);
        cout << " [꽝] 우승마는 " << winner << "번마였습니다." << endl;
        money -= bet;
    }

    SetColor(15);
    cout << " 현재 자산: $" << money << endl;
    ClearBuffer();
}