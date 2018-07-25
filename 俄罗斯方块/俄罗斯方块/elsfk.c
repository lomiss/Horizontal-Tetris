#include "elsfk.h"
#define     ID_TIMER 1                                         // �����ʱ��ID

// ��Ϸȫ�ֱ���

int         map_point[10][30] = { 0 };                         // �����ά��ͼ����
int         pre_point[4][4]   = { 0 };                         // ����Ԥ������ά��ͼ����
int         temp_point[10][30]  = { 0 };                       // ������ʱ����
int         score = 0;
POINT       Bpoint;                                            // �߿����Ͻ����꼯
CONST INT   BorLen    = 5,                                     // �߿�߳�
            BorLenGap = 7,                                     // �߿���
            LenGap    = 22,                                    // ������
            Length    = 20;                                    // ����߳�

// ����Ԥ���������������
int ShowPreOrder()
{
	int i, j, sort;
	srand((unsigned int)time(NULL));
	sort = rand() % 7;
	for (i = 0; i <= 3; i++)
	{
		for (j = 0; j <= 3; j++)
		{
			pre_point[i][j] = 0;
		}
	}
	switch (sort)
	{
	case 0:   // ������
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[2][0] = 1, pre_point[3][0] = 1;
		break;
	}
	case 1:   // ������
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[2][0] = 1, pre_point[1][1] = 1;
		break;
	}
	case 2:   // ������
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[0][1] = 1, pre_point[1][1] = 1;
		break;
	}
	case 3:   // Z����
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[1][1] = 1, pre_point[2][0] = 1;
		break;
	}
	case 4:   // ��Z����
	{
		pre_point[0][1] = 1, pre_point[1][1] = 1, pre_point[1][0] = 1, pre_point[2][0] = 1;
		break;
	}
	case 5:   // ֱ����
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[2][0] = 1, pre_point[0][1] = 1;
		break;
	}
	case 6:   // ��ֱ����
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[2][0] = 1, pre_point[2][1] = 1;
		break;
	}
	default:
		break;
	}
	return sort;
}

// ������Ϸ�������������
int ShowOrder(int sort)
{
	switch (sort)
	{
	case 0:   // ������
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[6][0] = 1, map_point[7][0] = 1;
		temp_point[5][0] = 1;
		break;
	}
	case 1:   // ������
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[6][0] = 1, map_point[5][1] = 1;
		temp_point[5][0] = 1;
		break;
	}
	case 2:   // ������
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[4][1] = 1, map_point[5][1] = 1;
		temp_point[5][0] = 1;
		break;
	}
	case 3:   // Z����
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[5][1] = 1, map_point[6][0] = 1;
		temp_point[5][1] = 1;
		break;
	}
	case 4:   // ��Z����
	{
		map_point[4][1] = 1, map_point[5][1] = 1, map_point[5][0] = 1, map_point[6][0] = 1;
		temp_point[5][1] = 1;
		break;
	}
	case 5:   // ֱ����
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[6][0] = 1, map_point[4][1] = 1;
		temp_point[5][0] = 1;
		break;
	}
	case 6:   // ��ֱ����
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[6][0] = 1, map_point[6][1] = 1;
		temp_point[5][0] = 1;
		break;
	}
	default:
		break;
	}
	return sort;
}

// ��ʾԤ��������
void ShowPreSquare(HDC HMemDc)
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));                         //��ɫ�߿�
	SelectObject(HMemDc, pen);
	int i, j;
	for (i = 0; i <= 3; i++)
	{
		for (j = 0; j <= 3; j++)
		{
			if (pre_point[i][j] == 1)
			{
				DrawSquare(HMemDc, (j + 3) * LenGap, (i + 12) * LenGap);
			}
		}
	}
	DeleteObject(pen);
}                     

// ��ʾ��Ϸ������
void ShowSquare(HDC HMemDc)
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));                         //��ɫ�߿�
	SelectObject(HMemDc, pen);
	int i, j;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 30; j++)
		{
			if (map_point[i][j] == 1 || map_point[i][j] == 2)
			{
				DrawSquare(HMemDc, j * LenGap, 7 + i * LenGap);
			}
		}
	}
	DeleteObject(pen);
}

