#include "elsfk.h"
#include "resource.h"
#define  ID_TIMER 1                                         // 定义计时器 ID


// 由于第一次用Windows编程，所以对一些函数都写了注释，篇幅较长，请见谅。
// 代码是用C写的，若有拙劣BUG之处，还请见谅。

// 窗口全局变量

HINSTANCE   hInst;                                          // 当前实例
BOOL        rightover = TRUE,
            upover    = FALSE,
            downover  = FALSE;
TCHAR		szTitle[] = TEXT("俄罗斯方块游戏"),             // 标题栏文本
            szWindowName[] = TEXT("elsfk");                 // 主窗口类名

int         score1 = 0,                                     // 定义分数           
            level = 0,                                      // 难度等级
	        goal = 0,                                       // 中间变量
            timegap = 500;                                  // 定义时间间隔

TCHAR       szBuffer[128];                                  // 定义字符缓冲区
size_t      iTarget;                   
POINT       Bpoint;                                         // 边框左上角坐标集
CONST INT   XWIDTH = 681,                                   // 定义窗口宽度
            YHEIGHT = 400,                                  // 定义窗口高度
            sXWIDTH = 425,                                  // 定义窗口起始X坐标
            sYHEIGHT = 200;                                 // 定义窗口起始Y坐标


// 前向申明

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK    DlgMain(HWND, UINT, WPARAM, LPARAM);
BOOL                InitInstance(HINSTANCE, int);
void                ShowTip(HDC hdc);
void                OutPaint(hdc);
void                OutUp(HWND hWnd);
void                OutDown(HWND hWnd);
void                OutLeft(HWND hWnd);
void                OutRight(HWND hWnd);

// WIN32主窗口创建

int APIENTRY wWinMain(          HINSTANCE hInstance,          // 应用程序实例句柄
	                            HINSTANCE hPrevInstance,      // 应用程序前一个实例句柄
                   	            LPWSTR    lpCmdLine,          // 命令行参数
	                            int       nCmdShow)           // 窗口的显示方式 
{
	UNREFERENCED_PARAMETER(hPrevInstance);                   // 未使用的参数要求,免警告申明
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEXW wcex;                                        // 定义窗口类结构体

	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;                              // 定义窗口风格
	wcex.lpfnWndProc   = WndProc;                                              // 指定窗口过程(回调函数) 
	wcex.cbClsExtra    = 0;                                                    // 窗口类额外扩展内存空间大小
	wcex.cbWndExtra    = 0;                                                    // 窗口的额外扩展空间大小
	wcex.hInstance     = hInstance;                                            // 应用窗口实例句柄
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));      // 设置图标
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);                          // 给所有基于该窗口类设定一个鼠标指针
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);                  // 窗口颜色(黑色画刷实现)
	wcex.lpszMenuName  = NULL;                                                 // 定义菜单名
	wcex.lpszClassName = szWindowName;                                         // 定义窗口类名
	wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1)); // 设置窗口左上角图标

	RegisterClassExW(&wcex);                                                   // 注册窗口

	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgMain);

	MSG msg;

	// 主消息循环: 

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);                                                  //将消息给操作系统，由操作系统调回窗口过程
	}
	return (int)msg.wParam;
}


//   保存实例句柄并创建主窗口

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;                                                           // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowName,                                      // 窗口类名
		                      szTitle,                                           // 窗口标题名
		                      WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,       // 窗口风格
		                      sXWIDTH,                                           // 窗口起始X
		                      sYHEIGHT,                                          // 窗口起始Y
		                      XWIDTH,                                            // 窗口宽度 
		                      YHEIGHT,                                           // 窗口高度
		                      NULL,                                              // 父句柄
		                      NULL,                                              // 菜单句柄
		                      hInstance,                                         // 与窗口相关的模块实例句柄
		                      NULL);                                             // 值指针，传递给窗口WM_CREATE消息
	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);                                                  // 显示窗口
	UpdateWindow(hWnd);                                                          // 更新窗口
	
	return TRUE;
}

// 处理DialogBox的回调函数

BOOL CALLBACK DlgMain(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_COMMAND:
	{
	case IDOK:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	}
	}
	return FALSE;
}

// 处理反馈在主窗口上的消息。

LRESULT CALLBACK WndProc(HWND   hWnd,                                            // 窗口句柄
	                     UINT   message,                                         // 消息编号 
	                     WPARAM wParam,                                          // 附加参数 
	                     LPARAM lParam)                                          // 附加参数
{
	switch (message)
	{
	case WM_CREATE:                                                              // 初始化数据
	{
		// PlaySound(TEXT("俄罗斯方块.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);             // 播放音乐
		return 0;
	}

	case WM_PAINT:                                                               // WM_PAINT 绘制主窗口
	{
		PAINTSTRUCT ps;                                                          // 保存绘图数据
		HDC hdc = BeginPaint(hWnd, &ps);                                         // 定义并得到设备hdc 	
		HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));                   // 定义画笔对象
		SelectObject(hdc, pen);                                                  // 选择对象

		ShowTip(hdc);                                                            // 显示当前分数
		preorder = ShowPreOrder();                                               // 生成预先方块序列
		Boundary(hdc);                                                           // 绘制边界方框

		DeleteObject(pen);                                                       // 删除画笔对象
		EndPaint(hWnd, &ps);                                                     // 结束绘制
	}
	break;

	case WM_TIMER:
	{
		OutTimer(hWnd);
	}
	break;

	case WM_KEYDOWN:
		switch (wParam)                                                           // 为了增加可读性，写个外置函数表示
		{
		case VK_UP:
			OutUp(hWnd); 
			break;
		case VK_DOWN:
			OutDown(hWnd);
			break;
		case VK_LEFT:
			OutLeft(hWnd);
			break;
		case VK_RIGHT:
			OutRight(hWnd);
			break;
		default:
			break;
		}

	case WM_CHAR:
	{
		switch (wParam)
		{
		case 's':                                                                  // 开始游戏
			SetTimer(hWnd, ID_TIMER, timegap, NULL);
			break;
		case 'a':                                                                  // 暂停游戏
			KillTimer(hWnd, ID_TIMER);
			break;
		default:
			break;
		}
	}
		return 0;
	case WM_CLOSE:                                                                
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:                                                              // WM_DESTROY 发送退出消息并返回
		PostQuitMessage(0);
		KillTimer(hWnd, ID_TIMER);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);                      // 窗口处理函数
	}
	return 0;
}

