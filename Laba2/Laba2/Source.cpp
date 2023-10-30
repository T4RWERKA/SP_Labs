#include <Windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <codecvt>

#define ON_OPEN_FILE   1

#define ID_EDIT_LINES       1001
#define ID_EDIT_ROWS        1002
#define ID_STATIC_LINES     1003
#define ID_STATIC_ROWS      1004
#define ID_STATIC_TEXT      1005
#define ID_EDIT_TEXT        1006

#define MAX_LINES           15
#define MAX_ROWS            15
#define MAX_TEXTLEN         41
#define TOOL_BAR_HIGHT       0
#define MAX_FILE_NAME      260 

const std::wstring startText = L"Привет мир!!!";
const int textHight = 50;
int cellH, cellW;
const RECT constRect = { 0, 30, 0 + 50, 30 + 50 };

int wndH = 600, wndW = 800;
int nRows = 1, mColumns = 1;
HINSTANCE hInstance, hLines, hRows, hText;
std::vector<std::wstring> text_table {L""};

OPENFILENAME ofn;
wchar_t filename[MAX_FILE_NAME];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2);
void RecalculateTable();
void FindBiggestCell(HDC hdc, std::vector<std::wstring> textTable, int nRows, int mColumns, POINT &posMaxH, POINT &posMaxW, LOGFONT &lf);
void DisplayText(HDC hdc, std::vector<std::wstring> textTable, int nRows, int mColumns);
void DisplayTableLines(HDC hdc);
void InitializeMenu(HWND hwnd);
void InitializeOFN(HWND hwnd);
int LoadData(wchar_t filename[MAX_FILE_NAME], std::vector<std::wstring> textTable);

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd; MSG msg;
    WNDCLASSEX wcexmain;

    hInstance = _hInstance;

    wcexmain.cbSize = sizeof(WNDCLASSEX);
    wcexmain.style = 0;
    wcexmain.lpfnWndProc = WndProc;
    wcexmain.cbClsExtra = 0;
    wcexmain.cbWndExtra = 0;
    wcexmain.hInstance = hInstance;
    wcexmain.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcexmain.hCursor = LoadCursor(0, IDC_ARROW);
    wcexmain.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcexmain.lpszMenuName = NULL;
    wcexmain.lpszClassName = L"MainClass";
    wcexmain.hIconSm = 0;
    RegisterClassEx(&wcexmain);

    hWnd = CreateWindowEx(0,			// создание главного окна
        L"MainClass",		            // имя класс
        L"Лабораторная 2",		        // имя окна
        WS_OVERLAPPEDWINDOW,			// режимы отображения окна
        200, 30,				        // положение окна по х и у
        wndW, wndH,				        // ширина и высота окна
        0,						        // дескриптор родительского окошка
        0,						        // дескриптор меню
        hInstance,				        // дескриптор экземпляра приложения
        NULL);					        // указатель на данные, передаваемые из пользовательской функции

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        InitializeMenu(hWnd);
        InitializeOFN(hWnd);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case ON_OPEN_FILE:
            if (GetOpenFileName(&ofn))
                LoadData(filename, text_table);
                RecalculateTable();
                InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;
    case WM_SIZE:
    {
        wndH = HIWORD(lParam);
        wndW = LOWORD(lParam);

        RecalculateTable();
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc;
        HFONT hFont, hOldFont;
        LOGFONT lf;
        TEXTMETRIC tm;
        RECT rect;
        POINT posMaxH {};
        POINT posMaxW {};
        

        ZeroMemory(&lf, sizeof(lf));
        lf.lfPitchAndFamily = FF_MODERN | DEFAULT_PITCH;
        lf.lfHeight = textHight;
        hdc = BeginPaint(hWnd, &ps);
        hFont = CreateFontIndirect(&lf);
        hOldFont = (HFONT)SelectObject(hdc, hFont);

        FindBiggestCell(hdc, text_table, nRows, mColumns, posMaxH, posMaxW, lf);
        
        hFont = CreateFontIndirect(&lf);
        DeleteObject(SelectObject(hdc, hFont));

        DisplayText(hdc, text_table, nRows, mColumns);
        DisplayTableLines(hdc);

        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

// Рисование линии.
BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
    POINT pt;
    MoveToEx(hdc, x1, y1, &pt);
    return LineTo(hdc, x2, y2);
}

// Вычисление размеров клеток
void RecalculateTable()
{
    cellH = (wndH - TOOL_BAR_HIGHT) / nRows;
    cellW = wndW / mColumns;
}

