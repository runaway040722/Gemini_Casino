#include "System.h"
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <iostream>

using namespace std;

// 40인 전체 명단으로 교체
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
    // numeric_limits 사용을 위해 <limits> 헤더가 필요할 수 있음
    cin.ignore(1000, '\n');
    cout << "\n[아무 키나 누르면 메뉴로 돌아갑니다]";
    _getch();
}