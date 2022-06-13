#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include <stdio.h>
#include "game_manager.h"
#include "resource.h"

//TODO
/*
Å¸¿ö »ý¼º½Ã¿¡ À§Ä¡ ¼³Á¤ÇØÁÖ±â
Àû ÀÌµ¿ Å¸ÀÌ¸Ó ¼öÁ¤ ¹× Àû ½ºÆù À§Ä¡ ¼³Á¤
*/


// map global
Map map;
int g_mapEditSelector;

//Object global
Player p;
int phase, spawn_count, difficulty;
BOOL key_buffer[4];
EnemyType Mob1, Mob2, Mob3, Mob4, Boss;
Bullet* B;
Tower T[MAX_TOWER_COUNT];
int tower_count, tower_way_set, tower_id_set;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void CALLBACK Enemy_spawn(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void CALLBACK Tower_Oparate(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void CALLBACK Bullet_fly(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void CALLBACK MOB1_Move(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void CALLBACK MOB2_Move(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void CALLBACK MOB3_Move(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void CALLBACK MOB4_Move(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void CALLBACK BOSS_Move(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
BOOL CALLBACK MapEditProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void Player_move();
void BOMB_target(Tower,Enemy);

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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1280, 960, NULL, (HMENU)NULL, hInstance, NULL);
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
    HDC hdc, MemDC, PrintDC;
    HBITMAP BackBit, oldBackBit;

    HBRUSH hBrush, oldBrush;

    HFONT hFont, oldFont;
    POINT mouse_pos, b_pos;

    vector<BOOL> v;
    Enemy* e_tmp;
    Bullet* b_tmp;

    static RECT message_box, edit_box;
    static RECT rc;
    static TCHAR name[10] = L"BoxLike";
    static TCHAR start_message[11] = L"Start Game";
    static TCHAR Select[17] = L"Select Character";
    static TCHAR edit_button[11] = L"Map Editer";

    static HWND hDlg;

    static BOOL LBClick;
    static BOOL RBClick;
    static BOOL building;
    static POINT mouse;

    RECT ClientRT = {0, 0, 1280, 960};

    switch (iMessage)
    {
    case WM_CREATE:
        AdjustWindowRect(&ClientRT, WS_OVERLAPPEDWINDOW, FALSE);
        SetWindowPos(hWnd, NULL, 0, 0, ClientRT.right - ClientRT.left, ClientRT.bottom - ClientRT.top, NULL);

        p.Set_Attack(PLAYER_STDATTACK);
        p.Set_Health(PLAYER_STDHEALTH);
        p.Set_Speed(PLAYER_STDSPEED);
        p.Set_Weapon(PISTOL);

        GetClientRect(hWnd, &rc);
        phase = PHASE_MENU;

        message_box.left = 200;
        message_box.top = 200;
        message_box.right = 600;
        message_box.bottom = 260;

        edit_box.left = 200;
        edit_box.top = 300;
        edit_box.right = 600;
        edit_box.bottom = 360;

        Mob1.enemy_count = 0; Mob2.enemy_count = 0; Mob3.enemy_count = 0; Mob4.enemy_count = 0;
        Mob1.link = NULL; Mob2.link = NULL; Mob3.link = NULL; Mob4.link = NULL;

        tower_way_set = IDB_UP;
        tower_id_set = ID_HEAL_TOWER;

        tower_count = 0;
        building = FALSE;
        break;

    case WM_LBUTTONDOWN:
        mouse_pos.x = LOWORD(lParam);
        mouse_pos.y = HIWORD(lParam);

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
                map.on_editMode();
                if (!IsWindow(hDlg))
                {
                    hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_MAPEDIT), hWnd, (DLGPROC)MapEditProc);
                    ShowWindow(hDlg, SW_SHOW);
                }
                InvalidateRect(hWnd, NULL, FALSE);
            }
        }

        if (phase == PHASE_EDIT)
        {
            mouse_pos.x = mouse_pos.x / TILE_SIZE;
            mouse_pos.y = mouse_pos.y / TILE_SIZE;
            if (g_mapEditSelector == MAP_PLAYER_SPAWN_POINT)
            {
                map.set_player_spawn(mouse_pos);
            }
            else if (g_mapEditSelector == MAP_ENEMY_SPAWN_POINT)
            {
                if (map.set_enemy_spawn(mouse_pos) == 4)
                {
                    // Àû ½ºÆù À§Ä¡ 4°³ ÃÊ°ú ÁöÁ¤ ºÒ°¡´É
                }
            }
            else if (g_mapEditSelector == MAP_REMOVE)
            {
                map.remove_spawn_point(mouse_pos);
            }
            else
            {
                map.tile_change(mouse_pos, g_mapEditSelector);
            }
            InvalidateRect(hWnd, NULL, FALSE);
        }

        break;

    case WM_CHAR:
        if (phase == PHASE_LOAD)
        {
            if (wParam == 'p' || wParam == 'P')
            {
                spawn_count = 0;
                p.Set_Location(map.get_player_spawn());
                SetTimer(hWnd, ENEMY_TIMER, ENEMY_TIMELAB, Enemy_spawn);
                //SetTimer(hWnd, MOB1_TIMER, MOB_TIMELAB, MOB1_Move);
                SetTimer(hWnd, TOWER_TIMER, TOWER_TIMELAB, Tower_Oparate);
                SetTimer(hWnd, BULLET_TIMER, BULLET_TIMELAB, Bullet_fly);
                phase = PHASE_PLAY;
            }
        }

        if (phase == PHASE_PLAY)
        {
            if (wParam == 'w' || wParam == 'W')
            {
                POINT Virtual_pos = p.Get_Location();

                Virtual_pos.y -= p.Get_Speed();

                if (map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE1 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE2)
                {
                    key_buffer[UP] = TRUE;
                }
            }

            if (wParam == 's' || wParam == 'S')
            {
                POINT Virtual_pos = p.Get_Location();

                Virtual_pos.y += p.Get_Speed();

                if (map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE1 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE2)
                {
                    key_buffer[DOWN] = TRUE;
                }
            }

            if (wParam == 'a' || wParam == 'A')
            {
                POINT Virtual_pos = p.Get_Location();

                Virtual_pos.x -= p.Get_Speed();

                if (map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE1 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE2)
                {
                    key_buffer[LEFT] = TRUE;
                }
            }

            if (wParam == 'd' || wParam == 'D')
            {
                POINT Virtual_pos = p.Get_Location();

                Virtual_pos.x += p.Get_Speed();

                if (map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE1 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE2)
                {
                    key_buffer[RIGHT] = TRUE;
                }
            }

            if (wParam == 't' || wParam == 'T')
            {
                if (!building && tower_count < MAX_TOWER_COUNT) building = TRUE;

                else if (building) building = FALSE;
            }
            
            if (wParam == 'q' || wParam == 'Q')
            {
                if (building)
                {
                    if (tower_id_set == ID_HEAL_TOWER) tower_id_set = ID_STD_TOWER;

                    else if (tower_id_set == ID_STD_TOWER) tower_id_set = ID_SNIPE_TOWER;

                    else if (tower_id_set == ID_SNIPE_TOWER) tower_id_set = ID_BOMB_TOWER;

                    else if (tower_id_set == ID_BOMB_TOWER) tower_id_set = ID_HEAL_TOWER;
                }
            }

            if (wParam == 'r' || wParam == 'R')
            {
                if (building)
                {
                    if (tower_way_set == IDB_UP) tower_way_set = IDB_RIGHT;

                    else if (tower_way_set == IDB_RIGHT) tower_way_set = IDB_DOWN;

                    else if (tower_way_set == IDB_DOWN) tower_way_set = IDB_LEFT;

                    else if (tower_way_set == IDB_LEFT) tower_way_set = IDB_UP;
                }
            }

            if (wParam == 'e' || wParam == 'E')
            {
                if (building)
                {
                    T[tower_count].Set_way(tower_way_set);
                    T[tower_count].Set_id(tower_id_set);

                    tower_count++;

                    building = FALSE;
                }
            }

            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

    case WM_KEYUP:
        for (int i = 0; i < 4; i++)
        {
            key_buffer[i] = FALSE;
        }
        break;

    case WM_COMMAND:
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_KEYDOWN:
        if (phase == PHASE_PLAY)
        {
            if (wParam == VK_LEFT)
            {
                switch (p.Get_Weapon_id())
                {
                case PISTOL:
                    p.Shot_Pistol(B,IDB_LEFT);
                    break;
                case RIFLE:
                    p.Shot_Rifle(B,IDB_LEFT);
                    break;
                case SHOTGUN:
                    p.Shot_Shotgun(B,IDB_LEFT);
                    break;
                case SNIPER:
                    p.Shot_Sniper(B,IDB_LEFT);
                    break;
                }
            }

            if (wParam == VK_RIGHT)
            {
                switch (p.Get_Weapon_id())
                {
                case PISTOL:
                    p.Shot_Pistol(B, IDB_RIGHT);
                    break;
                case RIFLE:
                    p.Shot_Rifle(B, IDB_RIGHT);
                    break;
                case SHOTGUN:
                    p.Shot_Shotgun(B, IDB_RIGHT);
                    break;
                case SNIPER:
                    p.Shot_Sniper(B, IDB_RIGHT);
                    break;
                }
            }

            if (wParam == VK_UP)
            {
                switch (p.Get_Weapon_id())
                {
                case PISTOL:
                    p.Shot_Pistol(B, IDB_UP);
                    break;
                case RIFLE:
                    p.Shot_Rifle(B, IDB_UP);
                    break;
                case SHOTGUN:
                    p.Shot_Shotgun(B, IDB_UP);
                    break;
                case SNIPER:
                    p.Shot_Sniper(B, IDB_UP);
                    break;
                }
            }

            if (wParam == VK_DOWN)
            {
                switch (p.Get_Weapon_id())
                {
                case PISTOL:
                    p.Shot_Pistol(B, IDB_DOWN);
                    break;
                case RIFLE:
                    p.Shot_Rifle(B, IDB_DOWN);
                    break;
                case SHOTGUN:
                    p.Shot_Shotgun(B, IDB_DOWN);
                    break;
                case SNIPER:
                    p.Shot_Sniper(B, IDB_DOWN);
                    break;
                }
            }
        }

        InvalidateRect(hWnd, NULL, FALSE);

        break;

    case WM_RBUTTONDOWN:

        break;

    case WM_MOUSEMOVE:
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        MemDC = CreateCompatibleDC(hdc);
        PrintDC = CreateCompatibleDC(hdc);
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

            //start ¸Þ´º ¹öÆ°
            Rectangle(MemDC, message_box.left, message_box.top, message_box.right, message_box.bottom);
            DrawText(MemDC, start_message, lstrlen(start_message), &message_box, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            //¸Ê ¿¡µðÆ® ¹öÆ°
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
            // ¸Ê Ãâ·Â
            map.draw(MemDC);

            //ÇÃ·¹ÀÌ¾î Ãâ·Â
            Player_move();

            SelectObject(PrintDC, p.Get_Image());
            StretchBlt(MemDC, p.Get_Location().x - OBJECT_X_SIZE / 2, p.Get_Location().y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE, PrintDC, 0, 0, p.Get_Info().bmWidth, p.Get_Info().bmHeight, SRCCOPY);

            //Àû Ãâ·Â
            v = map.get_enemy_type();

            if (v[0])
            {
                e_tmp = Mob1.link;

                SelectObject(PrintDC, e_tmp->Get_Image());

                while (e_tmp != NULL)
                {
                    StretchBlt(MemDC, e_tmp->Get_Location().x - OBJECT_X_SIZE / 2, e_tmp->Get_Location().y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE, PrintDC, 0, 0, e_tmp->Get_Info().bmWidth, e_tmp->Get_Info().bmHeight, SRCCOPY);
                    
                    e_tmp = e_tmp->Get_link();
                }
            }

            if (v[1])
            {
                e_tmp = Mob2.link;

                SelectObject(PrintDC, e_tmp->Get_Image());

                while (e_tmp != NULL)
                {
                    StretchBlt(MemDC, e_tmp->Get_Location().x - OBJECT_X_SIZE / 2, e_tmp->Get_Location().y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE, PrintDC, 0, 0, e_tmp->Get_Info().bmWidth, e_tmp->Get_Info().bmHeight, SRCCOPY);

                    e_tmp = e_tmp->Get_link();
                }
            }

            if (v[2])
            {
                e_tmp = Mob3.link;

                SelectObject(PrintDC, e_tmp->Get_Image());

                while (e_tmp != NULL)
                {
                    StretchBlt(MemDC, e_tmp->Get_Location().x - OBJECT_X_SIZE / 2, e_tmp->Get_Location().y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE, PrintDC, 0, 0, e_tmp->Get_Info().bmWidth, e_tmp->Get_Info().bmHeight, SRCCOPY);

                    e_tmp = e_tmp->Get_link();
                }
            }

            if (v[3])
            {
                e_tmp = Mob4.link;

                SelectObject(PrintDC, e_tmp->Get_Image());

                while (e_tmp != NULL)
                {
                    StretchBlt(MemDC, e_tmp->Get_Location().x - OBJECT_X_SIZE / 2, e_tmp->Get_Location().y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE, PrintDC, 0, 0, e_tmp->Get_Info().bmWidth, e_tmp->Get_Info().bmHeight, SRCCOPY);

                    e_tmp = e_tmp->Get_link();
                }
            }

            if (v[4])
            {
                e_tmp = Boss.link;

                SelectObject(PrintDC, e_tmp->Get_Image());

                while (e_tmp != NULL)
                {
                    StretchBlt(MemDC, e_tmp->Get_Location().x - OBJECT_X_SIZE / 2, e_tmp->Get_Location().y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE, PrintDC, 0, 0, e_tmp->Get_Info().bmWidth, e_tmp->Get_Info().bmHeight, SRCCOPY);

                    e_tmp = e_tmp->Get_link();
                }
            }

            //Å¸¿ö Ãâ·Â
            for (int i = 0; i < tower_count; i++)
            {
                SelectObject(PrintDC, T[i].Get_Image());

                StretchBlt(MemDC, T[i].Get_Location().x - OBJECT_X_SIZE / 2, T[i].Get_Location().y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE, PrintDC, 0, 0, T[i].Get_Info().bmWidth, T[i].Get_Info().bmHeight, SRCCOPY);
            }

            //ÃÑ¾Ë Ãâ·Â
            b_tmp = B;

            hBrush = CreateSolidBrush(RGB(255, 127, 0));
            oldBrush = (HBRUSH)SelectObject(MemDC, hBrush);

            while (b_tmp != NULL)
            {
                b_pos = b_tmp->Get_Location();

                Rectangle(MemDC, b_pos.x - 3, b_pos.y - 3, b_pos.x + 3, b_pos.y + 3);

                b_tmp = b_tmp->Get_Rlink();
            }

            SelectObject(MemDC, oldBrush);
            DeleteObject(hBrush);

        }

        if (phase == PHASE_EDIT)
        {
            map.draw(MemDC);
        }

        BitBlt(hdc, 0, 0, rc.right, rc.bottom, MemDC, 0, 0, SRCCOPY);

        SelectObject(MemDC, oldBackBit);
        DeleteDC(MemDC);
        DeleteObject(BackBit);
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
    Enemy *tmp;
    int all_mob_amount = Mob1.enemy_count + Mob2.enemy_count + Mob3.enemy_count + Mob4.enemy_count;


    if (spawn_count % ENEMY_SPAWN == 0)
    {
        if (all_mob_amount < ENEMY_MAXCOUNT)
        {
            Enemy *newnode = (Enemy *)malloc(sizeof(Enemy));
            vector<BOOL> v = map.get_enemy_type();
            
            int selec;

            while (1)
            {
                selec = rand() % 4;
                if (v[0] && selec == 0)
                {
                    printf("¸÷ 1 ½ºÆù\n");
                    tmp = Mob1.link;

                    newnode->Set_link(NULL);

                    newnode->Init_enemy(MOB1);

                    Mob1.enemy_count++;
                    break;
                }

                if (v[1] && selec == 1)
                {
                    printf("¸÷ 2 ½ºÆù\n");
                    tmp = Mob2.link;

                    newnode->Set_link(NULL);

                    newnode->Init_enemy(MOB2);

                    Mob2.enemy_count++;
                    break;
                }

                if (v[2] && selec == 2)
                {
                    printf("¸÷ 3 ½ºÆù\n");
                    tmp = Mob3.link;

                    newnode->Set_link(NULL);

                    newnode->Init_enemy(MOB3);

                    Mob3.enemy_count++;
                    break;
                }

                if (v[3] && selec == 3)
                {
                    printf("¸÷ 4 ½ºÆù\n");
                    tmp = Mob4.link;

                    newnode->Set_link(NULL);

                    newnode->Init_enemy(MOB4);

                    Mob4.enemy_count++;
                    break;
                }
            }

            while (tmp != NULL)
                tmp = tmp->Get_link();

            tmp = newnode;

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

void CALLBACK MOB1_Move(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    POINT p_pos = p.Get_Location();
    POINT e_pos;
    Enemy *e_mob = Mob1.link;

    while (e_mob != NULL)
    {
        e_pos = e_mob->Get_Location();

        if (p_pos.y > e_pos.y)
        {
            e_mob->Move_down();
        }

        if (p_pos.y < e_pos.y)
        {
            e_mob->Move_up();
        }

        if (p_pos.x > e_pos.x)
        {
            e_mob->Move_right();
        }

        if (p_pos.x < e_pos.x)
        {
            e_mob->Move_left();
        }


        e_mob = e_mob->Get_link();
    }
}

void CALLBACK MOB2_Move(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    POINT p_pos = p.Get_Location();
    POINT e_pos;
    Enemy *e_mob = Mob2.link;

    while (e_mob != NULL)
    {

        e_pos = e_mob->Get_Location();

        if (p_pos.y > e_pos.y)
        {
            e_mob->Move_down();
        }

        if (p_pos.y < e_pos.y)
        {
            e_mob->Move_up();
        }

        if (p_pos.x > e_pos.x)
        {
            e_mob->Move_right();
        }

        if (p_pos.x < e_pos.x)
        {
            e_mob->Move_left();
        }


        e_mob = e_mob->Get_link();
    }
}

void CALLBACK MOB3_Move(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    POINT p_pos = p.Get_Location();
    POINT e_pos;
    Enemy *e_mob = Mob3.link;

    while (e_mob != NULL)
    {

        e_pos = e_mob->Get_Location();

        if (p_pos.y > e_pos.y)
        {
            e_mob->Move_down();
        }

        if (p_pos.y < e_pos.y)
        {
            e_mob->Move_up();
        }

        if (p_pos.x > e_pos.x)
        {
            e_mob->Move_right();
        }

        if (p_pos.x < e_pos.x)
        {
            e_mob->Move_left();
        }


        e_mob = e_mob->Get_link();
    }
}

void CALLBACK MOB4_Move(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    POINT p_pos = p.Get_Location();
    POINT e_pos;
    Enemy *e_mob = Mob4.link;

    while (e_mob != NULL)
    {
        if (e_mob->Get_id() == MOB4)
        {
            e_pos = e_mob->Get_Location();

            if (p_pos.y > e_pos.y)
            {
                e_mob->Move_down();
            }

            if (p_pos.y < e_pos.y)
            {
                e_mob->Move_up();
            }

            if (p_pos.x > e_pos.x)
            {
                e_mob->Move_right();
            }

            if (p_pos.x < e_pos.x)
            {
                e_mob->Move_left();
            }
        }

        e_mob = e_mob->Get_link();
    }
}

void CALLBACK Tower_Oparate(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    for (int i = 0; i < tower_count;i++)
    {
        switch (T[i].Get_id())
        {
        case ID_HEAL_TOWER:
            T[i].Heal_target(p);
            break;
        case ID_BOMB_TOWER:
            //ÀûÀ» Á¤ÇØ¾ß ÇÏ´Âµ¥..
            break;
        case ID_SNIPE_TOWER:
            T[i].Tower_Oparate(B);
            break;
        case ID_STD_TOWER:
            T[i].Tower_Oparate(B);
            break;
        }
    }
}

void CALLBACK BOSS_Move(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    POINT p_pos = p.Get_Location();
    POINT e_pos;
    Enemy *e_mob = Boss.link;

    while (e_mob != NULL)
    {

        e_pos = e_mob->Get_Location();

        if (p_pos.y > e_pos.y)
        {
            e_mob->Move_down();
        }

        if (p_pos.y < e_pos.y)
        {
            e_mob->Move_up();
        }

        if (p_pos.x > e_pos.x)
        {
            e_mob->Move_right();
        }

        if (p_pos.x < e_pos.x)
        {
            e_mob->Move_left();
        }


        e_mob = e_mob->Get_link();
    }
}


void CALLBACK Bullet_fly(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    Bullet* tmp, *del;
    Enemy* e_tmp;
    POINT n_pos;
    tmp = B;

    while (tmp != NULL)
    {
        n_pos = tmp->Get_Location();

        switch (tmp->Get_way())
        {
        case IDB_UP:
            n_pos.y -= 2;
            tmp->Set_Location(n_pos);
            break;
        case IDB_DOWN:
            n_pos.y += 2;
            tmp->Set_Location(n_pos);
            break;
        case IDB_RIGHT:
            n_pos.x += 2;
            tmp->Set_Location(n_pos);
            break;
        case IDB_LEFT:
            n_pos.x -= 2;
            tmp->Set_Location(n_pos);
            break;
        }

        //Àû°ú Ãæµ¹ÆÇÁ¤
        if (map.get_enemy_type()[0])
        {
            e_tmp = Mob1.link;

            while (e_tmp != NULL)
            {
                if (n_pos.x > (e_tmp->Get_Location().x - OBJECT_X_SIZE / 2) && n_pos.x < (e_tmp->Get_Location().x + OBJECT_X_SIZE / 2) && n_pos.y >(e_tmp->Get_Location().y - OBJECT_Y_SIZE / 2) && n_pos.y < (e_tmp->Get_Location().y + OBJECT_Y_SIZE / 2))
                {
                    tmp->Deliver_damage(e_tmp);

                    if (tmp->Get_type() != SNIPE_BULLET)    //½º³ªÀÌÆÛ´Â °üÅë
                    {
                        tmp->Get_Llink()->Set_Rlink(tmp->Get_Rlink());
                        tmp->Get_Rlink()->Set_Llink(tmp->Get_Llink());

                        del = tmp;
                        tmp = tmp->Get_Rlink();

                        free(del);
                    }
                }

                e_tmp = e_tmp->Get_link();
            }
        }

        if (map.get_enemy_type()[1])
        {
            e_tmp = Mob2.link;

            while (e_tmp != NULL)
            {
                if (n_pos.x > (e_tmp->Get_Location().x - OBJECT_X_SIZE / 2) && n_pos.x < (e_tmp->Get_Location().x + OBJECT_X_SIZE / 2) && n_pos.y >(e_tmp->Get_Location().y - OBJECT_Y_SIZE / 2) && n_pos.y < (e_tmp->Get_Location().y + OBJECT_Y_SIZE / 2))
                {
                    tmp->Deliver_damage(e_tmp);

                    if (tmp->Get_type() != SNIPE_BULLET)
                    {
                        tmp->Get_Llink()->Set_Rlink(tmp->Get_Rlink());
                        tmp->Get_Rlink()->Set_Llink(tmp->Get_Llink());

                        del = tmp;
                        tmp = tmp->Get_Rlink();

                        free(del);
                    }
                }

                e_tmp = e_tmp->Get_link();
            }
        }

        if (map.get_enemy_type()[2])
        {
            e_tmp = Mob3.link;

            while (e_tmp != NULL)
            {
                if (n_pos.x > (e_tmp->Get_Location().x - OBJECT_X_SIZE / 2) && n_pos.x < (e_tmp->Get_Location().x + OBJECT_X_SIZE / 2) && n_pos.y >(e_tmp->Get_Location().y - OBJECT_Y_SIZE / 2) && n_pos.y < (e_tmp->Get_Location().y + OBJECT_Y_SIZE / 2))
                {
                    tmp->Deliver_damage(e_tmp);

                    if (tmp->Get_type() != SNIPE_BULLET)
                    {
                        tmp->Get_Llink()->Set_Rlink(tmp->Get_Rlink());
                        tmp->Get_Rlink()->Set_Llink(tmp->Get_Llink());

                        del = tmp;
                        tmp = tmp->Get_Rlink();

                        free(del);
                    }
                }

                e_tmp = e_tmp->Get_link();
            }
        }

        if (map.get_enemy_type()[3])
        {
            e_tmp = Mob4.link;

            while (e_tmp != NULL)
            {
                if (n_pos.x > (e_tmp->Get_Location().x - OBJECT_X_SIZE / 2) && n_pos.x < (e_tmp->Get_Location().x + OBJECT_X_SIZE / 2) && n_pos.y >(e_tmp->Get_Location().y - OBJECT_Y_SIZE / 2) && n_pos.y < (e_tmp->Get_Location().y + OBJECT_Y_SIZE / 2))
                {
                    tmp->Deliver_damage(e_tmp);

                    if (tmp->Get_type() != SNIPE_BULLET)
                    {
                        tmp->Get_Llink()->Set_Rlink(tmp->Get_Rlink());
                        tmp->Get_Rlink()->Set_Llink(tmp->Get_Llink());

                        del = tmp;
                        tmp = tmp->Get_Rlink();

                        free(del);
                    }
                }

                e_tmp = e_tmp->Get_link();
            }
        }

        if (map.get_enemy_type()[4])
        {
            e_tmp = Boss.link;

            while (e_tmp != NULL)
            {
                if (n_pos.x > (e_tmp->Get_Location().x - OBJECT_X_SIZE / 2) && n_pos.x < (e_tmp->Get_Location().x + OBJECT_X_SIZE / 2) && n_pos.y >(e_tmp->Get_Location().y - OBJECT_Y_SIZE / 2) && n_pos.y < (e_tmp->Get_Location().y + OBJECT_Y_SIZE / 2))
                {
                    tmp->Deliver_damage(e_tmp);

                    if (tmp->Get_type() != SNIPE_BULLET)
                    {
                        tmp->Get_Llink()->Set_Rlink(tmp->Get_Rlink());
                        tmp->Get_Rlink()->Set_Llink(tmp->Get_Llink());

                        del = tmp;
                        tmp = tmp->Get_Rlink();

                        free(del);
                    }
                }

                e_tmp = e_tmp->Get_link();
            }
        }

        //¸Ê ¹ÛÀ¸·Î ³ª°¡¸é »èÁ¦
        if (n_pos.x < 0 || n_pos.x > 1280 || n_pos.y < 0 || n_pos.y > 960)
        {
            tmp->Get_Llink()->Set_Rlink(tmp->Get_Rlink());
            tmp->Get_Rlink()->Set_Llink(tmp->Get_Llink());

            del = tmp;
            tmp = tmp->Get_Rlink();

            free(del);
        }

        tmp = tmp->Get_Rlink();
    }
}

BOOL CALLBACK MapEditProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hWnd = GetParent(hDlg);

    HBITMAP hBit;
    HWND hButton;
    static HWND hCheck[4];

    static POINT size;
    static int mapId;
    static vector<BOOL> typeSelector;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        g_mapEditSelector = MAP_NONE;
        for (int i = 0; i < 4; ++i)
            typeSelector.push_back(FALSE);

        size = {20, 15};
        map.make_new_map(size);

        mapId = -1;

        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR1));
        hButton = GetDlgItem(hDlg, IDC_FLOOR1);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR2));
        hButton = GetDlgItem(hDlg, IDC_FLOOR2);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR3));
        hButton = GetDlgItem(hDlg, IDC_FLOOR3);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL));
        hButton = GetDlgItem(hDlg, IDC_WALL1);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL2));
        hButton = GetDlgItem(hDlg, IDC_WALL3);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);
        hBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL3));
        hButton = GetDlgItem(hDlg, IDC_WALL2);
        SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)hBit);

        hCheck[0] = GetDlgItem(hDlg, IDC_TYPE1);
        hCheck[1] = GetDlgItem(hDlg, IDC_TYPE2);
        hCheck[2] = GetDlgItem(hDlg, IDC_TYPE3);
        hCheck[3] = GetDlgItem(hDlg, IDC_TYPE4);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK: // Save
            map.enemy_type_change(typeSelector);
            mapId = GetDlgItemInt(hDlg, IDC_INPUT, NULL, TRUE);
            map.save(mapId);
            break;

        case IDCANCEL:
            phase = PHASE_MENU;
            map.off_editMode();
            SendMessage(hWnd, WM_COMMAND, 0, 0);
            DestroyWindow(hDlg);
            break;

        case IDC_FLOOR1:
            g_mapEditSelector = MAP_FLOOR_TYPE1;
            break;
        case IDC_FLOOR2:
            g_mapEditSelector = MAP_FLOOR_TYPE2;
            break;
        case IDC_FLOOR3:
            g_mapEditSelector = MAP_FLOOR_TYPE3;
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
                SendMessage(hCheck[i], BM_SETCHECK, typeSelector[i], 0);
            SendMessage(hWnd, WM_COMMAND, 0, 0);
            break;
        }
        break;

    case WM_CLOSE:
        phase = PHASE_MENU;
        map.off_editMode();
        SendMessage(hWnd, WM_COMMAND, 0, 0);
        DestroyWindow(hDlg);
        break;
    }
    return 0;
}