// Поиск клетки самого большого текста таблицы
void FindBiggestCell(HDC hdc, std::vector<std::wstring> textTable, int nRows, int mColumns, POINT &posMaxH, POINT &posMaxW, LOGFONT &lf)
{
    RECT rect;
    HFONT hFont;
    int maxW = 0, maxH = 0;

    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < mColumns; j++)
        {
            rect = constRect;
            DrawText(hdc, textTable[i * mColumns + j].c_str(), -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_CALCRECT);

            if ((rect.right - rect.left >= maxW))
            {
                maxW = rect.right - rect.left;
                posMaxW.y = i;
                posMaxW.x = j;
            }

            if (rect.bottom - rect.top >= maxH)
            {
                maxH = rect.bottom - rect.top;
                posMaxH.y = i;
                posMaxH.x = j;
            }
        }
    }

    if (textTable[posMaxH.y * mColumns + posMaxH.x].size() > 0)
    {
        if (maxW <= cellW && maxH <= cellH)
        {
            while (maxW <= cellW && maxH <= cellH)
            {
                lf.lfHeight++;
                hFont = CreateFontIndirect(&lf);
                DeleteObject(SelectObject(hdc, hFont));

                rect = constRect;
                DrawText(hdc, textTable[posMaxH.y * mColumns + posMaxH.x].c_str(), -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_CALCRECT);
                maxH = rect.bottom - rect.top;

                rect = constRect;
                DrawText(hdc, textTable[posMaxW.y * mColumns + posMaxW.x].c_str(), -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_CALCRECT);
                maxW = rect.right - rect.left;
            }
            //lf.lfHeight--;
        }
        else
        {
            while ((maxW > cellW || maxH > cellH) && lf.lfHeight > 1)
            {
                lf.lfHeight--;
                hFont = CreateFontIndirect(&lf);
                DeleteObject(SelectObject(hdc, hFont));

                rect = constRect;
                DrawText(hdc, textTable[posMaxH.y * mColumns + posMaxH.x].c_str(), -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_CALCRECT);
                maxH = rect.bottom - rect.top;

                rect = constRect;
                DrawText(hdc, textTable[posMaxW.y * mColumns + posMaxW.x].c_str(), -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_CALCRECT);
                maxW = rect.right - rect.left;
            }
        }
    }
}

// Рисование текста
void DisplayText(HDC hdc, std::vector<std::wstring> textTable, int nRows, int mColumns)
{
    RECT rect;
    int x, y = TOOL_BAR_HIGHT;

    for (int i = 0; i < nRows; i++)
    {
        x = 0;
        for (int j = 0; j < mColumns; j++)
        {
            rect.left = x;
            rect.right = x + cellW;
            rect.top = y;
            rect.bottom = y + cellH;
            DrawText(hdc, textTable[i * mColumns + j].c_str(), -1, &rect, DT_CENTER | DT_WORDBREAK);

            x += cellW;
        }
        y += cellH;
    }
}

void DisplayTableLines(HDC hdc)
{
    int currY = TOOL_BAR_HIGHT;

    for (int i = 0; i < nRows; i++)
    {
        DrawLine(hdc, 0, currY, wndW, currY);
        currY += cellH;
    }
    DrawLine(hdc, 0, wndH, wndW, wndH);

    int currX = cellW;
    for (int i = 1; i < mColumns; i++)
    {
        DrawLine(hdc, currX, TOOL_BAR_HIGHT, currX, wndH);
        currX += cellW;
    }
    DrawLine(hdc, wndW, 0, wndW, wndH);
}

void InitializeMenu(HWND hwnd)
{
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(hFileMenu, MF_STRING, ON_OPEN_FILE, L"Open");
    SetMenu(hwnd, hMenu);
}

void InitializeOFN(HWND hwnd)
{
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFilter = L"*.txt";
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = L".\\Resources";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

int LoadData(wchar_t filename[MAX_FILE_NAME], std::vector<std::wstring> textTable)
{
    std::wifstream file(filename);
    file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

	if (!file.is_open()) 
    {
		return 1;
	}

	std::wstring line;

	if (std::getline(file, line)) 
        nRows = std::stoi(line);

	if (std::getline(file, line)) 
        mColumns = std::stoi(line);

    text_table.clear();

	while (std::getline(file, line)) 
    {
		if (!line.empty()) 
        {
			text_table.push_back(line);
		}
	}

    for (int i = text_table.size(); i < nRows * mColumns; i++)
        text_table.push_back(L"");

	file.close();

	return 0;
}