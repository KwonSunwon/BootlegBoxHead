#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include <stdio.h>

#include <Windows.h>

#include "game_manager.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void CALLBACK Enemy_spawn(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void Player_move();

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

int phase, spawn_count, difficulty;
BOOL key_buffer[4];

EnemyType e;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc,MemDC,PrintDC;
    HBITMAP BackBit, oldBackBit;

    HFONT hFont, oldFont;
    POINT mouse_pos;
    
    static RECT message_box, edit_box;
    static RECT rc;
    static TCHAR name[10] = L"BoxLike";
    static TCHAR start_message[11] = L"Start Game";
    static TCHAR Select[17] = L"Select Character";
    static TCHAR edit_button[11] = L"Map Editer";

    switch (iMessage)
    {
    case WM_CREATE:
        p.Set_Attack(PLAYER_STDATTACK); p.Set_Health(PLAYER_STDHEALTH); p.Set_Speed(PLAYER_STDSPEED);
        GetClientRect(hWnd, &rc);
        phase = PHASE_MENU;

        message_box.left = 200; message_box.top = 200; message_box.right = 600; message_box.bottom = 260;

        edit_box.left = 200; edit_box.top = 300; edit_box.right = 600; edit_box.bottom = 360;

        e.link = NULL; e.enemy_count = 0;

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

            if (edit_box.left < mouse_pos.x && edit_box.top < mouse_pos.y && edit_box.right > mouse_pos.x && edit_box.bottom > mouse_pos.y)
            {
                phase = PHASE_EDIT;

                InvalidateRect(hWnd, NULL, FALSE);
            }
        }

        break;

    case WM_CHAR:
        if (phase == PHASE_LOAD) 
        {
            if (wParam == 'p' || wParam == 'P') 
            {
                spawn_count = 0;
                SetTimer(hWnd, ENEMY_TIMER, ENEMY_TIMELAB, Enemy_spawn);
                phase = PHASE_PLAY;
            }
        }

        if (phase == PHASE_PLAY)
        {
            if (wParam == 'w' || wParam == 'W') 
            {

                key_buffer[UP] = TRUE;
            }

            if (wParam == 's' || wParam == 'S')
            {
                key_buffer[DOWN] = TRUE;
            }

            if (wParam == 'a' || wParam == 'A')
            {
                key_buffer[LEFT] = TRUE;
            }

            if (wParam == 'd' || wParam == 'D')
            {
                key_buffer[RIGHT] = TRUE;
            }

            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

    case WM_KEYUP:
        for (int i = 0;i < 4 ; i++) 
        {
            key_buffer[i] = FALSE;
        }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        MemDC = CreateCompatibleDC(hdc); PrintDC = CreateCompatibleDC(hdc);
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

            //게임 시작 버튼
            Rectangle(MemDC, message_box.left, message_box.top, message_box.right, message_box.bottom);
            DrawText(MemDC, start_message, lstrlen(start_message), &message_box, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            //맵 에디터 버튼
            Rectangle(MemDC, edit_box.left, edit_box.top, edit_box.right, edit_box.bottom);
            DrawText(MemDC, edit_button, lstrlen(edit_button), &edit_box, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SelectObject(MemDC, oldFont);
            DeleteObject(hFont);
        }

        if (phase == PHASE_LOAD) 
        {
            hFont = CreateFont(60, 40, 0, 0, FW_BOLD, 0, 0, 0, NULL, 0, 0, 0, 0, NULL);
            oldFont = (HFONT)SelectObject(MemDC, hFont);

            TextOut(MemDC, 25, 50, Select, lstrlen(Select));

            SelectObject(MemDC, oldFont);
            DeleteObject(hFont);
        }

        if (phase == PHASE_PLAY) 
        {
            Player_move();

            SelectObject(PrintDC, p.Get_Image());
            StretchBlt(MemDC, p.Get_Location().x - OBJECT_X_SIZE / 2, p.Get_Location().y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE, PrintDC, 0, 0, p.Get_Info().bmWidth, p.Get_Info().bmHeight, SRCCOPY);
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

void CALLBACK Enemy_spawn(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) 
{
    spawn_count++;

    if (spawn_count % ENEMY_SPAWN == 0) //적 스폰
    {
        if(e.enemy_count < ENEMY_MAXCOUNT) 
        {
            Enemy* newnode = (Enemy*)malloc(sizeof(Enemy));
            Enemy* tmp = e.link;

            //newnode의 set 함수를 이용해 초기화 
            newnode->Set_link(NULL);

            newnode->Init_enemy(MOB1);

            while (tmp != NULL) tmp = tmp->Get_link();

            tmp = newnode;

            e.enemy_count++;
        }
    }
}

void Player_move()
{
    if (key_buffer[UP])
    {
        p.Move_up();
    }

    if (key_buffer[DOWN])
    {
        p.Move_down();
    }

    if (key_buffer[LEFT])
    {
        p.Move_left();
    }

    if (key_buffer[RIGHT])
    {
        p.Move_right();
    }
}