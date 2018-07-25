#include "elsfk.h"
#include "resource.h"
#define  ID_TIMER 1                                         // �����ʱ�� ID


// ���ڵ�һ����Windows��̣����Զ�һЩ������д��ע�ͣ�ƪ���ϳ�������¡�
// ��������Cд�ģ�����׾��BUG֮����������¡�

// ����ȫ�ֱ���

HINSTANCE   hInst;                                          // ��ǰʵ��
BOOL        rightover = TRUE,
            upover    = FALSE,
            downover  = FALSE;
TCHAR		szTitle[] = TEXT("����˹������Ϸ"),             // �������ı�
            szWindowName[] = TEXT("elsfk");                 // ����������

int         score1 = 0,                                     // �������           
            level = 0,                                      // �Ѷȵȼ�
	        goal = 0,                                       // �м����
            timegap = 500;                                  // ����ʱ����

TCHAR       szBuffer[128];                                  // �����ַ�������
size_t      iTarget;                   
POINT       Bpoint;                                         // �߿����Ͻ����꼯
CONST INT   XWIDTH = 681,                                   // ���崰�ڿ��
            YHEIGHT = 400,                                  // ���崰�ڸ߶�
            sXWIDTH = 425,                                  // ���崰����ʼX����
            sYHEIGHT = 200;                                 // ���崰����ʼY����


// ǰ������

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK    DlgMain(HWND, UINT, WPARAM, LPARAM);
BOOL                InitInstance(HINSTANCE, int);
void                ShowTip(HDC hdc);
void                OutPaint(hdc);
void                OutUp(HWND hWnd);
void                OutDown(HWND hWnd);
void                OutLeft(HWND hWnd);
void                OutRight(HWND hWnd);

// WIN32�����ڴ���

int APIENTRY wWinMain(          HINSTANCE hInstance,          // Ӧ�ó���ʵ�����
	                            HINSTANCE hPrevInstance,      // Ӧ�ó���ǰһ��ʵ�����
                   	            LPWSTR    lpCmdLine,          // �����в���
	                            int       nCmdShow)           // ���ڵ���ʾ��ʽ 
{
	UNREFERENCED_PARAMETER(hPrevInstance);                   // δʹ�õĲ���Ҫ��,�⾯������
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEXW wcex;                                        // ���崰����ṹ��

	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;                              // ���崰�ڷ��
	wcex.lpfnWndProc   = WndProc;                                              // ָ�����ڹ���(�ص�����) 
	wcex.cbClsExtra    = 0;                                                    // �����������չ�ڴ�ռ��С
	wcex.cbWndExtra    = 0;                                                    // ���ڵĶ�����չ�ռ��С
	wcex.hInstance     = hInstance;                                            // Ӧ�ô���ʵ�����
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));      // ����ͼ��
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);                          // �����л��ڸô������趨һ�����ָ��
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);                  // ������ɫ(��ɫ��ˢʵ��)
	wcex.lpszMenuName  = NULL;                                                 // ����˵���
	wcex.lpszClassName = szWindowName;                                         // ���崰������
	wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1)); // ���ô������Ͻ�ͼ��

	RegisterClassExW(&wcex);                                                   // ע�ᴰ��

	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgMain);

	MSG msg;

	// ����Ϣѭ��: 

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);                                                  //����Ϣ������ϵͳ���ɲ���ϵͳ���ش��ڹ���
	}
	return (int)msg.wParam;
}


//   ����ʵ�����������������

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;                                                           // ��ʵ������洢��ȫ�ֱ�����

	HWND hWnd = CreateWindowW(szWindowName,                                      // ��������
		                      szTitle,                                           // ���ڱ�����
		                      WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,       // ���ڷ��
		                      sXWIDTH,                                           // ������ʼX
		                      sYHEIGHT,                                          // ������ʼY
		                      XWIDTH,                                            // ���ڿ�� 
		                      YHEIGHT,                                           // ���ڸ߶�
		                      NULL,                                              // �����
		                      NULL,                                              // �˵����
		                      hInstance,                                         // �봰����ص�ģ��ʵ�����
		                      NULL);                                             // ֵָ�룬���ݸ�����WM_CREATE��Ϣ
	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);                                                  // ��ʾ����
	UpdateWindow(hWnd);                                                          // ���´���
	
	return TRUE;
}

// ����DialogBox�Ļص�����

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

// ���������������ϵ���Ϣ��

