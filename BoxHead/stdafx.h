#pragma once

#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <atlImage.h>
#include <math.h>

#define PHASE_MENU 0
#define PHASE_LOAD 1
#define PHASE_PLAY 2
#define PHASE_EDIT 3

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int Get_distance(POINT a, POINT b);