void BOMB_target(Tower _tower,Enemy _target) //½ÇÇàÇÑ Å¸¿öµµ ÀÎ¼ö·Î ¹Þ¾Æ¾ß ÇÔ
{
    POINT Bomb_pos = _target.Get_Location();
    vector<BOOL> v = map.get_enemy_type();
    Enemy* tmp;

    _target.Get_Damage(BOMB_DAMAGE);

    if (v[0])
    {
        tmp = Mob1.link;

        while (tmp != NULL)
        {
            if (Get_distance(tmp->Get_Location(), _tower.Get_Location()) < BOMB_RANGE)
            {
                tmp->Get_Damage(BOMB_SPLASH);
            }

            tmp = tmp->Get_link();
        }
    }

    if (v[1])
    {
        tmp = Mob2.link;

        while (tmp != NULL)
        {
            if (Get_distance(tmp->Get_Location(), _tower.Get_Location()) < BOMB_RANGE)
            {
                tmp->Get_Damage(BOMB_SPLASH);
            }

            tmp = tmp->Get_link();
        }
    }

    if (v[2])
    {
        tmp = Mob3.link;
        while (tmp != NULL)
        {
            if (Get_distance(tmp->Get_Location(), _tower.Get_Location()) < BOMB_RANGE)
            {
                tmp->Get_Damage(BOMB_SPLASH);
            }

            tmp = tmp->Get_link();
        }
    }

    if (v[3])
    {
        tmp = Mob4.link;
        while (tmp != NULL)
        {
            if (Get_distance(tmp->Get_Location(), _tower.Get_Location()) < BOMB_RANGE)
            {
                tmp->Get_Damage(BOMB_SPLASH);
            }

            tmp = tmp->Get_link();
        }
    }

    if (v[4])
    {
        tmp = Boss.link;

        while (tmp != NULL)
        {
            if (Get_distance(tmp->Get_Location(), _tower.Get_Location()) < BOMB_RANGE)
            {
                tmp->Get_Damage(BOMB_SPLASH);
            }

            tmp = tmp->Get_link();
        }
    }

}

int Get_distance(POINT a, POINT b)
{
    return fabs(sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}