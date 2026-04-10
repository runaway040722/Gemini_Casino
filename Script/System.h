#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <string>

// 전역 변수 공유
extern std::vector<std::string> globalNamePool;

// 시스템 공통 함수
void gotoxy(int x, int y);
void SetColor(int color);
void FlushBuffer();
void ClearBuffer();

int GetBetAmount(int currentMoney);

void PrintResult(int& money, int bet, int winAmount);

#endif