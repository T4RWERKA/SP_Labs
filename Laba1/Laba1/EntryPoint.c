#include <Windows.h>
#include <Windowsx.h>
#include <math.h>
#include <stdio.h>
#pragma comment(lib, "Msimg32.lib")

RECT obj;

int objX = 100;
int objY = 100;
int sonicSize = 100;

const int WND_WIDTH = 500;
const int WND_HEIGHT = 500;

const int RECT_WIDTH = 160;
const int RECT_HEIGHT = 160;
const int STEP = 8;

const int ACTIVE_OBJ_SPEED =  8;
const int INACTIVE_OBJ_SPEED = 2;
const float ANGLE = 45 * (3.141 / 180.0);
int xVelocity, yVelocity;


BOOL objControlling = FALSE;

POINT prevMousePosition;

HBITMAP hBitmap;

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

#define WM_TIMER1 2
#define DEACTIVATION_TIME 5

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT paintStruct;
    HDC hdc, hObjDc;

    HDC hdcMem;
    HGDIOBJ oldBitmap;
    BITMAP bitmap;

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

        if (obj.left < 0) obj.left = 0;
        else if (obj.right > currentWindowSize.right) obj.left = currentWindowSize.right - RECT_WIDTH - 0;
        if (obj.top < 0) obj.top = 0;
        else if (obj.bottom > currentWindowSize.bottom) obj.top = currentWindowSize.bottom - RECT_HEIGHT - 0;

        objControlling = FALSE;
        InvalidateRect(hWnd, NULL, TRUE);
    case WM_KEYUP:
        objControlling = TRUE;
        break;
    case WM_PAINT:
    {
        HDC hdc, hsonicdc;

        hdc = BeginPaint(hWnd, &paintStruct);

        hsonicdc = CreateCompatibleDC(hdc);         
        SelectObject(hsonicdc, hBitmap);           
        TransparentBlt(                           
            hdc,                        
            obj.left,                      
            obj.top,                     
            RECT_WIDTH,                 
            RECT_HEIGHT,                
            hsonicdc,             
            0,                      
            0,                 
            sonicSize,       
            sonicSize,           
            RGB(0, 143, 0) 
        );

         // ќбновление окна.
        ValidateRect(hWnd, NULL, NULL);

        DeleteDC(hsonicdc);
        EndPaint(hWnd, &paintStruct);
        break;
    }
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
                if (!(obj.left < 0 || obj.right > currentWindowSize.right) &&
                    !(obj.top < 0 || obj.bottom > currentWindowSize.bottom))
                {
                    obj.top += mousePosition.y - prevMousePosition.y;
                    obj.left += mousePosition.x - prevMousePosition.x;
                    obj.right = obj.left + RECT_WIDTH;
                    obj.bottom = obj.top + RECT_HEIGHT;

                    if (obj.left < 0) obj.left = 1;
                    else if (obj.right > currentWindowSize.right) obj.left = currentWindowSize.right - RECT_WIDTH - 0;
                    if (obj.top < 0) obj.top = 1;
                    else if (obj.bottom > currentWindowSize.bottom) obj.top = currentWindowSize.bottom - RECT_HEIGHT - 1;
                }
                InvalidateRect(hWnd, NULL, TRUE);

                prevMousePosition.x = mousePosition.x;
                prevMousePosition.y = mousePosition.y;

            }
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
    case WM_MOUSEWHEEL:
        if (GetAsyncKeyState(VK_SHIFT))
        {
            if (!(obj.left < 0 || obj.right > currentWindowSize.right))
            {
                if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
                {
                    obj.left -= ACTIVE_OBJ_SPEED;
                    obj.right -= ACTIVE_OBJ_SPEED;
                }
                else
                {
                    obj.left += ACTIVE_OBJ_SPEED;
                    obj.right += ACTIVE_OBJ_SPEED;
                }
                if (obj.left < 0) obj.left = 1;
                else if (obj.right > currentWindowSize.right) obj.left = currentWindowSize.right - RECT_WIDTH - 0;
            }
        }
        else
        {
            if (!(obj.top < 0 || obj.bottom > currentWindowSize.bottom))
            {
                if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
                {
                    obj.top += ACTIVE_OBJ_SPEED;
                    obj.bottom += ACTIVE_OBJ_SPEED;
                }
                else
                {
                    obj.top -= ACTIVE_OBJ_SPEED;
                    obj.bottom -= ACTIVE_OBJ_SPEED;
                }
                if (obj.top < 0) obj.top = 1;
                else if (obj.bottom > currentWindowSize.bottom) obj.top = currentWindowSize.bottom - RECT_HEIGHT - 1;
            }
        }
        objControlling = TRUE;
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

    /*hBitmap = LoadImage(hInstance, L"sonicGreenBack.bmp", IMAGE_BITMAP, RECT_WIDTH, RECT_HEIGHT, LR_LOADFROMFILE);*/
    hBitmap = LoadImage(hInstance, L"sonicGreenBack.bmp", IMAGE_BITMAP, sonicSize, sonicSize, LR_LOADFROMFILE);

    if (hBitmap == NULL)
        MessageBoxW(hWnd, L"Failed to load image", L"Error", MB_OK);

    if (!SetTimer(hWnd, WM_TIMER1, 1, (TIMERPROC)NULL))
        MessageBox(hWnd, L"Timer1 null", L"Error Message", NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}