// �������ο�
void DrawSquare(HDC hdc, int x,int y)                                // ����������
{
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x + Length, y);
	LineTo(hdc, x + Length, y + Length);
	LineTo(hdc, x, y + Length);
	LineTo(hdc, x, y);
}

// ���߿����
void DrawBouSquare(HDC hdc, POINT point, int Length)                   //���߽緽��
{
	MoveToEx(hdc, point.x, point.y, NULL);
	LineTo(hdc, point.x + Length, point.y);
	LineTo(hdc, point.x + Length, point.y + Length);
	LineTo(hdc, point.x, point.y + Length);
	LineTo(hdc, point.x, point.y);
}

// ���ɱ߿��������
void Boundary(HDC HMemDc)
{
	int i;
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));              //��ʼ���߽緽������ֵ
	SelectObject(HMemDc, pen);
	for (i = 2; i < 662; i += BorLenGap)
	{
		Bpoint.x = i;
		Bpoint.y = 0;
		DrawBouSquare(HMemDc, Bpoint, BorLen);
		Bpoint.x = i;
		Bpoint.y = 227;
		DrawBouSquare(HMemDc, Bpoint, BorLen);
	}
	for (i = 7; i < 227; i += BorLenGap)
	{
		Bpoint.x = 660;
		Bpoint.y = i;
		DrawBouSquare(HMemDc, Bpoint, BorLen);
	}
	DeleteObject(pen);
}

// ��"����"��"��"
void MoveRightSqare()
{
	int i, j;
	for (j = 29; j >= 0; j--)
	{
		for (i = 0; i <= 9; i++)
		{
			if (map_point[i][j] == 1)
			{
				map_point[i][j + 1] = map_point[i][j];
				map_point[i][j] = 0;
			}
			if (temp_point[i][j] == 1)
			{
				temp_point[i][j + 1] = temp_point[i][j];
				temp_point[i][j] = 0;
			}
		}
	}
}

// �����ƶ�
void UpMove()
{
	int i, j;
	for (i = 0; i <= 9; i++)
	{
		for (j = 0; j <= 29; j++)
		{
			if (map_point[i][j] == 1)
			{
				map_point[i-1][j] = map_point[i][j];
				map_point[i][j] = 0;
			}
			if (temp_point[i][j] == 1)
			{
				temp_point[i - 1][j] = temp_point[i][j];
				temp_point[i][j] = 0;
			}
		}
	}
}

// �����ƶ�
void DownMove()
{
	int i, j;
	for (i = 9; i >= 0; i--)
	{
		for (j = 0; j <= 29; j++)
		{
			if (map_point[i][j] == 1)
			{
				map_point[i + 1][j] = map_point[i][j];
				map_point[i][j] = 0;
			}
			if (temp_point[i][j] == 1)
			{
				temp_point[i + 1][j] = temp_point[i][j];
				temp_point[i][j] = 0;
			}
		}
	}
}

// �ƶ�����"���"��ֹ����
void Transtill()
{
	int i, j;
	for (j = 29; j >= 0; j--)
	{
		for (i = 0; i <= 9; i++)
		{
			if (map_point[i][j] == 1)
			{
				map_point[i][j] = 2;
			}
			if (temp_point[i][j] == 1)
			{
				temp_point[i][j] = 0;
			}
		}
	}
}

// �����Ҷ��ж�
int ReachRightBlock()
{
	int i, j, flag = 0;
	for (i = 0; i < 10; i++)
	{
		if (map_point[i][29] == 1)
		{
			flag = 1;
		}
	}
	for (j = 29; j >= 0; j--)
	{
		for (i = 0; i <= 9; i++)
		{
			if (map_point[i][j + 1] == 2 && map_point[i][j] == 1)
			{
				flag = 1;
			}
		}
	}
	if (flag)
	{
		Transtill();
		return 1;
	}
	else return 0;
}

