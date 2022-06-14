#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include <stdio.h>
#include "game_manager.h"
#include "resource.h"

// map global
Map map;
int g_mapEditSelector;

// Object global
Player p;
int phase, spawn_count, difficulty;
BOOL key_buffer[4];
Tower T[MAX_TOWER_COUNT];
int tower_count, tower_way_set, tower_id_set ,tower_pos_set;
HBITMAP hBitmap_tmp;
Bullet shotgun[MAX_SHOTGUN], pistol[MAX_PISTOL], rifle[MAX_RIFLE], sniper;
Bullet tower_rifle[MAX_TOWERRIFLE], tower_sniper;
int shotgun_count, pistol_count, rifle_count, sniper_count;
int tower_rifle_count, tower_sniper_count;

Enemy Mob[ENEMY_MAXCOUNT];
int enemy_count;

POINT location_to_position(POINT _location);

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void CALLBACK Enemy_spawn(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
//void CALLBACK Tower_Oparate(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void CALLBACK Bullet_fly(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
BOOL CALLBACK MapEditProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void Player_move();
//void BOMB_target(Tower);

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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, NULL, (HMENU)NULL, hInstance, NULL);
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
    Enemy *e_tmp;

    static RECT message_box, edit_box, map1_box, map2_box, map3_box, map4_box;
    static RECT rc;
    static TCHAR name[10] = L"BoxLike";
    static TCHAR start_message[11] = L"Start Game";
    static TCHAR Select[11] = L"Select Map";
    static TCHAR edit_button[11] = L"Map Editer";
    static TCHAR map1_button[5] = L"Map1";
    static TCHAR map2_button[5] = L"Map2";
    static TCHAR map3_button[5] = L"Map3";
    static TCHAR map4_button[11] = L"Custom Map";

    static HWND hDlg;

    static BOOL LBClick;
    static BOOL RBClick;
    static BOOL building;
    static POINT mouse;

    RECT ClientRT = {0, 0, 1280, 960};

    static TCHAR lpstrFile[MAX_PATH], lpstrFileTitle[MAX_PATH] = L"";
    static TCHAR filter[100] = L"������(*.map)\0*.map\0";
    OPENFILENAME OFN;

    RECT tempRect;

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

        map1_box.left = 50;
        map1_box.top = 150;
        map1_box.right = 750;
        map1_box.bottom = 250;
        map2_box.left = 50;
        map2_box.top = 275;
        map2_box.right = 750;
        map2_box.bottom = 375;
        map3_box.left = 50;
        map3_box.top = 400;
        map3_box.right = 750;
        map3_box.bottom = 500;
        map4_box.left = 50;
        map4_box.top = 525;
        map4_box.right = 750;
        map4_box.bottom = 625;

        tower_way_set = IDB_UP;
        tower_id_set = ID_HEAL_TOWER;

        tower_count = 0;
        building = FALSE;

        shotgun_count = 0; pistol_count = 0; rifle_count = 0; sniper_count = 0;
        tower_rifle_count = 0; tower_sniper_count = 0;
        tower_pos_set = IDB_UP;
        enemy_count = 0;

        break;

    case WM_LBUTTONDOWN:
        mouse_pos.x = LOWORD(lParam);
        mouse_pos.y = HIWORD(lParam);

        if (phase == PHASE_MENU)
        {
            if (message_box.left < mouse_pos.x && message_box.top < mouse_pos.y && message_box.right > mouse_pos.x && message_box.bottom > mouse_pos.y)
            {
                phase = PHASE_LOAD;
                printf("start button clicked\n");
                InvalidateRect(hWnd, NULL, FALSE);
            }

            if (edit_box.left < mouse_pos.x && edit_box.top < mouse_pos.y && edit_box.right > mouse_pos.x && edit_box.bottom > mouse_pos.y)
            {
                phase = PHASE_EDIT;
                printf("edit button clicked\n");
                map.on_editMode();
                if (!IsWindow(hDlg))
                {
                    hDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_MAPEDIT), hWnd, (DLGPROC)MapEditProc);
                    ShowWindow(hDlg, SW_SHOW);
                }

                InvalidateRect(hWnd, NULL, FALSE);
            }
        }

        else if (phase == PHASE_LOAD)
        {
            // map1 ���ý�
            if (map1_box.left < mouse_pos.x && map1_box.top < mouse_pos.y && map1_box.right > mouse_pos.x && map1_box.bottom > mouse_pos.y)
            {
                // map �ҷ��� �ڿ�
                map.load(100);
                spawn_count = 0;
                p.Set_Location(map.get_player_spawn());

                hBitmap_tmp = (HBITMAP)LoadImage(g_hInst, TEXT("tmp_p.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                p.Set_Image(hBitmap_tmp);
                p.Set_Weapon(PISTOL);

                SetTimer(hWnd, BULLET_TIMER, BULLET_TIMELAB, Bullet_fly);
                SetTimer(hWnd, ENEMY_TIMER, ENEMY_TIMELAB, Enemy_spawn);

                /*
                SetTimer(hWnd, TOWER_TIMER, TOWER_TIMELAB, Tower_Oparate);
                    */
                phase = PHASE_PLAY;
            }

            // map2 ���ý�
            if (map2_box.left < mouse_pos.x && map2_box.top < mouse_pos.y && map2_box.right > mouse_pos.x && map2_box.bottom > mouse_pos.y)
            {
                // map �ҷ��� �ڿ�
                map.load(200);
                spawn_count = 0;
                p.Set_Location(map.get_player_spawn());
                hBitmap_tmp = (HBITMAP)LoadImage(g_hInst, TEXT("tmp_p.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                p.Set_Image(hBitmap_tmp);
                p.Set_Weapon(PISTOL);

                SetTimer(hWnd, BULLET_TIMER, BULLET_TIMELAB, Bullet_fly);
                SetTimer(hWnd, ENEMY_TIMER, ENEMY_TIMELAB, Enemy_spawn);

                /*
                SetTimer(hWnd, TOWER_TIMER, TOWER_TIMELAB, Tower_Oparate);
                    */
                phase = PHASE_PLAY;
            }

            // map3 ���ý�
            if (map3_box.left < mouse_pos.x && map3_box.top < mouse_pos.y && map3_box.right > mouse_pos.x && map3_box.bottom > mouse_pos.y)
            {
                // map �ҷ��� �ڿ�
                map.load(300);
                spawn_count = 0;
                p.Set_Location(map.get_player_spawn());

                hBitmap_tmp = (HBITMAP)LoadImage(g_hInst, TEXT("tmp_p.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                p.Set_Image(hBitmap_tmp);
                p.Set_Weapon(PISTOL);

                SetTimer(hWnd, BULLET_TIMER, BULLET_TIMELAB, Bullet_fly);
                SetTimer(hWnd, ENEMY_TIMER, ENEMY_TIMELAB, Enemy_spawn);

                /*
                SetTimer(hWnd, TOWER_TIMER, TOWER_TIMELAB, Tower_Oparate);
                     */
                phase = PHASE_PLAY;
            }

            // custom map ���ý�
            if (map4_box.left < mouse_pos.x && map4_box.top < mouse_pos.y && map4_box.right > mouse_pos.x && map4_box.bottom > mouse_pos.y)
            {
                // map �ҷ��� �ڿ�

                memset(&OFN, 0, sizeof(OPENFILENAME));
                OFN.lStructSize = sizeof(OPENFILENAME);
                OFN.hwndOwner = hWnd;
                OFN.lpstrFilter = filter;
                OFN.lpstrFile = lpstrFile;
                OFN.lpstrFileTitle = lpstrFileTitle;
                OFN.nMaxFile = 256;
                OFN.lpstrInitialDir = L".";

                if (GetOpenFileName(&OFN) != 0)
                {
                    map.load(OFN);
                }

                spawn_count = 0;
                p.Set_Location(map.get_player_spawn());
                SetTimer(hWnd, ENEMY_TIMER, ENEMY_TIMELAB, Enemy_spawn);
                //SetTimer(hWnd, TOWER_TIMER, TOWER_TIMELAB, Tower_Oparate);
                SetTimer(hWnd, BULLET_TIMER, BULLET_TIMELAB, Bullet_fly);


                phase = PHASE_PLAY;
            }

            DeleteObject(hBitmap_tmp);

            InvalidateRect(hWnd, NULL, FALSE);
        }

        else if (phase == PHASE_EDIT)
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
                    // ??û ½ºÆù ??§Ä¡ 4°³ ÃÊ°ú ÁöÁ¤ ºÒ°¡´É
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
        if (phase == PHASE_PLAY)
        {
            if (wParam == 'w' || wParam == 'W')
            {
                key_buffer[UP] = TRUE;

                RECT playerRect = p.Get_Rect();

                POINT Virtual_pos = p.Get_Location();
                Virtual_pos.y -= (OBJECT_Y_SIZE / 2);
                Virtual_pos.y -= p.Get_Speed();

                POINT position = location_to_position(Virtual_pos);

                position.x -= 1;
                for (int i = 0; i < 3; ++i)
                {
                    RECT mapRect = map.get_tile_rect(position);
                    if (IntersectRect(&tempRect, &playerRect, &mapRect) && (map.get_tile_type(position) >= 300 && map.get_tile_type(position) < 400))
                        key_buffer[UP] = FALSE;
                    position.x++;
                }

                // if (Virtual_pos.y - p.Get_Speed() > 0)
                // {
                //     Virtual_pos.y -= p.Get_Speed();

                //     if (map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE1 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE2 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE3)
                //     {
                //         key_buffer[UP] = TRUE;
                //     }
                // }
            }

            if (wParam == 's' || wParam == 'S')
            {
                key_buffer[DOWN] = TRUE;

                RECT playerRect = p.Get_Rect();

                POINT Virtual_pos = p.Get_Location();
                Virtual_pos.y += (OBJECT_Y_SIZE / 2);
                Virtual_pos.y += p.Get_Speed();

                POINT position = location_to_position(Virtual_pos);

                position.x -= 1;
                for (int i = 0; i < 3; ++i)
                {
                    RECT mapRect = map.get_tile_rect(position);
                    std::cout << map.get_tile_type(position) << "\n";

                    if (IntersectRect(&tempRect, &playerRect, &mapRect) && (map.get_tile_type(position) >= 300 && map.get_tile_type(position) < 400))
                    {
                        key_buffer[DOWN] = FALSE;
                    }
                    position.x++;
                }

                // POINT Virtual_pos = p.Get_Location();

                // if (Virtual_pos.y + p.Get_Speed() < 1280)
                // {
                //     Virtual_pos.y += p.Get_Speed();

                //     if (map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE1 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE2 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE3)
                //     {
                //         key_buffer[DOWN] = TRUE;
                //     }
                // }
            }

            if (wParam == 'a' || wParam == 'A')
            {
                key_buffer[LEFT] = TRUE;
                RECT playerRect = p.Get_Rect();

                POINT Virtual_pos = p.Get_Location();
                Virtual_pos.x -= (OBJECT_X_SIZE / 2);
                Virtual_pos.x -= p.Get_Speed();

                POINT position = location_to_position(Virtual_pos);

                position.y -= 1;
                for (int i = 0; i < 3; ++i)
                {
                    RECT mapRect = map.get_tile_rect(position);
                    if (IntersectRect(&tempRect, &playerRect, &mapRect) && (map.get_tile_type(position) >= 300 && map.get_tile_type(position) < 400))
                    {
                        key_buffer[LEFT] = FALSE;
                    }
                    position.y++;
                }

                // POINT Virtual_pos = p.Get_Location();

                // if (Virtual_pos.x - p.Get_Speed() > 0)
                // {
                //     Virtual_pos.x -= p.Get_Speed();

                //     if (map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE1 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE2 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE3)
                //     {
                //         key_buffer[LEFT] = TRUE;
                //     }
                // }
            }

            if (wParam == 'd' || wParam == 'D')
            {
                key_buffer[RIGHT] = TRUE;

                RECT playerRect = p.Get_Rect();

                POINT Virtual_pos = p.Get_Location();
                Virtual_pos.x += (OBJECT_X_SIZE / 2);
                Virtual_pos.x += p.Get_Speed();

                POINT position = location_to_position(Virtual_pos);

                position.y -= 1;
                for (int i = 0; i < 3; ++i)
                {
                    RECT mapRect = map.get_tile_rect(position);
                    if (IntersectRect(&tempRect, &playerRect, &mapRect) && (map.get_tile_type(position) >= 300 && map.get_tile_type(position) < 400))
                    {
                        key_buffer[RIGHT] = FALSE;
                    }
                    position.y++;
                }

                // POINT Virtual_pos = p.Get_Location();

                // if (Virtual_pos.x + p.Get_Speed() < 960)
                // {
                //     Virtual_pos.x += p.Get_Speed();

                //     if (map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE1 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE2 || map.get_tile_type(Virtual_pos) == MAP_FLOOR_TYPE3)
                //     {
                //         key_buffer[RIGHT] = TRUE;
                //     }
                // }
            }

            if (wParam == '1')
            {
                p.Set_Weapon(PISTOL);
            }

            if (wParam == '2')
            {
                p.Set_Weapon(SHOTGUN);
            }

            if (wParam == '3')
            {
                p.Set_Weapon(RIFLE);
            }

            if (wParam == '4')
            {
                p.Set_Weapon(SNIPER);
            }

            /*
            if (wParam == 't' || wParam == 'T')
            {
                if (!building && tower_count < MAX_TOWER_COUNT)
                    building = TRUE;

                else if (building)
                    building = FALSE;
            }

            if (wParam == 'q' || wParam == 'Q')
            {
                if (building)
                {
                    if (tower_id_set == ID_HEAL_TOWER)
                        tower_id_set = ID_STD_TOWER;

                    else if (tower_id_set == ID_STD_TOWER)
                        tower_id_set = ID_SNIPE_TOWER;

                    else if (tower_id_set == ID_SNIPE_TOWER)
                        tower_id_set = ID_BOMB_TOWER;

                    else if (tower_id_set == ID_BOMB_TOWER)
                        tower_id_set = ID_HEAL_TOWER;
                }
            }

            if (wParam == 'r' || wParam == 'R')
            {
                if (building)
                {
                    if (tower_way_set == IDB_UP)
                        tower_way_set = IDB_RIGHT;

                    else if (tower_way_set == IDB_RIGHT)
                        tower_way_set = IDB_DOWN;

                    else if (tower_way_set == IDB_DOWN)
                        tower_way_set = IDB_LEFT;

                    else if (tower_way_set == IDB_LEFT)
                        tower_way_set = IDB_UP;
                }
            }

            if (wParam == 'e' || wParam == 'E')
            {
                if (building)
                {
                    hBitmap_tmp = (HBITMAP)LoadImage(g_hInst, TEXT("tmp_t.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                    T[tower_count].Set_Image(hBitmap_tmp);
                    T[tower_count].Set_way(tower_way_set);
                    T[tower_count].Set_id(tower_id_set);

                    switch (tower_pos_set)
                    {
                    case IDB_UP:
                        b_pos = p.Get_Location();
                        b_pos.y -= OBJECT_Y_SIZE;

                        T[tower_count].Set_Location(b_pos);
                        break;
                    case IDB_DOWN:
                        b_pos = p.Get_Location();
                        b_pos.y += OBJECT_Y_SIZE;

                        T[tower_count].Set_Location(b_pos);
                        break;
                    case IDB_LEFT:
                        b_pos = p.Get_Location();
                        b_pos.x -= OBJECT_X_SIZE;

                        T[tower_count].Set_Location(b_pos);
                        break;
                    case IDB_RIGHT:
                        b_pos = p.Get_Location();
                        b_pos.x += OBJECT_X_SIZE;

                        T[tower_count].Set_Location(b_pos);
                        break;
                    }

                    tower_count++;

                    building = FALSE;
                }
            }
            */
            Player_move();
            Player_move();
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
                    if (pistol_count < MAX_PISTOL)
                    {
                        pistol[pistol_count].Shot_bullet(PISTOL_DAMAGE, IDB_LEFT, p.Get_Location());

                        pistol_count++;
                    }
                    break;
                case RIFLE:
                    if (rifle_count < MAX_RIFLE)
                    {
                        rifle[rifle_count].Shot_bullet(RIFLE_DAMAGE, IDB_LEFT, p.Get_Location());

                        rifle_count++;
                    }
                    break;
                case SHOTGUN:
                    if (shotgun_count < MAX_SHOTGUN)
                    {
                        shotgun[shotgun_count].Shot_bullet(SHOTGUN_DAMAGE, IDB_LEFT, p.Get_Location());

                        shotgun_count++;
                    }
                    break;
                case SNIPER:
                    if (sniper_count < MAX_SNIPER)
                    {
                        sniper.Shot_bullet(SNIPER_DAMAGE, IDB_LEFT, p.Get_Location());

                        sniper_count++;
                    }
                    break;
                }
            }

            if (wParam == VK_RIGHT)
            {
                switch (p.Get_Weapon_id())
                {
                case PISTOL:
                    if (pistol_count < MAX_PISTOL)
                    {
                        pistol[pistol_count].Shot_bullet(PISTOL_DAMAGE, IDB_RIGHT, p.Get_Location());

                        pistol_count++;
                    }
                    break;
                case RIFLE:
                    if (rifle_count < MAX_RIFLE)
                    {
                        rifle[rifle_count].Shot_bullet(RIFLE_DAMAGE, IDB_RIGHT, p.Get_Location());

                        rifle_count++;
                    }
                    break;
                case SHOTGUN:
                    if (shotgun_count < MAX_SHOTGUN)
                    {
                        shotgun[shotgun_count].Shot_bullet(SHOTGUN_DAMAGE, IDB_RIGHT, p.Get_Location());

                        shotgun_count++;
                    }
                    break;
                case SNIPER:
                    if (sniper_count < MAX_SNIPER)
                    {
                        sniper.Shot_bullet(SNIPER_DAMAGE, IDB_RIGHT, p.Get_Location());

                        sniper_count++;
                    }
                    break;
                }
            }

            if (wParam == VK_UP)
            {
                switch (p.Get_Weapon_id())
                {
                case PISTOL:
                    if (pistol_count < MAX_PISTOL)
                    {
                        pistol[pistol_count].Shot_bullet(PISTOL_DAMAGE, IDB_UP, p.Get_Location());

                        pistol_count++;
                    }
                    break;
                case RIFLE:
                    if (rifle_count < MAX_RIFLE)
                    {
                        rifle[rifle_count].Shot_bullet(RIFLE_DAMAGE, IDB_UP, p.Get_Location());

                        rifle_count++;
                    }
                    break;
                case SHOTGUN:
                    if (shotgun_count < MAX_SHOTGUN)
                    {
                        shotgun[shotgun_count].Shot_bullet(SHOTGUN_DAMAGE, IDB_UP, p.Get_Location());

                        shotgun_count++;
                    }
                    break;
                case SNIPER:
                    if (sniper_count < MAX_SNIPER)
                    {
                        sniper.Shot_bullet(SNIPER_DAMAGE, IDB_UP, p.Get_Location());

                        sniper_count++;
                    }
                    break;
                }
            }

            if (wParam == VK_DOWN)
            {
                switch (p.Get_Weapon_id())
                {
                case PISTOL:
                    if (pistol_count < MAX_PISTOL)
                    {
                        pistol[pistol_count].Shot_bullet(PISTOL_DAMAGE, IDB_DOWN, p.Get_Location());

                        pistol_count++;
                    }
                    break;
                case RIFLE:
                    if (rifle_count < MAX_RIFLE)
                    {
                        rifle[rifle_count].Shot_bullet(RIFLE_DAMAGE, IDB_DOWN, p.Get_Location());

                        rifle_count++;
                    }
                    break;
                case SHOTGUN:
                    if (shotgun_count < MAX_SHOTGUN)
                    {
                        shotgun[shotgun_count].Shot_bullet(SHOTGUN_DAMAGE, IDB_DOWN, p.Get_Location());

                        shotgun_count++;
                    }
                    break;
                case SNIPER:
                    if (sniper_count < MAX_SNIPER)
                    {
                        sniper.Shot_bullet(SNIPER_DAMAGE, IDB_DOWN, p.Get_Location());

                        sniper_count++;
                    }
                    break;
                }
            }
        }

        InvalidateRect(hWnd, NULL, FALSE);

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

            Rectangle(MemDC, message_box.left, message_box.top, message_box.right, message_box.bottom);
            DrawText(MemDC, start_message, lstrlen(start_message), &message_box, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            Rectangle(MemDC, edit_box.left, edit_box.top, edit_box.right, edit_box.bottom);
            DrawText(MemDC, edit_button, lstrlen(edit_button), &edit_box, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SelectObject(MemDC, oldFont);
            DeleteObject(hFont);
        }

        if (phase == PHASE_LOAD)
        {
            printf("print load phase\n");

            hFont = CreateFont(80, 60, 0, 0, FW_BOLD, 0, 0, 0, NULL, 0, 0, 0, 0, NULL);
            oldFont = (HFONT)SelectObject(MemDC, hFont);

            TextOut(MemDC, 25, 50, Select, lstrlen(Select));

            SelectObject(MemDC, oldFont);
            DeleteObject(hFont);

            hFont = CreateFont(60, 40, 0, 0, FW_BOLD, 0, 0, 0, NULL, 0, 0, 0, 0, NULL);
            oldFont = (HFONT)SelectObject(MemDC, hFont);

            Rectangle(MemDC, map1_box.left, map1_box.top, map1_box.right, map1_box.bottom);
            Rectangle(MemDC, map2_box.left, map2_box.top, map2_box.right, map2_box.bottom);
            Rectangle(MemDC, map3_box.left, map3_box.top, map3_box.right, map3_box.bottom);
            Rectangle(MemDC, map4_box.left, map4_box.top, map4_box.right, map4_box.bottom);

            DrawText(MemDC, map1_button, lstrlen(map1_button), &map1_box, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DrawText(MemDC, map2_button, lstrlen(map2_button), &map2_box, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DrawText(MemDC, map3_button, lstrlen(map3_button), &map3_box, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DrawText(MemDC, map4_button, lstrlen(map4_button), &map4_box, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SelectObject(MemDC, oldFont);
            DeleteObject(hFont);
        }

        if (phase == PHASE_PLAY)
        {
            // print map
            map.draw(MemDC);

            // Player_move();

            // print player
            //SelectObject(PrintDC, p.Get_Image());
            //StretchBlt(MemDC, p.Get_Location().x - OBJECT_X_SIZE / 2, p.Get_Location().y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE, PrintDC, 0, 0, p.Get_Info().bmWidth, p.Get_Info().bmHeight, SRCCOPY);
            Rectangle(MemDC, p.Get_Location().x - OBJECT_X_SIZE / 2, p.Get_Location().y - OBJECT_Y_SIZE / 2, p.Get_Location().x + OBJECT_X_SIZE / 2, p.Get_Location().y + OBJECT_Y_SIZE / 2);
            
            // print enemy
           for (int i = 0; i < enemy_count; i++)
           {
               b_pos = Mob[i].Get_Location();

               Rectangle(MemDC, b_pos.x - OBJECT_X_SIZE / 2, b_pos.y - OBJECT_Y_SIZE / 2, b_pos.x + OBJECT_X_SIZE / 2, b_pos.y + OBJECT_Y_SIZE / 2);
           }


             // print tower
            /*if (building)
            {
                switch (tower_pos_set)
                {
                case IDB_UP:
                    b_pos = p.Get_Location();
                    b_pos.y -= OBJECT_Y_SIZE;

                    Rectangle(MemDC, b_pos.x - OBJECT_X_SIZE / 2, b_pos.y - OBJECT_Y_SIZE / 2, b_pos.x + OBJECT_X_SIZE / 2, b_pos.y + OBJECT_Y_SIZE / 2);
                    break;
                case IDB_DOWN:
                    b_pos = p.Get_Location();
                    b_pos.y += OBJECT_Y_SIZE;

                    Rectangle(MemDC, b_pos.x - OBJECT_X_SIZE / 2, b_pos.y - OBJECT_Y_SIZE / 2, b_pos.x + OBJECT_X_SIZE / 2, b_pos.y + OBJECT_Y_SIZE / 2);
                    break;
                case IDB_LEFT:
                    b_pos = p.Get_Location();
                    b_pos.x -= OBJECT_X_SIZE;

                    Rectangle(MemDC, b_pos.x - OBJECT_X_SIZE / 2, b_pos.y - OBJECT_Y_SIZE / 2, b_pos.x + OBJECT_X_SIZE / 2, b_pos.y + OBJECT_Y_SIZE / 2);
                    break;
                case IDB_RIGHT:
                    b_pos = p.Get_Location();
                    b_pos.x += OBJECT_X_SIZE;

                    Rectangle(MemDC, b_pos.x - OBJECT_X_SIZE / 2, b_pos.y - OBJECT_Y_SIZE / 2, b_pos.x + OBJECT_X_SIZE / 2, b_pos.y + OBJECT_Y_SIZE / 2);
                    break;
                }
            }


             for (int i = 0; i < tower_count; i++)
             {
                 SelectObject(PrintDC, T[i].Get_Image());

                 StretchBlt(MemDC, T[i].Get_Location().x - OBJECT_X_SIZE / 2, T[i].Get_Location().y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE, PrintDC, 0, 0, T[i].Get_Info().bmWidth, T[i].Get_Info().bmHeight, SRCCOPY);
             }
             */

             // print bullet

             // print bullet
            hBrush = CreateSolidBrush(RGB(255, 127, 0));
            oldBrush = (HBRUSH)SelectObject(MemDC, hBrush);

            for (int i = 0; i < pistol_count; i++)
            {
                b_pos = pistol[i].Get_Location();
                Rectangle(MemDC, b_pos.x - 3, b_pos.y - 3, b_pos.x + 3, b_pos.y + 3);
            }

            for (int i = 0; i < shotgun_count; i++)
            {
                b_pos = shotgun[i].Get_Location();
                Rectangle(MemDC, b_pos.x - 3, b_pos.y - 3, b_pos.x + 3, b_pos.y + 3);
            }

            for (int i = 0; i < rifle_count; i++)
            {
                b_pos = rifle[i].Get_Location();
                Rectangle(MemDC, b_pos.x - 3, b_pos.y - 3, b_pos.x + 3, b_pos.y + 3);
            }

            if (sniper_count == MAX_SNIPER)
            {
                b_pos = sniper.Get_Location();
                Rectangle(MemDC, b_pos.x - 3, b_pos.y - 3, b_pos.x + 3, b_pos.y + 3);
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
        DeleteDC(PrintDC);
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
    int selec;
    vector<BOOL> v = map.get_enemy_type();
    POINT p_pos, e_pos;

    if (enemy_count < ENEMY_MAXCOUNT)
    {
        spawn_count++;
    }

    if (spawn_count % ENEMY_SPAWN == 0 && enemy_count < ENEMY_MAXCOUNT)
    {
        selec = rand() % 5;

        printf("spawner on\n");
        
        while (1)
        {
            if (v[0] && selec == 0)
            {
                Mob[enemy_count].Init_enemy(MOB1);
                printf("Mob1 spawn\n");
                break;
            }

            if (v[1] && selec == 1)
            {
                Mob[enemy_count].Init_enemy(MOB2);
                printf("Mob2 spawn\n");
                break;
            }

            if (v[2] && selec == 2)
            {
                Mob[enemy_count].Init_enemy(MOB3);
                printf("Mob3 spawn\n");
                break;
            }

            if (v[3] && selec == 3)
            {
                Mob[enemy_count].Init_enemy(MOB4);
                printf("Mob4 spawn\n");
                break;
            }

            if (v[4] && selec == 4)
            {
                Mob[enemy_count].Init_enemy(BOSS);
                printf("Boss spawn\n");
                break;
            }
        }

        Mob[enemy_count].Set_Location(map.get_enemy_spawn()[0]);

        enemy_count++;
       
    }

    p_pos = p.Get_Location();

    for (int i = 0; i < enemy_count; i++)
    {
        e_pos = Mob[i].Get_Location();

        if (p_pos.x > e_pos.x) Mob[i].Move_right();

        if (p_pos.x < e_pos.x) Mob[i].Move_left();

        if (p_pos.y > e_pos.y) Mob[i].Move_down();

        if (p_pos.y < e_pos.y) Mob[i].Move_up();
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
/*
void CALLBACK Tower_Oparate(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    for (int i = 0; i < tower_count; i++)
    {
        switch (T[i].Get_id())
        {
        case ID_HEAL_TOWER:
            T[i].Heal_target(p);
            break;
        case ID_BOMB_TOWER:
            BOMB_target(T[i]);
            break;
        case ID_SNIPE_TOWER:
            if (tower_sniper_count == 0)
            {
                tower_sniper.Shot_bullet(SNIPER_DAMAGE, T[i].Get_way(), T[i].Get_Location());
                tower_sniper_count++;
            }
            break;
        case ID_STD_TOWER:
            if (tower_rifle_count < MAX_TOWERRIFLE)
            {
                tower_rifle[tower_rifle_count].Shot_bullet(RIFLE_DAMAGE, T[i].Get_way(), T[i].Get_Location());
                tower_rifle_count++;
            }
            break;
        }
    }
}
*/

void CALLBACK Bullet_fly(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    POINT b_pos, e_pos;

    for (int i = 0; i < pistol_count; i++)
    {
        pistol[i].Move();

        b_pos = pistol[i].Get_Location();

        if (b_pos.x < 0 || b_pos.y < 0 || b_pos.x > 1280 || b_pos.y > 980)
        {
            for (int j = i; j < pistol_count - 1; j++)
            {
                pistol[j] = pistol[j + 1];
            }

            pistol_count--;
        }

        for (int e = 0; e < enemy_count; e++)
        {
            e_pos = Mob[e].Get_Location();

            if (b_pos.x > (e_pos.x - OBJECT_X_SIZE / 2) && b_pos.x < (e_pos.x + OBJECT_X_SIZE / 2) && b_pos.y >(e_pos.y - OBJECT_Y_SIZE / 2) && b_pos.y < (e_pos.y + OBJECT_Y_SIZE / 2))
            {
                pistol[i].Deliver_damage(Mob[e]);

                for (int j = i; j < pistol_count - 1; j++)
                {
                    pistol[j] = pistol[j + 1];
                }

                if (Mob[e].Get_Health() < 0)
                {
                    for (int j = e; j < enemy_count - 1; j++)
                    {
                        Mob[j] = Mob[j + 1];
                    }

                    enemy_count--;
                }

                pistol_count--;
            }
        }
    }

    for (int i = 0; i < shotgun_count; i++)
    {
        shotgun[i].Move();

        b_pos = shotgun[i].Get_Location();

        if (b_pos.x < 0 || b_pos.y < 0 || b_pos.x > 1280 || b_pos.y > 980)
        {
            for (int j = i; j < shotgun_count - 1; j++)
            {
                shotgun[j] = shotgun[j + 1];
            }

            shotgun_count--;
        }

        for (int e = 0; e < enemy_count; e++)
        {
            e_pos = Mob[e].Get_Location();

            if (b_pos.x > (e_pos.x - OBJECT_X_SIZE / 2) && b_pos.x < (e_pos.x + OBJECT_X_SIZE / 2) && b_pos.y >(e_pos.y - OBJECT_Y_SIZE / 2) && b_pos.y < (e_pos.y + OBJECT_Y_SIZE / 2))
            {
                shotgun[i].Deliver_damage(Mob[e]);

                for (int j = i; j < shotgun_count - 1; j++)
                {
                    shotgun[j] = shotgun[j + 1];
                }

                if (Mob[e].Get_Health() < 0)
                {
                    for (int j = e; j < enemy_count - 1; j++)
                    {
                        Mob[j] = Mob[j + 1];
                    }

                    enemy_count--;
                }

                shotgun_count--;
            }
        }
    }

    for (int i = 0; i < rifle_count; i++)
    {
        rifle[i].Move();

        b_pos = rifle[i].Get_Location();

        if (b_pos.x < 0 || b_pos.y < 0 || b_pos.x > 1280 || b_pos.y > 980)
        {
            for (int j = i; j < rifle_count - 1; j++)
            {
                rifle[j] = rifle[j + 1];
            }

            rifle_count--;
        }

        for (int e = 0; e < enemy_count; e++)
        {
            e_pos = Mob[e].Get_Location();

            if (b_pos.x > (e_pos.x - OBJECT_X_SIZE / 2) && b_pos.x < (e_pos.x + OBJECT_X_SIZE / 2) && b_pos.y >(e_pos.y - OBJECT_Y_SIZE / 2) && b_pos.y < (e_pos.y + OBJECT_Y_SIZE / 2))
            {
                rifle[i].Deliver_damage(Mob[e]);

                for (int j = i; j < pistol_count - 1; j++)
                {
                    rifle[j] = rifle[j + 1];
                }

                if (Mob[e].Get_Health() < 0)
                {
                    for (int j = e; j < enemy_count - 1; j++)
                    {
                        Mob[j] = Mob[j + 1];
                    }

                    enemy_count--;
                }

                rifle_count--;
            }
        }
    }

    if (sniper_count == MAX_SNIPER)
    {
        sniper.Move();

        b_pos = sniper.Get_Location();

        if (b_pos.x < 0 || b_pos.y < 0 || b_pos.x > 1280 || b_pos.y > 980)
        {
            sniper_count--;
        }

        for (int e = 0; e < enemy_count; e++)
        {
            e_pos = Mob[e].Get_Location();

            if (b_pos.x > (e_pos.x - OBJECT_X_SIZE / 2) && b_pos.x < (e_pos.x + OBJECT_X_SIZE / 2) && b_pos.y >(e_pos.y - OBJECT_Y_SIZE / 2) && b_pos.y < (e_pos.y + OBJECT_Y_SIZE / 2))
            {
                sniper.Deliver_damage(Mob[e]);

                if (Mob[e].Get_Health() < 0)
                {
                    for (int j = e; j < enemy_count - 1; j++)
                    {
                        Mob[j] = Mob[j + 1];
                    }

                    enemy_count--;
                }

                sniper_count--;
            }
        }
    }


    InvalidateRect(hWnd, NULL, FALSE);
}

BOOL CALLBACK MapEditProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hWnd = GetParent(hDlg);

    HBITMAP hBit;
    HWND hButton;
    static HWND hCheck[5];

    static POINT size;
    static int mapId;
    static vector<BOOL> typeSelector;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        g_mapEditSelector = MAP_NONE;
        for (int i = 0; i < 5; ++i)
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
        hCheck[4] = GetDlgItem(hDlg, IDC_TYPE5);
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
        case IDC_TYPE5:
            typeSelector[4] = 1 - typeSelector[4];
            break;

        case IDC_LOAD:
            mapId = GetDlgItemInt(hDlg, IDC_INPUT, NULL, TRUE);
            map.load(mapId);
            typeSelector = map.get_enemy_type();
            for (int i = 0; i < 5; ++i)
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
/*
void BOMB_target(Tower _tower)
{
    POINT Bomb_pos;
    vector<BOOL> v = map.get_enemy_type();
    Enemy *tmp;

    if (v[0] && Mob1.enemy_count > 0)
    {
        tmp = Mob1.link;

        while (tmp != NULL)
        {
            Bomb_pos = tmp->Get_Location();

            if (Get_distance(tmp->Get_Location(), _tower.Get_Location()) < BOMB_RANGE)
            {
                tmp->Get_Damage(BOMB_SPLASH);
            }

            tmp = tmp->Get_link();
        }
    }

    if (v[1] && Mob2.enemy_count > 0)
    {
        tmp = Mob2.link;

        while (tmp != NULL)
        {
            Bomb_pos = tmp->Get_Location();

            if (Get_distance(tmp->Get_Location(), _tower.Get_Location()) < BOMB_RANGE)
            {
                tmp->Get_Damage(BOMB_SPLASH);
            }

            tmp = tmp->Get_link();
        }
    }

    if (v[2] && Mob3.enemy_count > 0)
    {
        tmp = Mob3.link;
        while (tmp != NULL)
        {
            Bomb_pos = tmp->Get_Location();
            if (Get_distance(tmp->Get_Location(), _tower.Get_Location()) < BOMB_RANGE)
            {
                tmp->Get_Damage(BOMB_SPLASH);
            }

            tmp = tmp->Get_link();
        }
    }

    if (v[3] && Mob4.enemy_count > 0)
    {
        tmp = Mob4.link;
        while (tmp != NULL)
        {
            Bomb_pos = tmp->Get_Location();
            if (Get_distance(tmp->Get_Location(), _tower.Get_Location()) < BOMB_RANGE)
            {
                tmp->Get_Damage(BOMB_SPLASH);
            }

            tmp = tmp->Get_link();
        }
    }

    if (v[4] && Boss.enemy_count > 0)
    {
        tmp = Boss.link;

        while (tmp != NULL)
        {
            Bomb_pos = tmp->Get_Location();
            if (Get_distance(tmp->Get_Location(), _tower.Get_Location()) < BOMB_RANGE)
            {
                tmp->Get_Damage(BOMB_SPLASH);
            }

            tmp = tmp->Get_link();
        }
    }
}
*/

int Get_distance(POINT a, POINT b)
{
    return fabs(sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}

POINT location_to_position(POINT _location)
{
    POINT temp = {
        _location.x / TILE_SIZE,
        _location.y / TILE_SIZE,
    };
    return temp;
}