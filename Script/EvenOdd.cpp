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
    // [연동] 게임 진입 전 파산 체크 (입장 컷)
    if (CheckBankruptcy("당신", money, 1, true)) return;

    while (true) {
        random_device rd; mt19937 gen(rd());

        system("cls");
        SetColor(14);
        cout << "===============================================" << endl;
        cout << "              [ 3. EVEN OR ODD ]" << endl;
        cout << "===============================================" << endl;

        // 1. 베팅 (보유 자산 표시 및 베팅금 입력)
        int bet = GetBetAmount(money);
        if (bet <= 0) return;

        // 베팅 금액 선차감
        money -= bet;
        FlushBuffer();

        // 2. 홀/짝 선택
        SetColor(15);
        cout << "\n 어느 쪽을 선택하시겠습니까?" << endl;
        cout << " [1] 홀수(Odd)  [2] 짝수(Even) : ";
        char choice;
        while (true) {
            choice = (char)_getch();
            if (choice == '1' || choice == '2') break;
        }
        cout << (choice == '1' ? "홀수" : "짝수") << endl;
        Sleep(500);

        // 3. 36칸 궤도 좌표 생성
        vector<Point> orbit;
        for (int x = 10; x <= 40; x += 3) orbit.push_back({ x, 4 });
        for (int y = 5; y <= 12; y += 1) orbit.push_back({ 40, y });
        for (int x = 40; x >= 10; x -= 3) orbit.push_back({ x, 13 });
        for (int y = 12; y >= 5; y -= 1) orbit.push_back({ 10, y });

        // 4. 판 그리기
        system("cls");
        for (int i = 0; i < (int)orbit.size(); i++) {
            int num = i + 1;
            gotoxy(orbit[i].x, orbit[i].y);
            SetNumberColor(num);
            cout << num;
        }
        gotoxy(23, 8); SetColor(14); cout << "[    ]";

        int currentIdx = 0;
        int totalSteps = 60 + (gen() % 30);

        // 5. 구슬 회전 연출
        for (int i = 0; i < totalSteps; i++) {
            int prevNum = currentIdx + 1;
            gotoxy(orbit[currentIdx].x, orbit[currentIdx].y);
            SetNumberColor(prevNum);
            cout << (prevNum < 10 ? " " : "") << prevNum;

            currentIdx = (currentIdx + 1) % (int)orbit.size();

            gotoxy(orbit[currentIdx].x, orbit[currentIdx].y);
            SetColor(14); cout << "O ";

            int displayNum = currentIdx + 1;
            gotoxy(25, 8);
            SetNumberColor(displayNum);
            cout << setw(2) << displayNum;

            if (totalSteps - i < 15) {
                Sleep(50 + (15 - (totalSteps - i)) * 35);
            }
            else {
                Sleep(25);
            }
        }

        // 6. 결과 판정
        int finalNumber = currentIdx + 1;
        bool win = (choice == '1' && finalNumber % 2 != 0) || (choice == '2' && finalNumber % 2 == 0);

        gotoxy(0, 15);
        SetColor(14); cout << " 최종 결과: ";
        SetNumberColor(finalNumber); cout << "[" << finalNumber << "]";
        SetColor(14); cout << " -> " << (finalNumber % 2 == 0 ? "짝수(EVEN)" : "홀수(ODD)") << endl;

        int winAmount = (win) ? bet * 2 : 0;

        // [연동] 공통 정산 함수 호출
        PrintResult(money, bet, winAmount);

        // [연동] 결과 후 파산 체크 (0원이면 즉시 광산행 연출 시작)
        if (CheckBankruptcy("당신", money, 1, true)) return;

        SetColor(15);
        cout << "\n [1] 계속하기 [0] 나가기 : ";
        int exitChoice;
        cin >> exitChoice;
        if (exitChoice == 0) break;
    }

    ClearBuffer();
}