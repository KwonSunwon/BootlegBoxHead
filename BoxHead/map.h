#pragma once

#include "stdafx.h"

#define TILE_SIZE 64

// 움직일 수 없는 구역, 화면 상에 그려지지 않지만 맵의 경계선 역할
#define MAP_NONE 100

// 오브젝트가 이동할 수 있는 구역
#define MAP_FLOOR_TYPE1 200
#define MAP_FLOOR_TYPE2 201

// 오브젝트가 이동할 수 없는 구역
#define MAP_WALL_TYPE1 300
#define MAP_WALL_TYPE2 301
#define MAP_WALL_TYPE3 302

// 수정모드 선택
#define MAP_PLAYER_SPAWN_POINT 400;
#define MAP_ENEMY_SPAWN_POINT 401;
#define MAP_REMOVE 402

using namespace std;

class Map
{
private:
    int id;      // 저장되는 파일 이름이랑 동일
    POINT count; // 맵 타일의 갯수
    vector<vector<int>> map;
    POINT playerSpawn;           // 한 개 지점만 사용
    vector<POINT> enemySpawn;    // 여러곳 지정 가능?
    vector<BOOL> spawnEnemyType; // 등장 시킬 적의 종류

    POINT size; // 맵 크기 - 따로 저장하지 않고 정해진 사이즈 사용할듯?
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