// 创建兼容性DC实现窗口无闪烁
void OutPaint(HDC hdc)
{
	HDC HMemDc = CreateCompatibleDC(hdc);                                         // 创建兼容性DC

	HBITMAP hBitmapBack = CreateCompatibleBitmap(hdc, 681, 400);                  // 创造一张纸

	SelectObject(HMemDc, hBitmapBack);                                            // 选择当前句柄

	Boundary(HMemDc);
	score1 = ClearRow();                                                          // 消行处理，并返回分数
	ShowTip(HMemDc);                                                              // 显示窗口文本信息
	if (rightover)
	{
		order = ShowOrder(preorder);                                              // 右方到底后生成方块
		preorder = ShowPreOrder();                                                // 显示下一个方块的序列
		rightover = FALSE;
	}
	ShowPreSquare(HMemDc);                                                        // 显示预览区方块
    ShowSquare(HMemDc);                                                           // 显示游戏区方块

	BitBlt(hdc, 0, 0, XWIDTH, YHEIGHT, HMemDc, 0, 0, SRCCOPY);                    // 在内存预先绘制好后传递

	DeleteObject(hBitmapBack);                                                    // 删除位图

	DeleteDC(HMemDc);                                                             // 删除句柄
}

// 游戏主循环
void OutTimer(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	MoveRightSqare();                                                            // 自动向右
	if (ReachRightBlock())                                                       // 右方到底后加分处理，以及难度处理
	{
		rightover = TRUE;
		if (score1 % 20 == 0 && timegap > 100 && score1 != goal) 
		{
			level = score1 / 20;
			timegap -= 10;
			goal += score1;
			KillTimer(hWnd, ID_TIMER);
			SetTimer(hWnd, ID_TIMER, timegap, NULL);
		}
	}
	OutPaint(hdc);                                                                // 绘制界面
	ReleaseDC(hWnd, hdc);
	if (AllRow())                                                                 // 满行后停止游戏
	{
		KillTimer(hWnd, ID_TIMER);
		UINT nRet = MessageBox(NULL, TEXT("是否重玩"), TEXT("Game Over"), MB_OKCANCEL);
		if (nRet == IDOK)
		{
			AllClear();
			rightover = TRUE;
			OutPaint(hdc);
			preorder = ShowPreOrder();
			SetTimer(hWnd, ID_TIMER, timegap, NULL);
		}
		else if(nRet == IDCANCEL)
		{
			DestroyWindow(hWnd);
		}
	}
}

// 显示窗口文本信息
void ShowTip(HDC HMemDc)
{
	StringCchPrintf(szBuffer, 128, TEXT("%s: %d"), TEXT("当前分数"), score1);
	StringCchLength(szBuffer, 128, &iTarget);
	TextOut(HMemDc, 250, 250, szBuffer, iTarget);
	StringCchPrintf(szBuffer, 128, TEXT("%s: %d"), TEXT("当前难度"), level);
	StringCchLength(szBuffer, 128, &iTarget);
	TextOut(HMemDc, 250, 300, szBuffer, iTarget);
	StringCchPrintf(szBuffer, 128, TEXT("%s"), TEXT("游戏帮助"));
	StringCchLength(szBuffer, 128, &iTarget);
	TextOut(HMemDc, 400, 250, szBuffer, iTarget);
	StringCchPrintf(szBuffer, 128, TEXT("%s"), TEXT("s：开始游戏 a：暂停游戏"));
	StringCchLength(szBuffer, 128, &iTarget);
	TextOut(HMemDc, 400, 280, szBuffer, iTarget);
	StringCchPrintf(szBuffer, 128, TEXT("%s"), TEXT("加速-> 旋转<- 上移up键 下移down键"));
	StringCchLength(szBuffer, 128, &iTarget);
	TextOut(HMemDc, 400, 310, szBuffer, iTarget);
}

// 向上函数
void OutUp(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	if (ReachUpBlock())                                                           // 判断是否可以上移
		upover = TRUE;
	else
		upover = FALSE;
	if (!upover)
		UpMove();
	OutPaint(hdc);
	ReleaseDC(hWnd, hdc);
}

// 向上函数
void OutDown(HWND hWnd)                                                           // 判断是否可以上移
{
	HDC hdc = GetDC(hWnd);
	if (ReachDowmBlock())
		downover = TRUE;
	else
		downover = FALSE;
	if(!downover)
	    DownMove();
	OutPaint(hdc);
	ReleaseDC(hWnd, hdc);
}

// 旋转函数
void OutLeft(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);

	if (order == 0)
	{
		RevolveRectan();
	}
	else if (order == 2)                                                          // 正方块旋转就是本身，不用旋转
	{
	}                
	else
		RevolveSqare();
	   
	OutPaint(hdc);
	ReleaseDC(hWnd, hdc);
}

// 加速函数
void OutRight(HWND hWnd)
{
	OutTimer(hWnd);                                                               // 就是向WM_TIMER人为发送消息
}