LRESULT CALLBACK WndProc(HWND   hWnd,                                            // ���ھ��
	                     UINT   message,                                         // ��Ϣ��� 
	                     WPARAM wParam,                                          // ���Ӳ��� 
	                     LPARAM lParam)                                          // ���Ӳ���
{
	switch (message)
	{
	case WM_CREATE:                                                              // ��ʼ������
	{
		// PlaySound(TEXT("����˹����.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);             // ��������
		return 0;
	}

	case WM_PAINT:                                                               // WM_PAINT ����������
	{
		PAINTSTRUCT ps;                                                          // �����ͼ����
		HDC hdc = BeginPaint(hWnd, &ps);                                         // ���岢�õ��豸hdc 	
		HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));                   // ���廭�ʶ���
		SelectObject(hdc, pen);                                                  // ѡ�����

		ShowTip(hdc);                                                            // ��ʾ��ǰ����
		preorder = ShowPreOrder();                                               // ����Ԥ�ȷ�������
		Boundary(hdc);                                                           // ���Ʊ߽緽��

		DeleteObject(pen);                                                       // ɾ�����ʶ���
		EndPaint(hWnd, &ps);                                                     // ��������
	}
	break;

	case WM_TIMER:
	{
		OutTimer(hWnd);
	}
	break;

	case WM_KEYDOWN:
		switch (wParam)                                                           // Ϊ�����ӿɶ��ԣ�д�����ú�����ʾ
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
		case 's':                                                                  // ��ʼ��Ϸ
			SetTimer(hWnd, ID_TIMER, timegap, NULL);
			break;
		case 'a':                                                                  // ��ͣ��Ϸ
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

	case WM_DESTROY:                                                              // WM_DESTROY �����˳���Ϣ������
		PostQuitMessage(0);
		KillTimer(hWnd, ID_TIMER);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);                      // ���ڴ�����
	}
	return 0;
}

// ����������DCʵ�ִ�������˸
void OutPaint(HDC hdc)
{
	HDC HMemDc = CreateCompatibleDC(hdc);                                         // ����������DC

	HBITMAP hBitmapBack = CreateCompatibleBitmap(hdc, 681, 400);                  // ����һ��ֽ

	SelectObject(HMemDc, hBitmapBack);                                            // ѡ��ǰ���

	Boundary(HMemDc);
	score1 = ClearRow();                                                          // ���д��������ط���
	ShowTip(HMemDc);                                                              // ��ʾ�����ı���Ϣ
	if (rightover)
	{
		order = ShowOrder(preorder);                                              // �ҷ����׺����ɷ���
		preorder = ShowPreOrder();                                                // ��ʾ��һ�����������
		rightover = FALSE;
	}
	ShowPreSquare(HMemDc);                                                        // ��ʾԤ��������
    ShowSquare(HMemDc);                                                           // ��ʾ��Ϸ������

	BitBlt(hdc, 0, 0, XWIDTH, YHEIGHT, HMemDc, 0, 0, SRCCOPY);                    // ���ڴ�Ԥ�Ȼ��ƺú󴫵�

	DeleteObject(hBitmapBack);                                                    // ɾ��λͼ

	DeleteDC(HMemDc);                                                             // ɾ�����
}

// ��Ϸ��ѭ��
void OutTimer(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	MoveRightSqare();                                                            // �Զ�����
	if (ReachRightBlock())                                                       // �ҷ����׺�ӷִ����Լ��Ѷȴ���
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
	OutPaint(hdc);                                                                // ���ƽ���
	ReleaseDC(hWnd, hdc);
	if (AllRow())                                                                 // ���к�ֹͣ��Ϸ
	{
		KillTimer(hWnd, ID_TIMER);
		UINT nRet = MessageBox(NULL, TEXT("�Ƿ�����"), TEXT("Game Over"), MB_OKCANCEL);
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

// ��ʾ�����ı���Ϣ
void ShowTip(HDC HMemDc)
{
	StringCchPrintf(szBuffer, 128, TEXT("%s: %d"), TEXT("��ǰ����"), score1);
	StringCchLength(szBuffer, 128, &iTarget);
	TextOut(HMemDc, 250, 250, szBuffer, iTarget);
	StringCchPrintf(szBuffer, 128, TEXT("%s: %d"), TEXT("��ǰ�Ѷ�"), level);
	StringCchLength(szBuffer, 128, &iTarget);
	TextOut(HMemDc, 250, 300, szBuffer, iTarget);
	StringCchPrintf(szBuffer, 128, TEXT("%s"), TEXT("��Ϸ����"));
	StringCchLength(szBuffer, 128, &iTarget);
	TextOut(HMemDc, 400, 250, szBuffer, iTarget);
	StringCchPrintf(szBuffer, 128, TEXT("%s"), TEXT("s����ʼ��Ϸ a����ͣ��Ϸ"));
	StringCchLength(szBuffer, 128, &iTarget);
	TextOut(HMemDc, 400, 280, szBuffer, iTarget);
	StringCchPrintf(szBuffer, 128, TEXT("%s"), TEXT("����-> ��ת<- ����up�� ����down��"));
	StringCchLength(szBuffer, 128, &iTarget);
	TextOut(HMemDc, 400, 310, szBuffer, iTarget);
}

// ���Ϻ���
void OutUp(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	if (ReachUpBlock())                                                           // �ж��Ƿ��������
		upover = TRUE;
	else
		upover = FALSE;
	if (!upover)
		UpMove();
	OutPaint(hdc);
	ReleaseDC(hWnd, hdc);
}

// ���Ϻ���
void OutDown(HWND hWnd)                                                           // �ж��Ƿ��������
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

// ��ת����
void OutLeft(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);

	if (order == 0)
	{
		RevolveRectan();
	}
	else if (order == 2)                                                          // ��������ת���Ǳ���������ת
	{
	}                
	else
		RevolveSqare();
	   
	OutPaint(hdc);
	ReleaseDC(hWnd, hdc);
}

// ���ٺ���
void OutRight(HWND hWnd)
{
	OutTimer(hWnd);                                                               // ������WM_TIMER��Ϊ������Ϣ
}