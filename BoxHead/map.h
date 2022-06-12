#pragma once

#include "stdafx.h"

#define TILE_SIZE 64

// ������ �� ���� ����, ȭ�� �� �׷����� ������ ���� ��輱 ����
#define MAP_NONE 100

// ������Ʈ�� �̵��� �� �ִ� ����
#define MAP_FLOOR_TYPE1 200
#define MAP_FLOOR_TYPE2 201

// ������Ʈ�� �̵��� �� ���� ����
#define MAP_WALL_TYPE1 300
#define MAP_WALL_TYPE2 301
#define MAP_WALL_TYPE3 302

// ������� ����
#define MAP_PLAYER_SPAWN_POINT 400;
#define MAP_ENEMY_SPAWN_POINT 401;
#define MAP_REMOVE 402

using namespace std;

class Map
{
private:
    int id;      // ����Ǵ� ���� �̸��̶� ����
    POINT count; // �� Ÿ���� ����
    vector<vector<int>> map;
    POINT playerSpawn;           // �� �� ������ ���
    vector<POINT> enemySpawn;    // ������ ���� ����?
    vector<BOOL> spawnEnemyType; // ���� ��ų ���� ����

    POINT size; // �� ũ�� - ���� �������� �ʰ� ������ ������ ����ҵ�?
    BOOL isEditMode;

public:
    Map();
    void draw(HDC);

    int load(int);

    int get_tile_type(POINT);
    POINT get_player_spawn();
    vector<POINT> get_enemy_spawn();
    vector<BOOL> get_enemy_type();

    // if on EditMode
    void on_editMode();
    void off_editMode();
    BOOL get_isEditMode();

    void make_new_map(POINT);
    void tile_change(POINT, int);
    void enemy_type_change(int, BOOL);
    int save(int);

    // test
    void test_print_file();
    void test_out_content();
};