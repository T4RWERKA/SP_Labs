#include <Windows.h>

const int WND_WIDTH = 500;
const int WND_HEIGHT = 500;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
        L"MainWindowClass",		            // ��� �����
        L"Laba1",		                    // ��� ����
        WS_OVERLAPPEDWINDOW,			    // ������ ����������� ����
        CW_USEDEFAULT, CW_USEDEFAULT,       // ��������� ���� �� � � �
        WND_WIDTH, WND_HEIGHT,	            // ������ � ������ ����
        0,						            // ���������� ������������� ����
        0,						            // ���������� ����
        hInstance,				            // ���������� ���������� ����������
        NULL
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}