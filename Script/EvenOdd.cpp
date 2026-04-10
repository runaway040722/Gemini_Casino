#include "EvenOdd.h"
#include "System.h"    
#include <random>
#include <vector>
#include <iomanip>
#include <iostream>    
#include <conio.h>     
#include <windows.h>

using namespace std;

struct Point { int x, y; };

// 홀짝 색상 설정 (홀수: 흰색, 짝수: 빨간색)
void SetNumberColor(int num) {
    if (num % 2 != 0) SetColor(15); // 홀수 흰색
    else SetColor(12);              // 짝수 빨간색
}

void PlayEvenOdd(int& money) {
    random_device rd; mt19937 gen(rd());
    // 궤도가 36칸이므로 1~36 범위 설정
    uniform_int_distribution<int> dist(1, 36);

    system("cls");
    int bet;
    cout << "현재 자산: $" << money << endl;
    cout << "배팅 금액: ";

    // 배팅 금액 입력 및 유효성 검사
    if (!(cin >> bet) || bet <= 0 || bet > money) {
        cout << "잘못된 배팅 금액입니다!" << endl;
        ClearBuffer();
        return;
    }
    FlushBuffer();

    cout << "1. 홀수(Odd)  2. 짝수(Even) : ";
    char choice = (char)_getch();
    FlushBuffer();

    // --- 36칸 좌표 생성 (테두리 정렬) ---
    vector<Point> orbit;
    // 상단 (1~10)
    for (int x = 10; x <= 40; x += 3) orbit.push_back({ x, 4 });
    // 우측 (11~18)
    for (int y = 5; y <= 12; y += 1) orbit.push_back({ 40, y });
    // 하단 (19~28)
    for (int x = 40; x >= 10; x -= 3) orbit.push_back({ x, 13 });
    // 좌측 (29~36)
    for (int y = 12; y >= 5; y -= 1) orbit.push_back({ 10, y });

    // --- 초기 판 출력 ---
    system("cls");
    for (int i = 0; i < (int)orbit.size(); i++) {
        int num = i + 1;
        gotoxy(orbit[i].x, orbit[i].y);
        SetNumberColor(num);
        cout << num;
    }
    // 중앙 전광판 틀 출력
    gotoxy(23, 8); SetColor(14); cout << "[    ]";

    int currentIdx = 0;
    int totalSteps = 60 + (gen() % 30); // 회전수 설정 (충분히 돌도록 상향)

    // --- 구슬 회전 및 전광판 연출 ---
    for (int i = 0; i < totalSteps; i++) {
        // 1. 이전 위치 숫자 복구
        int prevNum = currentIdx + 1;
        gotoxy(orbit[currentIdx].x, orbit[currentIdx].y);
        SetNumberColor(prevNum);
        cout << prevNum;

        // 2. 다음 칸 이동
        currentIdx = (currentIdx + 1) % orbit.size();

        // 3. 새 위치에 구슬 표시
        gotoxy(orbit[currentIdx].x, orbit[currentIdx].y);
        SetColor(14); cout << "O "; // 구슬은 노란색

        // 4. 중앙 전광판 숫자 실시간 동기화 (핵심 수정)
        int displayNum = currentIdx + 1; // 현재 구슬이 있는 칸의 번호
        gotoxy(25, 8);
        SetNumberColor(displayNum);    // 전광판 숫자 색상도 홀짝에 맞춤
        cout << setw(2) << displayNum;

        // 5. 속도 조절 (점점 느려지는 연출)
        if (totalSteps - i < 15) {
            Sleep(50 + (15 - (totalSteps - i)) * 25);
        }
        else {
            Sleep(20);
        }
    }

    // 최종 결과 확정
    int finalNumber = currentIdx + 1;

    // --- 최종 결과 출력 ---
    gotoxy(0, 15);
    cout << "\n-----------------------------------------------" << endl;
    SetColor(14); cout << "최종 결과: ";
    SetNumberColor(finalNumber); cout << "[" << finalNumber << "]";
    SetColor(14); cout << " -> " << (finalNumber % 2 == 0 ? "짝수" : "홀수") << endl;

    // 승패 판정
    bool win = (choice == '1' && finalNumber % 2 != 0) || (choice == '2' && finalNumber % 2 == 0);

    if (win) {
        SetColor(10); // 초록색
        cout << ">>> 승리! +$" << bet << " <<<" << endl;
        money += bet;
    }
    else {
        SetColor(12); // 빨간색
        cout << ">>> 패배! -$" << bet << " <<<" << endl;
        money -= bet;
    }

    SetColor(15);
    cout << "현재 보유 자산: $" << money << endl;

    ClearBuffer();
}