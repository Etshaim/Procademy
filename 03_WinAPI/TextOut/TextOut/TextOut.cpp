#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;

// 전역 문자열, 윈도우 클래스를 정의하는 사용됨
LPCTSTR lpszClass = TEXT("TextOut");

// WinMain			Entry Point 
// APIENTRY			_stdcall을 사용한다는 뜻
// hInstance		프로그램의 인스턴스 핸들
// lpszCmdParam		명령행으로 입력된 프로그램 인수. 실행 직후에 열 파일의 경로
// nCmdShow			프로그램이 실행될 형태
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;

	MSG Message;

	WNDCLASS WndClass;

	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;	// 일종의 예약 영역
	WndClass.cbWndExtra = 0;	// 일종의 예약 영역


	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 윈도우 배경색
	//WndClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 255));	// 파란색 배경
	// GetStockObject 윈도우즈가 기본적으로 제공하는 브러시, 펜 등의 핸들을 구하는 함수

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// 윈도우가 사용할 커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// 윈도우가 사용할 아이콘

	WndClass.hInstance = hInstance;		// 이 윈도우 클래스를 등록하는 프로그램의 번호

	WndClass.lpfnWndProc = WndProc;	// 메시지 처리 함수 지정
	WndClass.lpszClassName = lpszClass; // 윈도우 클래스의 이름을 문자열로 정의

	WndClass.lpszMenuName = NULL;	// 프로그램이 사용할 메뉴 지정
	// 빠지면 메뉴가 쓰레기 문자열로 지정되어 에러로 처리 됨

	WndClass.style = CS_HREDRAW | CS_VREDRAW;	// 윈도우 스타일, 가로나 세로가 바뀌면 윈도우를 새로 그리는 옵션


	RegisterClass(&WndClass);	// 윈도우 클래스를 등록

	hWnd = CreateWindow(lpszClass,	// 생성하고자 하는 윈도우의 클래스를 지정하는 문자열
		TEXT("My First Program")/*lpszClass*/,					// 윈도우의 타이틀 바에 나타날 문자열
		WS_OVERLAPPEDWINDOW,		// 만들고자 하는 윈도우의 형태를 지정하는 인수
		// WS_OVERLAPPEDWINDOW : 가장 무난한 윈도우
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		// x, y, width, height	윈도우의 크기와 위치를 지정
		// 운영체제가 화면크기에 맞게 적당한 크기와 위치를 설정
		NULL,				// 부모 윈도우 핸들 지정, NULL이면 데스크탑이 부모
		(HMENU)NULL,		// 윈도우에서 사용할 메뉴의 핸들 지정
		hInstance,			// 프로그램의 핸들을 지정
		NULL);

	// hWnd		화면에 출력하고자 하는 윈도우의 핸들
	//CreateWindow가 리턴한 핸들을 그대로 넘기면 됨
	// nCmdShow	윈도우를 화면에 출력하는 방법을 지정

	ShowWindow(hWnd, nCmdShow);

	// 메시지 큐에서 메시지를 읽어들임
	while (GetMessage(&Message, NULL, 0, 0))
	{
		// 키보드 눌림 메시지가 발생할 때 문자가 입력되었다는 메시지를 만드는 역할
		TranslateMessage(&Message);
		// 메시지 큐에서 꺼낸 메시지를 윈도우의 메시지 처리 함수로 전달
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

// 사용자와 시스템이 보내오는 메시지를 처리
// 콜백함수
LRESULT CALLBACK WndProc(HWND hWnd,		// 메시지를 받을 윈도우의 핸들
	UINT iMessage,	// 어떤 종류의 메시지인가
	WPARAM wParam,	// 
	LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, TEXT("ohhhhhhh"), 8);
		EndPaint(hWnd, &ps);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}