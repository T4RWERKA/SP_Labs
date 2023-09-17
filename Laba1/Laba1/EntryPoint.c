#include <Windows.h>
#include <Windowsx.h>
#include <math.h>
#include <stdio.h>

RECT obj;

const int WND_WIDTH = 500;
const int WND_HEIGHT = 500;

const int RECT_WIDTH = 160;
const int RECT_HEIGHT = 160;
const int STEP = 10;

const int ACTIVE_OBJ_SPEED =  8;
const int INACTIVE_OBJ_SPEED = 2;
const float ANGLE = 45 * (3.141 / 180.0);
int xVelocity, yVelocity;


BOOL objControlling = FALSE;

POINT prevMousePosition;

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

#define WM_TIMER1 2
#define DEACTIVATION_TIME 5

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT paint_struct;
    HDC hdc;

    RECT currentWindowSize;
    GetClientRect(hWnd, &currentWindowSize);

    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_DOWN:
        case VK_S:
            obj.top += STEP;
            obj.bottom += STEP;
            break;
        case VK_LEFT:
        case VK_A:
            obj.right -= STEP;
            obj.left -= STEP;
            break;
        case VK_UP:
        case VK_W:
            obj.top -= STEP;
            obj.bottom -= STEP;
            break;
        case VK_RIGHT:
        case VK_D:
            obj.left += STEP;
            obj.right += STEP;
            break;
        default:
            break;
        }
        objControlling = FALSE;
        InvalidateRect(hWnd, NULL, TRUE);
    case WM_KEYUP:
        objControlling = TRUE;
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &paint_struct);
        obj.right = obj.left + RECT_WIDTH;
        obj.bottom = obj.top + RECT_HEIGHT;

        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(220,0,0));
        SetDCPenColor(hdc, RGB(0,0,255));

        Rectangle(hdc, obj.left, obj.top, obj.right, obj.bottom);
        EndPaint(hWnd, &paint_struct);
        break;
    case WM_LBUTTONDOWN:;
        POINT mousePosition;

        mousePosition.x = LOWORD(lParam);
        mousePosition.y = HIWORD(lParam);
        if (PtInRect(&obj, mousePosition))
        {
            prevMousePosition.x = GET_X_LPARAM(lParam);
            prevMousePosition.y = GET_Y_LPARAM(lParam);
            objControlling = FALSE;
        }
        break;
    case WM_MOUSEMOVE:
        if (GetAsyncKeyState(VK_LBUTTON))
        {
            POINT mousePosition;
            mousePosition.x = LOWORD(lParam);
            mousePosition.y = HIWORD(lParam);
            if (PtInRect(&obj, mousePosition))
            {
                obj.top += mousePosition.y - prevMousePosition.y;
                obj.left += mousePosition.x - prevMousePosition.x;
                obj.right = obj.left + RECT_WIDTH;
                obj.bottom = obj.top + RECT_HEIGHT;
            }
            InvalidateRect(hWnd, NULL, TRUE);

            prevMousePosition.x = mousePosition.x;
            prevMousePosition.y = mousePosition.y;
        }
        break;
    case WM_LBUTTONUP:
        mousePosition.x = LOWORD(lParam);
        mousePosition.y = HIWORD(lParam);
        if (PtInRect(&obj, mousePosition))
        {
            prevMousePosition.x = GET_X_LPARAM(lParam);
            prevMousePosition.y = GET_Y_LPARAM(lParam);
            objControlling = TRUE;
        }
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case  WM_TIMER1:
            if (objControlling)
            {
                obj.left += xVelocity;
                obj.top += yVelocity;
                obj.right = obj.left + RECT_WIDTH;
                obj.bottom = obj.top + RECT_HEIGHT;

                if (obj.left < 0 || obj.right > currentWindowSize.right)
                {
                    xVelocity *= -1;
                }
                if (obj.top < 0 || obj.bottom > currentWindowSize.bottom)
                {
                    yVelocity *= -1;
                }
            }
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void InitializeScene()
{
    obj.left = (WND_WIDTH - RECT_WIDTH) / 2;
    obj.top = (WND_HEIGHT - RECT_HEIGHT) / 2;
    obj.right = obj.left + RECT_WIDTH;
    obj.bottom = obj.top + RECT_HEIGHT;

    xVelocity = round(INACTIVE_OBJ_SPEED * cos(ANGLE));
    yVelocity = round(INACTIVE_OBJ_SPEED * sin(ANGLE));
}

void RegisterMainWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"MainWindowClass";
    wcex.hIconSm = wcex.hIcon;

    RegisterClassEx(&wcex);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd; MSG msg;

    RegisterMainWindow(hInstance);

    hWnd = CreateWindow(
        L"MainWindowClass",		            // им€ класс
        L"Laba1",		                    // им€ окна
        WS_OVERLAPPEDWINDOW,			    // режимы отображени€ окна
        CW_USEDEFAULT, CW_USEDEFAULT,       // положение окна по х и у
        WND_WIDTH, WND_HEIGHT,	            // ширина и высота окна
        0,						            // дескриптор родительского окна
        0,						            // дескриптор меню
        hInstance,				            // дескриптор экземпл€ра приложени€
        NULL
    );

    InitializeScene();

    if (!SetTimer(hWnd, WM_TIMER1, 1, (TIMERPROC)NULL))
        MessageBox(hWnd, "Timer2 null", L"Error Message", NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}