// ���ﶥ���ж�
int ReachUpBlock()
{
	int i, j, flag = 0;
	for (j = 0; j < 30; j++)
	{
		if (map_point[0][j] == 1)
		{
			flag = 1;
		}
	}
	for (i = 1; i <= 9; i++)
	{
		for (j = 0; j <= 29; j++)
		{
			if (map_point[i-1][j] == 2 && map_point[i][j] == 1)
			{
				flag = 1;
			}
		}
	}
	if (flag)
		return 1;
	else return 0;



}

// ����׶��ж�
int ReachDowmBlock()
{
	int i, j, flag = 0;
	for (j = 0; j < 30; j++)
	{
		if (map_point[9][j] == 1)
		{
			flag = 1;
		}
	}
	for (i = 8; i >= 0; i--)
	{
		for (j = 0; j <= 29; j++)
		{
			if (map_point[i + 1][j] == 2 && map_point[i][j] == 1)
			{
				flag = 1;
			}
		}
	}
	if (flag)
		return 1;
	else return 0;
}

// �Ÿ񷽿���ת
void RevolveSqare()
{
	int i, j, temp;
	for (i = 1; i <= 8; i++)
	{
		for (j = 28; j >= 1; j--)
		{
			if (temp_point[i][j] == 1 && map_point[i][j + 1] != 2 && map_point[i + 1][j] != 2 && 
				map_point[i - 1][j] != 2 && map_point[i - 1][j + 1] != 2 && map_point[i + 1][j + 1] != 2)
			{
				temp = map_point[i - 1][j];
				map_point[i - 1][j] = map_point[i][j + 1];
				map_point[i][j + 1] = map_point[i + 1][j];
				map_point[i + 1][j] = map_point[i][j - 1];
				map_point[i][j - 1] = temp;

				temp = map_point[i - 1][j - 1];
				map_point[i - 1][j - 1] = map_point[i - 1][j + 1];
				map_point[i - 1][j + 1] = map_point[i + 1][j + 1];
				map_point[i + 1][j + 1] = map_point[i + 1][j - 1];
				map_point[i + 1][j - 1] = temp;
			}
		}
	}
}

// ��������ת
void RevolveRectan()
{
	int i, j, temp;
	for (i = 2; i <= 7; i++)
	{
		for (j = 28; j >= 1; j--)
		{
			if (temp_point[i][j] == 1 && map_point[i][j + 2] != 2 && map_point[i + 2][j] != 2 && 
				map_point[i - 1][j] != 2 && map_point[i - 1][j + 1] != 2 && map_point[i - 1][j + 2] != 2 &&
				map_point[i + 1][j + 1] != 2 && map_point[i + 1][j + 2] != 2 && map_point[i + 2][j + 1] != 2 &&
				map_point[i + 2][j + 2] != 2)
			{
				temp = map_point[i - 1][j];
				map_point[i - 1][j] = map_point[i][j - 1];
				map_point[i][j - 1] = temp;

				temp = map_point[i][j + 1];
				map_point[i][j + 1] = map_point[i + 1][j];
				map_point[i + 1][j] = temp;

				temp = map_point[i][j + 2];
				map_point[i][j + 2] = map_point[i + 2][j];
				map_point[i + 2][j] = temp;
	
			}
		}
	}
}

// ���д���
int ClearRow()
{
	int i, j = 29, cnt = 0;
	int temp_j, temp1_j;
	while(j)
	{
		for (i = 0; i <= 9; i++)
		{
			if (map_point[i][j] == 2)
				cnt++;
		}
		if (cnt == 10)
		{
			temp1_j = j - 1;
			for (temp_j = j - 1; temp_j >= 0; temp_j--)
			{
				for (i = 0; i <= 9; i++)
				{
					map_point[i][temp_j + 1] = map_point[i][temp_j];
					map_point[i][temp_j] = 0;
				}
			}
			score += 10;
			j = temp1_j + 1;
		}
		else j--;
		cnt = 0;
	}
	return score;
}

// ���д���
int AllRow()
{
	int i;
	for (i = 0; i <= 9; i++)
	{
		if (map_point[i][2] == 2)
			return 1;
	}
	return 0;
}

// �����������������Ϸ
void AllClear()
{
	int i, j;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 30; j++)
		{
			map_point[i][j] = 0;
		}
	}
}