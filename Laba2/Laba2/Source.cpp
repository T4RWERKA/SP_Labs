#include <Windows.h>
#include <string>

#define MAX_LINES           15
#define MAX_ROWS            15
#define MAX_TEXTLEN         41
#define TOOL_HIGHT          30

const std::wstring startText = L"Привет мир!!!";
const int textHight = 50;

int wndH = 700, wndW = 1200;
int nLines = 7, mRows = 7;
int cellH, cellW;
std::wstring textTable[MAX_LINES][MAX_ROWS];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2);
void RecalculateTable();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd; MSG msg;
    WNDCLASSEX wcexmain;


    for (int i = 0; i < MAX_LINES; i++)
    {
        for (int j = 0; j < MAX_ROWS; j++)
        {
            textTable[i][j] = startText;
        }
    }

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
        int x, y = TOOL_HIGHT;
        POINT posMaxH = { 0, 0 };
        POINT posMaxW = { 0, 0 };
        int maxW = 0;
        int maxH = 0;
        const RECT constRect = { 0, 30, 0 + cellW, 30 + cellH };

        memset(&lf, 0, sizeof(lf));
        lf.lfPitchAndFamily = FF_MODERN | DEFAULT_PITCH;
        lf.lfHeight = textHight;
        hdc = BeginPaint(hWnd, &ps);
        hFont = CreateFontIndirect(&lf);
        hOldFont = (HFONT)SelectObject(hdc, hFont);

        for (int i = 0; i < nLines; i++)
        {
            for (int j = 0; j < mRows; j++)
            {
                rect = constRect;
                DrawText(hdc, textTable[i][j].c_str(), -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_CALCRECT);

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

        if (maxW <= cellW && maxH <= cellH)
        {
            while (maxW <= cellW && maxH <= cellH)
            {
                lf.lfHeight++;
                hFont = CreateFontIndirect(&lf);
                DeleteObject(SelectObject(hdc, hFont));

                rect = constRect;
                DrawText(hdc, textTable[posMaxH.y][posMaxH.x].c_str(), -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_CALCRECT);
                maxH = rect.bottom - rect.top;

                rect = constRect;
                DrawText(hdc, textTable[posMaxW.y][posMaxW.x].c_str(), -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_CALCRECT);
                maxW = rect.right - rect.left;
            }
            lf.lfHeight--;
        }
        else
        {
            while ((maxW > cellW || maxH > cellH) && lf.lfHeight > 1)
            {
                lf.lfHeight--;
                hFont = CreateFontIndirect(&lf);
                DeleteObject(SelectObject(hdc, hFont));

                rect = constRect;
                DrawText(hdc, textTable[posMaxH.y][posMaxH.x].c_str(), -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_CALCRECT);
                maxH = rect.bottom - rect.top;

                rect = constRect;
                DrawText(hdc, textTable[posMaxW.y][posMaxW.x].c_str(), -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_CALCRECT);
                maxW = rect.right - rect.left;
            }
        }
        hFont = CreateFontIndirect(&lf);
        DeleteObject(SelectObject(hdc, hFont));

        for (int i = 0; i < nLines; i++)
        {
            x = 0;
            for (int j = 0; j < mRows; j++)
            {
                rect.left = x;
                rect.right = x + cellW;
                rect.top = y;
                rect.bottom = y + cellH;
                DrawText(hdc, textTable[i][j].c_str(), -1, &rect, DT_CENTER | DT_WORDBREAK);

                x += cellW;
            }
            y += cellH;
        }

        int currY = TOOL_HIGHT;
        for (int i = 0; i < nLines; i++)
        {
            DrawLine(hdc, 0, currY, wndW, currY);
            currY += cellH;
        }
        DrawLine(hdc, 0, wndH, wndW, wndH);

        int currX = cellW;
        for (int i = 1; i < mRows; i++)
        {
            DrawLine(hdc, currX, TOOL_HIGHT, currX, wndH);
            currX += cellW;
        }
        DrawLine(hdc, wndW, 0, wndW, wndH);

        SelectObject(hdc, hOldFont);
        DeleteObject(hOldFont);
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

void RecalculateTable()
{
    cellH = (wndH - TOOL_HIGHT) / nLines;
    cellW = wndW / mRows;
}