


#include "framework.h"
#include "DynamicMemoryAndFiles-BasicsOfProgramming-lab8.h"
#include <stdio.h>

#define MAX_LOADSTRING 100


HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_DYNAMICMEMORYANDFILESBASICSOFPROGRAMMINGLAB8, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);


	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DYNAMICMEMORYANDFILESBASICSOFPROGRAMMINGLAB8));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_DYNAMICMEMORYANDFILESBASICSOFPROGRAMMINGLAB8));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DYNAMICMEMORYANDFILESBASICSOFPROGRAMMINGLAB8);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void DrawTriangle(HDC hdc, POINT v1, POINT v2, POINT v3)
{
	POINT point[3] =
	{
		v1.x, v1.y,
		v2.x, v2.y,
		v3.x, v3.y
	};
	Polygon(hdc, point, 3);
}

void DrawAxies(HDC hdc, int left, int top, int right, int bottom, int offset = 0)
{
	HBRUSH triangleBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, triangleBrush);


	MoveToEx(hdc, left, top - offset, NULL);
	LineTo(hdc, left, bottom);
	LineTo(hdc, right + offset, bottom);

	int size = 7;
	int pickSize = 3;
	DrawTriangle(hdc, { left - size, top - offset }, { left, top - size * pickSize - offset }, { left + size, top - offset });
	DrawTriangle(hdc, { right + offset, bottom - size }, { right + size * pickSize + offset, bottom }, { right + offset, bottom + size });
}

void PlotGraph(HDC hdc, int left, int top, int right, int bottom, int* num, int elementsCount, int minID = -1, int maxID = -1)
{
	DrawAxies(hdc, left, top, right, bottom, 20);

	float width = (float)(right - left);
	float height = (float)(bottom - top);
	float elementWidht = width / elementsCount;

	HBRUSH positiveBrush = CreateSolidBrush(RGB(100, 100, 255));
	HBRUSH negativeBrush = CreateSolidBrush(RGB(255, 100, 100));


	HBRUSH minBrush = CreateSolidBrush(RGB(100, 255, 100));
	HBRUSH maxBrush = CreateSolidBrush(RGB(100, 255, 100));

	for (int i = 0; i < elementsCount; i++)
	{
		if (i != minID && i != maxID)
		{
			if (num[i] > 0)
				SelectObject(hdc, positiveBrush);
			else if (num[i] < 0)
				SelectObject(hdc, negativeBrush);
		}
		else if (i == minID)
			SelectObject(hdc, minBrush);
		else if (i == maxID)
			SelectObject(hdc, maxBrush);

		Rectangle(hdc, left + i * elementWidht, bottom - (num[i] / 20.0f) * (bottom - top), left + (i + 1) * elementWidht, bottom);
	}

}
void TaskD(int* array, int elementsCount, int** newArray, int* newElementsCount)
{
	int min = array[0];
	int max = array[0];

	int edge0 = 0;
	int edge1 = 0;

	for (int x = 0; x < elementsCount; x++)
	{
		if (array[x] < min)
		{
			min = array[x];
			edge0 = x;
		}
		if (array[x] > max)
		{
			max = array[x];
			edge1 = x;
		}
	}

	if (edge0 > edge1)
	{
		int temp = edge0;
		edge0 = edge1;
		edge1 = temp;
	}

	int countBetweenEdgesToDelete = 0;

	for (int x = edge0 + 1; x < edge1; x++)
	{
		if ((array[x] % 2) == 0)
			countBetweenEdgesToDelete++;
	}
	*newElementsCount = elementsCount - countBetweenEdgesToDelete;

	*newArray = new int[(*newElementsCount)];

	int id = 0;
	for (int x = 0; x < elementsCount; x++)
		if (!(x > edge0 && x < edge1 && array[x] % 2 == 0))
		{
			(*newArray)[id] = array[x];
			id++;
		}
}

void TaskD(int** array, int* elementsCount)
{
	int min = (*array)[0];
	int max = (*array)[0];

	int edge0 = 0;
	int edge1 = 0;

	for (int x = 0; x < (*elementsCount); x++)
	{
		if ((*array)[x] < min)
		{
			min = (*array)[x];
			edge0 = x;
		}
		if ((*array)[x] > max)
		{
			max = (*array)[x];
			edge1 = x;
		}
	}

	if (edge0 > edge1)
	{
		int temp = edge0;
		edge0 = edge1;
		edge1 = temp;
	}

	int countBetweenEdgesToDelete = 0;

	for (int x = edge0 + 1; x < edge1; x++)
	{
		if (((*array)[x] % 2) == 0)
			countBetweenEdgesToDelete++;
	}
	int temporaryElementsCount = (*elementsCount) - countBetweenEdgesToDelete;
	int* temporaryArray = new int[temporaryElementsCount];

	int id = 0;
	for (int x = 0; x < (*elementsCount); x++)
		if (!(x > edge0 && x < edge1 && (*array)[x] % 2 == 0))
		{
			temporaryArray[id] = (*array)[x];
			id++;
		}

	delete[*elementsCount] array;

	*elementsCount = temporaryElementsCount;
	*array = temporaryArray;
}

void TaskDShift(int** array, int* elementsCount)
{
	int min = (*array)[0];
	int max = (*array)[0];

	int edge0 = 0;
	int edge1 = 0;

	for (int x = 0; x < (*elementsCount); x++)
	{
		if ((*array)[x] < min)
		{
			min = (*array)[x];
			edge0 = x;
		}
		if ((*array)[x] > max)
		{
			max = (*array)[x];
			edge1 = x;
		}
	}

	if (edge0 > edge1)
	{
		int temp = edge0;
		edge0 = edge1;
		edge1 = temp;
	}

	int temporaryElementsCount = 0;
	int id = 0;
	for (int x = 0; x < (*elementsCount); x++)
		if (!(x > edge0 && x < edge1 && (*array)[x] % 2 == 0))
		{
			(*array)[id] = (*array)[x];
			id++;
			temporaryElementsCount++;
		}
	*elementsCount = temporaryElementsCount;
}


void GetArrayFromFile(int** array, int* elementsCount, const char* filePath)
{
	FILE* filePointer;
	fopen_s(&filePointer, filePath, "r");

	fscanf_s(filePointer, "%d", elementsCount);
	(*array) = new int[*elementsCount];

	for (int i = 0; i < *elementsCount; i++)
		fscanf_s(filePointer, "%d", &(*array)[i]);
}
void WriteArrayToFile(int* array, int elementsCount, const char* filePath)
{
	FILE* filePointer;
	fopen_s(&filePointer, filePath, "w");

	for (int i = 0; i < elementsCount; i++)
		fprintf_s(filePointer, "%d ", array[i]);

	fclose(filePointer);
}

int* num;
int elementsCount = 0;
int* num2;
int elementsCount2 = 0;
int minID = -1;
int maxID = -1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
	{
		GetArrayFromFile(&num, &elementsCount, "data.txt");
		GetArrayFromFile(&num2, &elementsCount2, "data.txt");
		TaskDShift(&num2, &elementsCount2);
		WriteArrayToFile(num, elementsCount, "OutputData.txt");
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT client;
		GetClientRect(hWnd, &client);
		int width = client.right - client.left;
		int height = client.bottom - client.top;

		PlotGraph(hdc, width * 0.05f, height * 0.3f, width * 0.4f, height * 0.7f, num, elementsCount, minID, maxID);
		PlotGraph(hdc, width * 0.6f, height * 0.3f, width - width * 0.05f, height * 0.7f, num2, elementsCount2, minID, maxID);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		delete[elementsCount] num;
		delete[elementsCount2] num2;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}