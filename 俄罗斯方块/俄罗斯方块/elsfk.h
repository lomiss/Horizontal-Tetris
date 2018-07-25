#pragma once
#pragma comment(lib,"winmm.lib")        // 播放音乐支持的库

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include "strsafe.h"
#include <mmsystem.h>                   // PlaySound函数的头文件

// 游戏逻辑层前向声明

int  preorder, order;
void DrawBouSquare(HDC hdc, POINT point, int Length);
void DrawSquare(HDC hdc, int x, int y);
void Boundary(HDC HMemDc);
void ShowSquare(HDC HMemDc);
void ShowPreSquare(HDC HMemDc);
void MoveRightSqare();
void OutTimer(HWND hWnd);
int ReachRightBlock();
void Transtill();
void UpMove();
void DownMove();
int ReachUpBlock();
int ReachDowmBlock();
void RevolveSqare();
void RevolveRectan();
int ShowOrder(int order);
int ShowPreOrder();
int ClearRow();
int AllRow();
void AllClear();