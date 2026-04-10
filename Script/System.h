#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

void SetColor(int color);
void gotoxy(int x, int y);
void ClearBuffer();
void FlushBuffer();
int GetBetAmount(int currentMoney);
void PrintResult(int& money, int bet, int winAmount);

// [통일] DragToMine과 CheckBankruptcy 모두 const std::string& 를 사용하도록 맞춥니다.
void DragToMine(const std::string& name, bool isPlayer);
bool CheckBankruptcy(const std::string& name, int& money, int threshold, bool isPlayer);

void PrintActionLog(std::string name, std::string action, int color);

extern std::vector<std::string> globalNamePool;

#endif