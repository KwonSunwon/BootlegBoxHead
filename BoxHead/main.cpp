#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include <stdio.h>

#include "game_manager.h"
#include "resource.h"

// map global
Map map;
int g_mapEditSelector;

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

    static BOOL isEditMode;

    static BOOL LBClick;
    static BOOL RBClick;
    static POINT mouse;

    switch (iMessage)
    {
    case WM_CREATE:
        isEditMode = FALSE;
        LBClick = FALSE;
        RBClick = FALSE;
        break;

    case WM_COMMAND:
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN: // 1 key 임시 맵 수정모드 불러오기
            isEditMode = TRUE;
            map.on_editMode();
            if (!IsWindow(hDlg))
            {
                hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_MAPEDIT), hWnd, (DLGPROC)MapEditProc);
                ShowWindow(hDlg, SW_SHOW);
            }
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_LBUTTONDOWN:
        mouse = {LOWORD(lParam), HIWORD(lParam)};
        if (isEditMode)
        {
            mouse.x = mouse.x / TILE_SIZE;
            mouse.y = mouse.y / TILE_SIZE;
            map.tile_change(mouse, g_mapEditSelector);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

    case WM_RBUTTONDOWN:

        break;

    case WM_MOUSEMOVE:
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

    HBITMAP hBit;
    HWND hButton;

    static POINT size;
    static int mapId;
    static vector<BOOL> typeSelector;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        g_mapEditSelector = MAP_NONE;
        for (int i = 0; i < 4; ++i)
            typeSelector.push_back(FALSE);

        size = {50, 50};
        map.make_new_map(size);

        mapId = -1;

        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR1));
        hButton = GetDlgItem(hDlg, IDC_FLOOR1);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR2));
        hButton = GetDlgItem(hDlg, IDC_FLOOR2);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL));
        hButton = GetDlgItem(hDlg, IDC_WALL1);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL2));
        hButton = GetDlgItem(hDlg, IDC_WALL3);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BARREL));
        hButton = GetDlgItem(hDlg, IDC_WALL2);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK: // Save
            for (int i = 0; i < 4; ++i)
                map.enemy_type_change(i, typeSelector[i]);
            mapId = GetDlgItemInt(hDlg, IDC_INPUT, NULL, TRUE);
            map.save(mapId);
            break;

        case IDCANCEL:
            DestroyWindow(hDlg);
            break;

        case IDC_FLOOR1:
            g_mapEditSelector = MAP_FLOOR_TYPE1;
            break;
        case IDC_FLOOR2:
            g_mapEditSelector = MAP_FLOOR_TYPE2;
            break;
        case IDC_FLOOR3:
            break;
        case IDC_WALL1:
            g_mapEditSelector = MAP_WALL_TYPE1;
            break;
        case IDC_WALL2:
            g_mapEditSelector = MAP_WALL_TYPE2;
            break;
        case IDC_WALL3:
            g_mapEditSelector = MAP_WALL_TYPE3;
            break;
        case IDC_PLAYER:
            g_mapEditSelector = MAP_PLAYER_SPAWN_POINT;
            break;
        case IDC_ENEMY:
            g_mapEditSelector = MAP_ENEMY_SPAWN_POINT;
            break;
        case IDC_REMOVE:
            g_mapEditSelector = MAP_REMOVE;
            break;

        case IDC_TYPE1:
            typeSelector[0] = 1 - typeSelector[0];
            break;
        case IDC_TYPE2:
            typeSelector[1] = 1 - typeSelector[1];
            break;
        case IDC_TYPE3:
            typeSelector[2] = 1 - typeSelector[2];
            break;
        case IDC_TYPE4:
            typeSelector[3] = 1 - typeSelector[3];
            break;

        case IDC_LOAD:
            mapId = GetDlgItemInt(hDlg, IDC_INPUT, NULL, TRUE);
            map.load(mapId);
            typeSelector = map.get_enemy_type();
            for (int i = 0; i < 4; ++i)
                SendMessage(hDlg, IDC_TYPE1, BM_SETCHECK, typeSelector[i]);
            SendMessage(hWnd, WM_COMMAND, 0, 0);
            break;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hDlg);
        break;
    }
    return 0;
}