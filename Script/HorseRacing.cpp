#include "HorseRacing.h"
#include "System.h"   
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
    double winWeight; // 가속 확률 가중치
    double dividend;  // 배당률
};

void PlayHorseRacing(int& money) {
    random_device rd;
    mt19937 gen(rd());

    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 4. 대 박 경 마 스 타 킹 ]                " << endl;
    cout << "==========================================================" << endl;
    SetColor(15);

    // 말 데이터 설정
    vector<Horse> horses = {
        {1, 0, 12, 35.0, 1.8}, // 1번: 안정적인 우승후보
        {2, 0, 11, 25.0, 2.8},
        {3, 0, 14, 20.0, 4.0},
        {4, 0, 13, 12.0, 8.5},
        {5, 0, 10, 8.0, 15.0}  // 5번: 역전의 다크호스
    };

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

    cout << " 현재 자산: $" << money << endl;
    int bet = GetBetAmount(money);

    // 우승 예상마 선택
    int pick;
    while (true) {
        cout << " 우승 예상마 선택 (1-5): ";
        if (cin >> pick && pick >= 1 && pick <= 5) {
            break;
        }
        else {
            SetColor(12);
            cout << " [!] 1번부터 5번 사이의 번호를 입력하세요." << endl;
            SetColor(15);
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    FlushBuffer();

    // 게임 시작 전 베팅금 선차감
    money -= bet;

    system("cls");
    int finish = 60; // 트랙 길이
    int winner = -1;

    // 초기 트랙 가이드 라인 출력
    for (int i = 0; i < 5; i++) {
        gotoxy(5, i + 3);
        for (int j = 0; j < finish; j++) cout << "-";
        cout << "| GOAL";
    }

    uniform_real_distribution<double> dist(0.0, 100.0);
    uniform_int_distribution<int> boost(1, 3);

    // 질주 시작
    while (winner == -1) {
        for (int i = 0; i < 5; i++) {
            // 1. 이전 위치에 트랙 복구 (말 모양이 i> 이므로 두 칸 공백/트랙 처리)
            gotoxy(horses[i].position + 5, i + 3);
            cout << "--";

            // 2. 이동 로직
            double chance = dist(gen);
            if (chance < 15.0) {
                horses[i].position += 1;
            }
            else if (chance < 15.0 + (horses[i].winWeight * 0.4)) { // 가중치 영향력 소폭 상향
                horses[i].position += boost(gen);
            }

            // 결승선 체크
            if (horses[i].position >= finish) {
                horses[i].position = finish;
                if (winner == -1) winner = horses[i].id;
            }

            // 3. 말 출력
            gotoxy(horses[i].position + 5, i + 3);
            SetColor(horses[i].color);
            cout << horses[i].id << ">";
            SetColor(15);
        }
        Sleep(60); // 질주 속도감 조절
    }

    // 결과 정산
    gotoxy(0, 10);
    cout << "\n----------------------------------------------------------" << endl;

    if (pick == winner) {
        // 배당금 계산 시 반올림 처리
        int reward = (int)(bet * horses[winner - 1].dividend + 0.5);
        SetColor(10);
        cout << " [★적중★] " << winner << "번마가 우승했습니다! 배당금: " << reward << "$" << endl;
        money += reward;
    }
    else {
        SetColor(12);
        cout << " [꽝] 우승마는 " << winner << "번마였습니다. 배팅금을 잃었습니다." << endl;
    }

    SetColor(15);
    cout << " 현재 보유 자산: $" << money << endl;
    cout << "----------------------------------------------------------" << endl;

    ClearBuffer();
}