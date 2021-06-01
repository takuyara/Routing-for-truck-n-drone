//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include <windows.h>
#include <cstdio>
#include "BaseData.h"
#include "gconfig.h"
#include <algorithm>
const int dlt = 10000, ctbd = 5000;
const double inf = 1e100;
inline Point RePos(const Point &a){
	return Point((a.x + dlt) / 25, (a.y + dlt) / 25);
}

inline bool iscity(const Point &a){
	return a.x < ctbd && a.y < ctbd && a.x >= -ctbd && a.y > -ctbd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	static BaseData dt;
	static Point pt[config::maxv];
	switch (message){
		case WM_CREATE:{
			freopen("debug.log", "w", stderr);
			dt.ReadFromFile(gconfig::file_name);
			fprintf(stderr, "%d %d %d %d %d %d\n", dt.V, dt.E, dt.M, dt.N, dt.L, dt.Z);
			double mnx = inf, mny = inf, mxx = -inf, mxy = -inf;
			for (int i = 1; i <= dt.V; ++i){
				pt[i] = RePos(dt.vertex[i]);
				mnx = std::min(dt.vertex[i].x, mnx), mny = std::min(dt.vertex[i].y, mny);
				mxx = std::max(dt.vertex[i].x, mxx), mxy = std::max(dt.vertex[i].y, mxy);
			}
			fprintf(stderr, "LeftTop: (%.2lf, %.2lf), RightBot: (%.2lf, %.2lf)\n", mnx, mny, mxx, mxy);
			break;
		}
		case WM_PAINT:{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
			HPEN hPenB = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
			HPEN hPenG = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			HPEN hPenW = (HPEN)GetStockObject(WHITE_PEN);
			HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
			SelectObject(hDC, hBrush);
			SelectObject(hDC, hPenW);
			for (int i = 1; i <= dt.V; ++i){
				Point p = RePos(dt.vertex[i]);
				Ellipse(hDC, p.x - 3, p.y - 3, p.x + 3, p.y + 3);
				//fprintf(stderr, "%.2lf %.2lf\n", p.x, p.y);
			}
			for (auto i: dt.edge){
				Point p1 = RePos(dt.vertex[i.first.first]), p2 = RePos(dt.vertex[i.first.second]);
				SelectObject(hDC, i.second.first > 5 && iscity(dt.vertex[i.first.first]) ? hPenG : hPenB);
				MoveToEx(hDC, p1.x, p1.y, NULL);
				LineTo(hDC, p2.x, p2.y);
			}
			DeleteObject(hPenB);
			DeleteObject(hPenG);
			DeleteObject(hBrush);
			
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow){
	WNDCLASS wndClass;
	TCHAR lpszClassName[] = "MapView";
	TCHAR lpszTitle[] = "A View of our Map";
	wndClass.style = wndClass.cbClsExtra = wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = lpszClassName;
	if (!RegisterClass(&wndClass)){
		MessageBeep(0);
		return -1;
	}
	HWND hWnd = CreateWindow(lpszClassName, lpszTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0)){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

