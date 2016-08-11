#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOWWIDTH 1900
#define WINDOWHEIGHT 1020

int i = 0;
FILE *fp;
int x = 0, y = 0;

//RGBではなくBGR
// フラットカラー
int cTab[10] = {
	/*0xe74c3c*/0x3c4ce7, //@
	/*0xe67e22*/0x227ee6, //#
	/*0xf1c40f*/0x0fc4f1, //*
	/*0x2ecc71*/0x71cc2e, //|
	/*0x1abc9c*/0x9cbc1a, //!
	/*0x3498db*/0xdb9834, //:
	/*0x9b59b6*/0xb6599b, //,
	/*0x95a5a6*/0xa6a595, //.
	/*0xecf0f1*//*0xf1f0ec*/0xFFFFFF, //
	/*0x34495e*/0x5e4934  //\n
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;
	int c;

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		char *filename = "15.txt";
		int a;
		if (!fp && (fopen_s(&fp, filename, "r")) != 0){
			exit(EXIT_FAILURE);
		}
		hdc = BeginPaint(hwnd, &ps);
		while ((a = fgetc(fp)) != EOF){
			switch (a){
			case '@': c = 0; break;
			case '#': c = 1; break;
			case '*': c = 2; break;
			case '|': c = 3; break;
			case '!': c = 4; break;
			case ':': c = 5; break;
			case ',': c = 6; break;
			case '.': c = 7; break;
			case ' ': c = 8; break;
			case '\n': c = 9; y += 1; x = 0; break;
			}//switch
			SetPixel(hdc, x, y, cTab[c]);
			x += 1;
		}//while
		EndPaint(hwnd, &ps);
		fclose(fp);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;

	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WndProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT("pixel");

	if (!RegisterClass(&winc)) return -1;

	hwnd = CreateWindow(
		TEXT("pixel"), 
		TEXT("ピクセル"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		WINDOWWIDTH, 
		WINDOWHEIGHT,
		NULL, NULL, hInstance, NULL
		);
	if (hwnd == NULL) return -1;

	while (GetMessage(&msg, NULL, 0, 0)) DispatchMessage(&msg);
	return msg.wParam;
}