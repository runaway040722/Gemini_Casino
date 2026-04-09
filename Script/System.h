#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <random>
#include <conio.h>
#include <iomanip>
#include <limits>

// 함수 선언 (이 도구들을 사용할 거라고 컴파일러에게 알림)
void gotoxy(int x, int y);
void SetColor(int color);
void FlushBuffer(); 
void ClearBuffer(); 

#endif