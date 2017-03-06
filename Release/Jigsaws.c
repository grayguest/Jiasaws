#include <windows.h>
#include <stdlib.h>
#include <time.h>
#pragma comment (lib,"msimg32.lib")//TransparentBlt���������
#pragma comment(lib,"winmm.lib")//��������ͷ�ļ���



TCHAR szAppName[] = TEXT("Jigsaws");

HBITMAP hBitmap;//λͼ���
UINT cxPic, cyPic;//λͼ����
#define CXJIGSAW 3 //����ƴͼά��
#define CYJIGSAW 3

typedef struct _JIGSAWSPT
{
	UINT uiX;
	UINT uiY;
} JIGSAWSPT;

LRESULT CALLBACK JigsawsWinProc(
								HWND hwnd,      // handle to window
								UINT uMsg,      // message identifier
								WPARAM wParam,  // first message parameter
								LPARAM lParam   // second message parameter
								);

int WINAPI WinMain(
				   HINSTANCE hInstance,      // handle to current instance
				   HINSTANCE hPrevInstance,  // handle to previous instance
				   LPSTR lpCmdLine,          // command line
				   int nCmdShow              // show state
				   )
{
	//λͼ���
	BITMAP bm;
	CHAR szBmBuffer[255];
	
	WNDCLASS wndcls;
	HWND hwnd;
	BOOL bRet;
	MSG msg;
	
	
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndcls.hInstance = hInstance;
	wndcls.lpfnWndProc = JigsawsWinProc;
	wndcls.lpszClassName = szAppName;
	wndcls.lpszMenuName = NULL;//�޲˵�
	wndcls.style = CS_VREDRAW | CS_HREDRAW;
	
	if (!RegisterClass(&wndcls))
	{
		MessageBox(NULL, TEXT("�˳�����Ҫ��WINDOWS NT����ϵͳ������!"),
			szAppName, MB_ICONERROR);
		return 0;
	}
	
	//��ȡλͼ��С
	hBitmap = LoadImage(0, "Pic\\Default1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	memset(&bm, 0, sizeof(BITMAP));
	memset(szBmBuffer, 0, 255);
	GetObject(hBitmap, sizeof(BITMAP), &bm);
	
	hwnd = CreateWindow(szAppName, TEXT("Jigsaws Ver0.1"), WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME &
		~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, cxPic = bm.bmWidth, cyPic = bm.bmHeight,
		NULL, NULL, hInstance, NULL);
	
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);
	
	while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
	{ 
		if (-1 == bRet)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}
	
	
	return 0;
}

