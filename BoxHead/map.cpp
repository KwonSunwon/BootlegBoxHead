#include "map.h"

using namespace std;

Map::Map()
{
    id = -1;
    count = {0, 0};
    map.resize(0);
    playerSpawn = {0, 0};
    enemySpawn.resize(0);

    for (int i = 0; i < 4; ++i)
        spawnEnemyType.push_back(FALSE);

    size = {0, 0};
    isEditMode = FALSE;
}
void Map::draw(HDC hdc)
{
    HBRUSH hBrush, oldBrush;
    CImage img;
    int width;
    int height;

    RECT drawRt = {0, 0, TILE_SIZE, TILE_SIZE};

    for (int i = 0; i < count.y; ++i)
    {
        for (int j = 0; j < count.x; ++j)
        {
            if (map[i][j] == MAP_NONE)
            {
            }
            else if (map[i][j] == MAP_FLOOR_TYPE1)
            {
                img.Load(TEXT("Floor1.png"));
                if (img.IsNull())
                    continue;
                width = img.GetWidth();
                height = img.GetHeight();
                img.Draw(hdc, drawRt.right * j, drawRt.bottom * i, TILE_SIZE, TILE_SIZE, 0, 0, width, height);
                img.Destroy();
            }
            else if (map[i][j] == MAP_FLOOR_TYPE2)
            {
                img.Load(TEXT("Floor2.png"));
                if (img.IsNull())
                    continue;
                width = img.GetWidth();
                height = img.GetHeight();
                img.Draw(hdc, drawRt.right * j, drawRt.bottom * i, TILE_SIZE, TILE_SIZE, 0, 0, width, height);
                img.Destroy();
            }
            else if (map[i][j] == MAP_WALL_TYPE1)
            {
                img.Load(TEXT("Wall.png"));
                if (img.IsNull())
                    continue;
                width = img.GetWidth();
                height = img.GetHeight();
                img.Draw(hdc, drawRt.right * j, drawRt.bottom * i, TILE_SIZE, TILE_SIZE, 0, 0, width, height);
                img.Destroy();
            }
            else if (map[i][j] == MAP_WALL_TYPE2)
            {
                img.Load(TEXT("BarrelA.png"));
                if (img.IsNull())
                    continue;
                width = img.GetWidth();
                height = img.GetHeight();
                img.Draw(hdc, drawRt.right * j, drawRt.bottom * i, TILE_SIZE, TILE_SIZE, 0, 0, width, height);
                img.Destroy();
            }
            else if (map[i][j] == MAP_WALL_TYPE3)
            {
                img.Load(TEXT("120px-Small_altar.png"));
                if (img.IsNull())
                    continue;
                width = img.GetWidth();
                height = img.GetHeight();
                img.Draw(hdc, drawRt.right * j, drawRt.bottom * i, TILE_SIZE, TILE_SIZE, 0, 0, width, height);
                img.Destroy();
            }

            if (isEditMode)
            {
                RECT temp = drawRt;
                OffsetRect(&temp, drawRt.right * j, drawRt.bottom * i);
                hBrush = CreateSolidBrush(RGB(255, 0, 0));
                FrameRect(hdc, &temp, hBrush);
                DeleteObject(hBrush);
            }
        }
    }
}

int Map::load(int _id)
{
    string fileName;

    fileName = to_string(_id) + ".txt";
    ifstream mapFile;
    mapFile.open(fileName, ios_base::in);

    string in;

    if (mapFile.is_open())
    {
        mapFile >> in;
        id = stoi(in);
        if (id != _id)
        {
            // 잘못된 id
            return -1;
        }
        mapFile >> in;
        count.x = stoi(in);
        mapFile >> in;
        count.y = stoi(in);

        mapFile >> in;
        playerSpawn.x = stoi(in);
        mapFile >> in;
        playerSpawn.y = stoi(in);

        mapFile >> in;
        enemySpawn.clear();
        int enemySpawnSize = stoi(in);
        for (int i = 0; i < enemySpawnSize; ++i)
        {
            POINT temp;
            mapFile >> in;
            temp.x = stoi(in);
            mapFile >> in;
            temp.y = stoi(in);
            enemySpawn.push_back(temp);
        }
        for (int i = 0; i < 4; ++i)
        {
            mapFile >> in;
            spawnEnemyType[i] = stoi(in);
        }

        map.clear();
        map.resize(count.y);
        for (int i = 0; i < count.y; ++i)
        {
            for (int j = 0; j < count.x; ++j)
            {
                mapFile >> in;
                map[i].push_back(stoi(in));
            }
        }
    }
    else
    {
        // 파일이 존재하지 않음
        return -1;
    }
    mapFile.close();
    return id;
}

