#include "System.h"
using namespace std;

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
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    cout << "\n[아무 키나 누르면 메뉴로 돌아갑니다]";
    _getch();
}