LRESULT CALLBACK JigsawsWinProc(
								HWND hwnd,      // handle to window
								UINT uMsg,      // message identifier
								WPARAM wParam,  // first message parameter
								LPARAM lParam   // second message parameter
								)
{
	
	
	void Exchange(JIGSAWSPT* jigsawspt1, JIGSAWSPT* jigsawspt2);//��������JIGSAWSPT�ṹ���ֵ
	
	static JIGSAWSPT jigsawspts[CYJIGSAW][CXJIGSAW];//ԭ����
	static JIGSAWSPT jigsawsptsCur[CYJIGSAW][CXJIGSAW];//��ǰ����
	static UINT cxPerBlock, cyPerBlock;//ƴͼÿ���,�ݴ�С
	static int iCurPtX = CYJIGSAW - 1;//��ǰ�Ŀհ׿�����
	static int iCurPtY = CXJIGSAW - 1;//��ǰ�Ŀհ׿�����
	
	
	int i,j;
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hCompatibleDC;//ƴͼ����ȡ���ļ���DC
	RECT rect;//ȷ�����»�ͼ����
	
	int iRandomX;
	int iRandomY;

	int iCorrectBlock;
	
	
	
	switch(uMsg)
	{
	case WM_CREATE:
		cxPerBlock = cxPic / CXJIGSAW;
		cyPerBlock = cyPic / CYJIGSAW;
		//��ʼ��ԭ����,��ǰ����
		for (i = 0 ; i < CYJIGSAW; ++i)
		{
			for (j = 0 ; j < CXJIGSAW ; ++j)
			{
				jigsawspts[i][j].uiX = j * cxPerBlock;
				jigsawspts[i][j].uiY = i * cyPerBlock;
				jigsawsptsCur[i][j].uiX = j * cxPerBlock;
				jigsawsptsCur[i][j].uiY = i * cyPerBlock;
			}
		}
		//�������ǰ����
		srand((unsigned)time(NULL));
		for (i = 0; i < CXJIGSAW * CYJIGSAW; ++i)
		{
			iRandomX = rand() % CYJIGSAW;
			iRandomY = rand() % CXJIGSAW;
			//Ԥ�����һ��ռ�
			if (CYJIGSAW - 1 == iRandomX && CXJIGSAW - 1 == iRandomY)
			{
				continue;
			}
			if (CYJIGSAW - 1 == iRandomX / 2 + 1 && CXJIGSAW - 1 == iRandomY / 3 + 1)
			{
				continue;
			}
			Exchange(&jigsawsptsCur[iRandomX][iRandomY],
				&jigsawsptsCur[iRandomX / 2 + 1][iRandomY / 3 + 1]);
		}
		return 0;
		
	case WM_KEYDOWN:
		//��Ӧ����,���ŵ�ǰ����,�����𴰿��ػ�
		switch(wParam)
		{
		case VK_LEFT:
			if ((CXJIGSAW - 1) != iCurPtY)
			{
				Exchange(&jigsawsptsCur[iCurPtX][iCurPtY], &jigsawsptsCur[iCurPtX][iCurPtY + 1]);
				iCurPtX = iCurPtX;
				iCurPtY = iCurPtY + 1;
				rect.left = (iCurPtY - 1) * cxPerBlock;
				rect.right = (iCurPtY + 1) * cxPerBlock;
				rect.top = iCurPtX * cyPerBlock;
				rect.bottom = (iCurPtX + 1) * cyPerBlock;
				InvalidateRect(hwnd, &rect, TRUE);
			}
			break;
		case VK_RIGHT:
			if (0 != iCurPtY)
			{
				Exchange(&jigsawsptsCur[iCurPtX][iCurPtY], &jigsawsptsCur[iCurPtX][iCurPtY - 1]);
				iCurPtX = iCurPtX;
				iCurPtY = iCurPtY - 1;
				rect.left = iCurPtY * cxPerBlock;
				rect.right = (iCurPtY + 2) * cxPerBlock;
				rect.top = iCurPtX * cyPerBlock;
				rect.bottom = (iCurPtX + 1) * cyPerBlock;
				InvalidateRect(hwnd, &rect, TRUE);
			}
			break;
		case VK_UP:
			if ((CYJIGSAW - 1) != iCurPtX)
			{
				Exchange(&jigsawsptsCur[iCurPtX][iCurPtY], &jigsawsptsCur[iCurPtX + 1][iCurPtY]);
				iCurPtX = iCurPtX + 1;
				iCurPtY = iCurPtY;
				rect.left = iCurPtY * cxPerBlock;
				rect.right = (iCurPtY + 1) * cxPerBlock;
				rect.top = (iCurPtX - 1) * cyPerBlock;
				rect.bottom = (iCurPtX + 1) * cyPerBlock;
				InvalidateRect(hwnd, &rect, TRUE);
			}
			break;
		case VK_DOWN:
			if (0 != iCurPtX)
			{
				Exchange(&jigsawsptsCur[iCurPtX][iCurPtY], &jigsawsptsCur[iCurPtX - 1][iCurPtY]);
				iCurPtX = iCurPtX - 1;
				iCurPtY = iCurPtY;
				rect.left = iCurPtY * cxPerBlock;
				rect.right = (iCurPtY + 1) * cxPerBlock;
				rect.top = iCurPtX * cyPerBlock;
				rect.bottom = (iCurPtX + 2) * cyPerBlock;
				InvalidateRect(hwnd, &rect, TRUE);
			}
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		//��������
		PlaySound("Sound\\ding.wav",NULL,SND_ASYNC);

	case WM_PAINT:
		//��ʾ��ǰ�����������ƴͼ
//		InvalidateRect(hwnd, NULL, TRUE);
		hdc = BeginPaint(hwnd, &ps);
		hCompatibleDC = CreateCompatibleDC(hdc);
		SelectObject(hCompatibleDC, hBitmap);
		for (i = 0 ; i < CYJIGSAW ; ++i)
		{
			for (j = 0 ; j < CXJIGSAW ; ++j)
			{
				if ((CXJIGSAW - 1) * cxPerBlock == jigsawsptsCur[i][j].uiX && 
					(CYJIGSAW - 1) * cyPerBlock == jigsawsptsCur[i][j].uiY)//Ԥ�����һ��λͼ
				{
					continue;
				}
				TransparentBlt(hdc, j * cxPerBlock, i * cyPerBlock, cxPerBlock, cyPerBlock,
					hCompatibleDC, jigsawsptsCur[i][j].uiX, jigsawsptsCur[i][j].uiY, cxPerBlock, 
					cyPerBlock, RGB(255,255,255));
			}
		}
		//����Ƿ��Ѿ����ƴͼ
		iCorrectBlock = 0;
		for (i = 0 ; i < CYJIGSAW ; ++i)
		{
			for (j = 0 ; j < CXJIGSAW ; ++j)
			{

				if (jigsawspts[i][j].uiX == jigsawsptsCur[i][j].uiX &&
						 	jigsawspts[i][j].uiY == jigsawsptsCur[i][j].uiY)
					++iCorrectBlock;
 			
			}
		}
		if (CXJIGSAW * CYJIGSAW == iCorrectBlock)
		{
			MessageBox(hwnd, "Perfect!", szAppName, MB_OK);
		}
		//ɾ��GDI����,��Ҫɾ��ȫ�ֶ���λͼ
		DeleteObject(hdc);
		DeleteObject(hCompatibleDC);
		EndPaint(hwnd, &ps);
		return 0;
		
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	
}

//////////////////////////////////////////////////////////////////////////
//��������JIGSAWSPT�ṹ���ֵ
void Exchange(JIGSAWSPT* jigsawspt1, JIGSAWSPT* jigsawspt2)
{
	int iCurX,iCurY;
	iCurX = jigsawspt1->uiX;	iCurY = jigsawspt1->uiY;
	jigsawspt1->uiX = jigsawspt2->uiX; jigsawspt1->uiY = jigsawspt2->uiY;
	jigsawspt2->uiX = iCurX; jigsawspt2->uiY = iCurY;
}
















