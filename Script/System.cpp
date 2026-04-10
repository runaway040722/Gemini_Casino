#include "System.h"
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <iostream>

using namespace std;

// 이름 풀 (NPC용)
vector<string> globalNamePool = {
    "민지", "학진", "재용", "라이덴", "서연", "규찬", "미토마", "이강인",
    "미쿠", "카인", "선바", "선우", "유현", "정우", "세현", "동현",
    "영환", "프리렌", "니노", "아루", "유린", "카프카", "호두", "감우",
    "응광", "클레", "부현", "은랑", "스파클", "나히다", "마비카", "닐루",
    "푸리나", "청작", "제레", "호날두", "니코", "아리마", "키타가와", "페른"
};

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void FlushBuffer() {
    while (_kbhit()) _getch();
    cin.clear();
}

void ClearBuffer() {
    while (_kbhit()) _getch();
    cin.clear();
    if (cin.rdbuf()->in_avail()) {
        cin.ignore(1000, '\n');
    }
    SetColor(14);
    cout << "\n [아무 키나 누르면 메뉴로 돌아갑니다]";
    _getch();
}

// 이게 네가 원한 깔끔한 베팅 함수 전체 로직이야
int GetBetAmount(int currentMoney) {
    int bet = 0;
    while (true) {
        system("cls"); // 베팅 시작할 때 화면 깔끔하게 정리

        // 상단 헤더 느낌
        SetColor(14);
        cout << "===============================================" << endl;
        cout << "                [ 베팅 단계 ]" << endl;
        cout << "===============================================" << endl;

        SetColor(15);
        cout << "\n [ 보유 자산: " << currentMoney << "$ ]" << endl;

        SetColor(14);
        cout << " [ 베팅 금액 투입 ] (올인: a) >> ";

        string input;
        cin >> input;

        if (input == "a" || input == "A") {
            if (currentMoney <= 0) {
                SetColor(12);
                cout << "\n >> 잔액이 부족하여 올인을 할 수 없습니다!" << endl;
                Sleep(1000);
                continue;
            }
            bet = currentMoney;
            SetColor(11);
            cout << "\n >>> 올인 완료!! 모든 자산을 걸었습니다!" << endl;
        }
        else {
            try {
                bet = stoi(input);
            }
            catch (...) {
                SetColor(12);
                cout << "\n >> 숫자 또는 'a'를 정확히 입력해 주세요." << endl;
                Sleep(1000);
                continue;
            }

            if (bet > currentMoney) {
                SetColor(12);
                cout << "\n >> 자산이 부족합니다! (현재 최대 베팅: " << currentMoney << "$)" << endl;
                Sleep(1000);
                continue;
            }
            if (bet <= 0) {
                SetColor(12);
                cout << "\n >> 최소 1$ 이상은 투입해야 합니다." << endl;
                Sleep(1000);
                continue;
            }
        }

        // 베팅 성공 피드백
        SetColor(10);
        cout << " >> 베팅 성공! 잠시 후 게임이 시작됩니다...";
        Sleep(800);

        system("cls"); // ★ 베팅 끝나면 화면 싹 지우고 게임판으로 넘김
        break;
    }
    return bet;
}

void PrintResult(int& money, int bet, int winAmount) {
    int profit = winAmount - bet; // 순수익 계산

    cout << "\n-----------------------------------------------" << endl;

    if (winAmount > 0) {
        money += winAmount; // 자산 업데이트

        SetColor(10); // 초록색
        cout << " [결과] 당첨! 축하합니다." << endl;
        SetColor(15);
        cout << "  - 총 당첨금 : +$" << winAmount << endl;

        if (profit > 0) {
            SetColor(11); // 하늘색
            cout << "  - 실제 수익 : +$" << profit << endl;
        }
        else if (profit == 0) {
            SetColor(15);
            cout << "  - 실제 수익 : +$0 (본전치기!)" << endl;
        }
        else {
            SetColor(12); // 빨간색 (딴 돈보다 건 돈이 클 때)
            cout << "  - 실제 수익 : -$" << abs(profit) << " (손해 발생)" << endl;
        }
    }
    else {
        // winAmount가 0인 경우 (완전 꽝)
        SetColor(12);
        cout << " [결과] 꽝! 베팅금을 모두 잃었습니다." << endl;
        cout << "  - 손실 금액 : -$" << bet << endl;
    }

    SetColor(14); // 노란색
    cout << "\n [ 현재 자산 : $" << money << " ]" << endl;
    SetColor(15);
    cout << "-----------------------------------------------" << endl;
}