int Map::get_tile_type(POINT _pos)
{
    return map[_pos.y][_pos.x];
}
POINT Map::get_player_spawn()
{
    return playerSpawn;
}
vector<POINT> Map::get_enemy_spawn()
{
    return enemySpawn;
}
vector<BOOL> Map::get_enemy_type()
{
    return spawnEnemyType;
}

// if on EditMode
BOOL Map::get_isEditMode()
{
    return isEditMode;
}
void Map::on_editMode()
{
    isEditMode = TRUE;
}
void Map::off_editMode()
{
    isEditMode = FALSE;
}
void Map::tile_change(POINT _pos, int _type)
{
    if (!isEditMode)
    {
        // 현재 수정모드가 아님
        return;
    }
    if (_pos.x > count.y || _pos.y > count.x)
    {
        // 맵 크기 범위 벗어난 좌표
        return;
    }
    map[_pos.y][_pos.x] = _type;
}
void Map::enemy_type_change(int _type, BOOL check)
{
    spawnEnemyType[_type] = check;
}

void Map::make_new_map(POINT _count)
{
    id = -1;
    count = _count;
    map.clear();
    map.resize(count.y);
    for (int i = 0; i < count.y; ++i)
    {
        for (int j = 0; j < count.x; ++j)
            map[i].push_back(MAP_NONE);
    }
    playerSpawn = {0, 0};

    enemySpawn.clear();
    for (int i = 0; i < 4; ++i)
        spawnEnemyType[i] = FALSE;

    size = {0, 0};
}

int Map::save(int _id)
{
    if (!isEditMode)
    {
        // 현재 수정모드가 아님
        return -1;
    }

    string fileName;
    string out;

    id = _id;

    fileName = to_string(id) + ".txt";
    ofstream newMapFile;
    newMapFile.open(fileName, ios_base::out);
    if (newMapFile.is_open())
    {
        out = to_string(id) + "\n";                                              // 맵 고유번호
        out += to_string(count.x) + " " + to_string(count.y) + "\n";             // 가로세로 칸 개수
        out += to_string(playerSpawn.x) + " " + to_string(playerSpawn.y) + "\n"; // 플레이어 시작 위치
        out += to_string(enemySpawn.size()) + "\n";                              // 적 스폰 위치 개수
        for (int i = 0; i < enemySpawn.size(); ++i)
        {
            out += to_string(enemySpawn[i].x) + " " + to_string(enemySpawn[i].y) + "\n"; // 적들 스폰 위치
        }
        for (int i = 0; i < 4; ++i)
        {
            out += to_string(spawnEnemyType[i]) + "\n";
        }
        for (int i = 0; i < count.y; ++i)
        {
            for (int j = 0; j < count.x; ++j)
            {
                out += to_string(map[i][j]) + " "; // 맵 파일
            }
            out += "\n";
        }
        newMapFile.write(out.c_str(), out.size());
        newMapFile.close();
    }

    return id;
}

// test func
void Map::test_print_file()
{
    cout << id << "\n";
    cout << count.x << ", " << count.y << "\n";
    cout << playerSpawn.x << ", " << playerSpawn.y << "\n";
    for (int i = 0; i < enemySpawn.size(); ++i)
        cout << enemySpawn[i].x << ", " << enemySpawn[i].y << "\n";
    for (int i = 0; i < 4; ++i)
        cout << spawnEnemyType[i] << "\n";
    for (int i = 0; i < count.y; ++i)
    {
        for (int j = 0; j < count.x; ++j)
        {
            cout << map[i][j] << ", ";
        }
        cout << "\n";
    }
}

void Map::test_out_content()
{
    isEditMode = TRUE;
    id = 200;
    count = {6, 4};
    playerSpawn = {3, 3};
    enemySpawn.push_back({1, 1});
    enemySpawn.push_back({2, 2});
    enemySpawn.push_back({3, 3});
    enemySpawn.push_back({4, 5});
    for (int i = 0; i < 4; ++i)
        spawnEnemyType[i] = TRUE;
    map.clear();
    map.resize(count.y);
    for (int i = 0; i < count.y; ++i)
    {
        for (int j = 0; j < count.x; ++j)
            map[i].push_back(MAP_FLOOR_TYPE1);
    }
}