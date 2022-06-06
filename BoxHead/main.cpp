#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include <stdio.h>

#include "game_manager.h"
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MapEditProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0))
    {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static HDC mdc, tmpDc;
    static HBITMAP backBit;
    static RECT bufferRT;

    static HWND hDlg;

    static Map map;
    static BOOL isEditMode;

    switch (iMessage)
    {
    case WM_CREATE:
        isEditMode = FALSE;

        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN: // 1 key 임시 맵 수정모드 불러오기
            isEditMode = TRUE;
            if (!IsWindow(hDlg))
            {
                hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_MAPEDIT), hWnd, (DLGPROC)MapEditProc);
                ShowWindow(hDlg, SW_SHOW);
            }
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_LBUTTONDOWN:
        map.load(100);
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_RBUTTONDOWN:
        map.load(200);
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &bufferRT);
        mdc = CreateCompatibleDC(hdc);
        backBit = CreateCompatibleBitmap(hdc, bufferRT.right, bufferRT.bottom);
        SelectObject(mdc, (HBITMAP)backBit);
        PatBlt(mdc, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);

        // Draw, using mdc
        map.draw(mdc);

        GetClientRect(hWnd, &bufferRT);
        BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, mdc, 0, 0, SRCCOPY);
        DeleteObject(backBit);
        DeleteDC(mdc);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK MapEditProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hWnd = GetParent(hDlg);

    int selector;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        selector = MAP_NONE;
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            break;
        case IDCANCEL:
            DestroyWindow(hDlg);
            break;

        case IDC_FLOOR1:
            selector = MAP_FLOOR_TYPE1;
            break;
        case IDC_FLOOR2:
            selector = MAP_FLOOR_TYPE2;
            break;
        case IDC_FLOOR3:
            break;
        case IDC_WALL1:
            selector = MAP_WALL_TYPE1;
            break;
        case IDC_WALL2:
            selector = MAP_WALL_TYPE2;
            break;
        case IDC_WALL3:
            selector = MAP_WALL_TYPE3;
            break;
        case IDC_PLAYER:
            selector = MAP_PLAYER_SPAWN_POINT;
            break;
        case IDC_ENEMY:
            selector = MAP_ENEMY_SPAWN_POINT;
            break;
        case IDC_REMOVE:
            selector = MAP_REMOVE;
            break;

        case IDC_TYPE1:
            break;
        case IDC_TYPE2:
            break;
        case IDC_TYPE3:
            break;
        case IDC_TYPE4:
            break;

        case IDC_INPUT:
            break;

        case IDC_LOAD:
            break;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hDlg);
        break;
    }
    return 0;
}