#pragma once

#include "stdafx.h"

// ������ �� ���� ����, ȭ�� �� �׷����� ������ ���� ��輱 ����
#define MAP_NONE 100

// ������Ʈ�� �̵��� �� �ִ� ����
#define MAP_FLOOR_TYPE1 200
#define MAP_FLOOR_TYPE2 201

// ������Ʈ�� �̵��� �� ���� ����
#define MAP_WALL_TYPE1 300
#define MAP_WALL_TYPE2 301
#define MAP_WALL_TYPE3 302

using namespace std;

class Map
{
private:
    int id;
    POINT size; // �� ũ��
    int count;  // �� Ÿ���� ����
    vector<vector<int>> map;

public:
};