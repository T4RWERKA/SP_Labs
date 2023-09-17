#include <Windows.h>

RECT obj;

const int WND_WIDTH = 500;
const int WND_HEIGHT = 500;

const int RECT_WIDTH = 160;
const int RECT_HEIGHT = 160;
const int STEP = 10;

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT paint_struct;
    HDC hdc;

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
            //WallJump(hWnd);
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        //UpdateWindow(hWnd);
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &paint_struct);
        //SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(red, green, blue)));
        obj.right = obj.left + RECT_WIDTH;
        obj.bottom = obj.top + RECT_HEIGHT;
        //BitBlt(winDC, rectangle.left, rectangle.top, widthRect, heightRect, bmpDC, 0, 0, SRCCOPY);

        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(220,0,0));
        SetDCPenColor(hdc, RGB(0,0,255));

        Rectangle(hdc, obj.left, obj.top, obj.right, obj.bottom);
        EndPaint(hWnd, &paint_struct);
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

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}