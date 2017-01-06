#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;

// ���� ���ڿ�, ������ Ŭ������ �����ϴ� ����
LPCTSTR lpszClass = TEXT("First");

// WinMain			Entry Point 
// APIENTRY			_stdcall�� ����Ѵٴ� ��
// hInstance		���α׷��� �ν��Ͻ� �ڵ�
// lpszCmdParam		��������� �Էµ� ���α׷� �μ�. ���� ���Ŀ� �� ������ ���
// nCmdShow			���α׷��� ����� ����
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;

	MSG Message;

	WNDCLASS WndClass;

	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;	// ������ ���� ����
	WndClass.cbWndExtra = 0;	// ������ ���� ����

	
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ������ ����
	//WndClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 255));	// �Ķ��� ���
	// GetStockObject ������� �⺻������ �����ϴ� �귯��, �� ���� �ڵ��� ���ϴ� �Լ�

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// �����찡 ����� Ŀ��
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// �����찡 ����� ������

	WndClass.hInstance = hInstance;		// �� ������ Ŭ������ ����ϴ� ���α׷��� ��ȣ
	
	WndClass.lpfnWndProc = WndProc;	// �޽��� ó�� �Լ� ����
	WndClass.lpszClassName = lpszClass; // ������ Ŭ������ �̸��� ���ڿ��� ����
	
	WndClass.lpszMenuName = NULL;	// ���α׷��� ����� �޴� ����
									// ������ �޴��� ������ ���ڿ��� �����Ǿ� ������ ó�� ��
	
	WndClass.style = CS_HREDRAW | CS_VREDRAW;	// ������ ��Ÿ��, ���γ� ���ΰ� �ٲ�� �����츦 ���� �׸��� �ɼ�
	
	
	RegisterClass(&WndClass);	// ������ Ŭ������ ���

	hWnd = CreateWindow(lpszClass,	// �����ϰ��� �ϴ� �������� Ŭ������ �����ϴ� ���ڿ�
		TEXT("My First Program")/*lpszClass*/,					// �������� Ÿ��Ʋ �ٿ� ��Ÿ�� ���ڿ�
		WS_OVERLAPPEDWINDOW,		// ������� �ϴ� �������� ���¸� �����ϴ� �μ�
									// WS_OVERLAPPEDWINDOW : ���� ������ ������
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
									// x, y, width, height	�������� ũ��� ��ġ�� ����
									// �ü���� ȭ��ũ�⿡ �°� ������ ũ��� ��ġ�� ����
		NULL,				// �θ� ������ �ڵ� ����, NULL�̸� ����ũž�� �θ�
		(HMENU)NULL,		// �����쿡�� ����� �޴��� �ڵ� ����
		hInstance,			// ���α׷��� �ڵ��� ����
		NULL);

	// hWnd		ȭ�鿡 ����ϰ��� �ϴ� �������� �ڵ�
				//CreateWindow�� ������ �ڵ��� �״�� �ѱ�� ��
	// nCmdShow	�����츦 ȭ�鿡 ����ϴ� ����� ����

	ShowWindow(hWnd, nCmdShow);

	// �޽��� ť���� �޽����� �о����
	while (GetMessage(&Message, NULL, 0, 0))	
	{
		// Ű���� ���� �޽����� �߻��� �� ���ڰ� �ԷµǾ��ٴ� �޽����� ����� ����
		TranslateMessage(&Message);

		// �޽��� ť���� ���� �޽����� �������� �޽��� ó�� �Լ��� ����
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

// ����ڿ� �ý����� �������� �޽����� ó��
// �ݹ��Լ�
LRESULT CALLBACK WndProc(HWND hWnd,		// �޽����� ���� �������� �ڵ�
						UINT iMessage,	// � ������ �޽����ΰ�
						WPARAM wParam,	// 
						LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return ( DefWindowProc(hWnd, iMessage, wParam, lParam) );
}