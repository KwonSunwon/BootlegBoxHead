#pragma once

#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "msimg32.lib")

#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <atlImage.h>
#include <math.h>
#include <time.h>
#include <cstdlib>

#define RENDER 10

#define PHASE_MENU 0
#define PHASE_LOAD 1
#define PHASE_PLAY 2
#define PHASE_EDIT 3

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define MOB_TIMELAB 450
#define TOWER_TIMELAB 2000
#define BULLET_TIMELAB 10

#define MOB1_TIMER 800
#define MOB2_TIMER 801
#define MOB3_TIMER 802
#define MOB4_TIMER 803
#define BOSS_TIMER 804
#define TOWER_TIMER 805
#define BULLET_TIMER 806

#define TILE_SIZE 64

int Get_distance(POINT a, POINT b);