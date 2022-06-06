#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include <stdio.h>

#include <Windows.h>

#include "game_manager.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

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

Player p;

int phase;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc,MemDC;
    HBITMAP BackBit, oldBackBit;

    HFONT hFont, oldFont;
    POINT mouse_pos;

    static RECT rc, message_box;
    static TCHAR name[10] = L"BoxLike";
    static TCHAR start_message[11] = L"Start Game";

    switch (iMessage)
    {
    case WM_CREATE:
        p.Set_Attack(PLAYER_STDATTACK); p.Set_Health(PLAYER_STDHEALTH); p.Set_Speed(PLAYER_STDSPEED);
        GetClientRect(hWnd, &rc);
        phase = PHASE_MENU;
        break;

    case WM_LBUTTONDOWN:
        mouse_pos.x = LOWORD(lParam); mouse_pos.y = HIWORD(lParam);

        if (phase == PHASE_MENU) 
        {
            if (message_box.left < mouse_pos.x && message_box.top < mouse_pos.y && message_box.right > mouse_pos.x && message_box.bottom > mouse_pos.y)
            {
                phase = PHASE_LOAD;
                InvalidateRect(hWnd, NULL, FALSE);
            }
        }

        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        MemDC = CreateCompatibleDC(hdc);
        BackBit = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);

        oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);

        PatBlt(MemDC, 0, 0, rc.right, rc.bottom, WHITENESS);

        if (phase == PHASE_MENU) 
        {
            hFont = CreateFont(120, 90, 0, 0, FW_BOLD, 0, 0, 0, NULL, 0, 0, 0, 0, NULL);
            oldFont = (HFONT)SelectObject(MemDC, hFont);

            TextOut(MemDC, 25, 50, name, lstrlen(name));

            SelectObject(MemDC, oldFont);
            DeleteObject(hFont);

            hFont = CreateFont(50, 30, 0, 0, FW_BOLD, 0, 0, 0, NULL, 0, 0, 0, 0, NULL);
            oldFont = (HFONT)SelectObject(MemDC, hFont);

            message_box.left = 200; message_box.top = 200; message_box.right = 600; message_box.bottom = 260;

            Rectangle(MemDC, 200, 200, 600, 260);
            DrawText(MemDC, start_message, lstrlen(start_message), &message_box, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SelectObject(MemDC, oldFont);
            DeleteObject(hFont);
        }

        //더블 버퍼링
        BitBlt(hdc, 0, 0, rc.right, rc.bottom, MemDC, 0, 0, SRCCOPY);

        SelectObject(MemDC, oldBackBit);
        DeleteDC(MemDC); DeleteObject(BackBit);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}