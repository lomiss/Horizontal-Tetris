#include "elsfk.h"
#define     ID_TIMER 1                                         // 定义计时器ID

// 游戏全局变量

int         map_point[10][30] = { 0 };                         // 定义二维地图坐标
int         pre_point[4][4]   = { 0 };                         // 定义预览器二维地图坐标
int         temp_point[10][30]  = { 0 };                       // 定义临时坐标
int         score = 0;
POINT       Bpoint;                                            // 边框左上角坐标集
CONST INT   BorLen    = 5,                                     // 边框边长
            BorLenGap = 7,                                     // 边框间距
            LenGap    = 22,                                    // 方框间距
            Length    = 20;                                    // 方框边长

// 生成预览区随机方块序列
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
	case 0:   // 长方形
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[2][0] = 1, pre_point[3][0] = 1;
		break;
	}
	case 1:   // 土字形
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[2][0] = 1, pre_point[1][1] = 1;
		break;
	}
	case 2:   // 正方形
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[0][1] = 1, pre_point[1][1] = 1;
		break;
	}
	case 3:   // Z字形
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[1][1] = 1, pre_point[2][0] = 1;
		break;
	}
	case 4:   // 反Z子形
	{
		pre_point[0][1] = 1, pre_point[1][1] = 1, pre_point[1][0] = 1, pre_point[2][0] = 1;
		break;
	}
	case 5:   // 直角形
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[2][0] = 1, pre_point[0][1] = 1;
		break;
	}
	case 6:   // 反直角形
	{
		pre_point[0][0] = 1, pre_point[1][0] = 1, pre_point[2][0] = 1, pre_point[2][1] = 1;
		break;
	}
	default:
		break;
	}
	return sort;
}

// 生成游戏区随机方块序列
int ShowOrder(int sort)
{
	switch (sort)
	{
	case 0:   // 长方形
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[6][0] = 1, map_point[7][0] = 1;
		temp_point[5][0] = 1;
		break;
	}
	case 1:   // 土字形
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[6][0] = 1, map_point[5][1] = 1;
		temp_point[5][0] = 1;
		break;
	}
	case 2:   // 正方形
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[4][1] = 1, map_point[5][1] = 1;
		temp_point[5][0] = 1;
		break;
	}
	case 3:   // Z字形
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[5][1] = 1, map_point[6][0] = 1;
		temp_point[5][1] = 1;
		break;
	}
	case 4:   // 反Z子形
	{
		map_point[4][1] = 1, map_point[5][1] = 1, map_point[5][0] = 1, map_point[6][0] = 1;
		temp_point[5][1] = 1;
		break;
	}
	case 5:   // 直角形
	{
		map_point[4][0] = 1, map_point[5][0] = 1, map_point[6][0] = 1, map_point[4][1] = 1;
		temp_point[5][0] = 1;
		break;
	}
	case 6:   // 反直角形
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

// 显示预览区方块
void ShowPreSquare(HDC HMemDc)
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));                         //白色边框
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

// 显示游戏区方块
void ShowSquare(HDC HMemDc)
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));                         //白色边框
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

// 画正方形框
void DrawSquare(HDC hdc, int x,int y)                                // 画单个方块
{
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x + Length, y);
	LineTo(hdc, x + Length, y + Length);
	LineTo(hdc, x, y + Length);
	LineTo(hdc, x, y);
}

// 画边框矩形
void DrawBouSquare(HDC hdc, POINT point, int Length)                   //画边界方块
{
	MoveToEx(hdc, point.x, point.y, NULL);
	LineTo(hdc, point.x + Length, point.y);
	LineTo(hdc, point.x + Length, point.y + Length);
	LineTo(hdc, point.x, point.y + Length);
	LineTo(hdc, point.x, point.y);
}

// 生成边框矩形坐标
void Boundary(HDC HMemDc)
{
	int i;
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));              //初始化边界方块坐标值
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

// 受"重力"向"右"
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

// 向上移动
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

// 向下移动
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

// 移动方块"变成"静止方块
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

// 到达右端判断
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

// 到达顶部判断
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

// 到达底端判断
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

// 九格方块旋转
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

// 长方块旋转
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

// 消行处理
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

// 满行处理
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

// 清空坐标数据重新游戏
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