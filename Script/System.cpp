#include "System.h"

using namespace std; // cout, endl, cin 에러 해결

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void FlushBuffer() {
    // 1. 키보드 버퍼 청소 (기다리지 않고 현재 쌓인 키만 모두 삭제)
    while (_kbhit()) _getch();

    // 2. cin 스트림 상태 복구 및 잔여 데이터 제거
    cin.clear();

}

void ClearBuffer() {
    // 1. 게임 중에 사용자가 미친듯이 누른 엔터/글자들을 여기서 싹 비움
    while (_kbhit()) _getch(); // 키보드 버퍼에 남은 입력들 강제 소진
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');

    // 2. 이제야 깨끗해진 상태에서 사용자의 진짜 입력을 기다림
    cout << "\n[아무 키나 누르면 메뉴로 돌아갑니다]";
    _getch();
}