#pragma once

#include "stdafx.h"

// 움직일 수 없는 구역, 화면 상에 그려지지 않지만 맵의 경계선 역할
#define MAP_NONE 100

// 오브젝트가 이동할 수 있는 구역
#define MAP_FLOOR_TYPE1 200
#define MAP_FLOOR_TYPE2 201

// 오브젝트가 이동할 수 없는 구역
#define MAP_WALL_TYPE1 300
#define MAP_WALL_TYPE2 301
#define MAP_WALL_TYPE3 302

using namespace std;

class Map
{
private:
    int id;
    POINT size; // 맵 크기
    int count;  // 맵 타일의 갯수
    vector<vector<int>> map;

public:
};