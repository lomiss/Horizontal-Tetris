#pragma once
#pragma comment(lib,"winmm.lib")        // ��������֧�ֵĿ�

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include "strsafe.h"
#include <mmsystem.h>                   // PlaySound������ͷ�ļ�

// ��Ϸ�߼���ǰ������

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