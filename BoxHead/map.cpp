#include "map.h"

using namespace std;

Map::Map()
{
    id = -1;
    count = {0, 0};
    map.resize(0);
    playerSpawn = {0, 0};
    enemySpawn.resize(0);

    size = {0, 0};
    isEditMode = FALSE;
}
void Map::draw(HDC hdc)
{
    if (isEditMode) // ��������� �� ���
    {
    }
    else // �� ���� ���
    {
        if (id != -1) // -1 �ƹ��͵� ���� �ȵ� ���� �ҷ����� ���� �߻�
        {
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
            // �߸��� id
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
        // ������ �������� ����
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

// if on EditMode
BOOL Map::get_isEditMode()
{
    return isEditMode;
}
void Map::on_editMode()
{
    isEditMode = TRUE;
}
void Map::tile_change(POINT _pos, int _type)
{
    if (!isEditMode)
    {
        // ���� ������尡 �ƴ�
        return;
    }
    if (_pos.x > count.x || _pos.y > count.y)
    {
        // �� ũ�� ���� ��� ��ǥ
        return;
    }
    map[_pos.y][_pos.x] = _type;
}
int Map::save()
{
    if (!isEditMode)
    {
        // ���� ������尡 �ƴ�
        return -1;
    }

    string fileName;
    string out;

    fileName = to_string(id) + ".txt";
    ofstream newMapFile;
    newMapFile.open(fileName, ios_base::out);
    if (newMapFile.is_open())
    {
        out = to_string(id) + "\n";                                              // �� ������ȣ
        out += to_string(count.x) + " " + to_string(count.y) + "\n";             // ���μ��� ĭ ����
        out += to_string(playerSpawn.x) + " " + to_string(playerSpawn.y) + "\n"; // �÷��̾� ���� ��ġ
        out += to_string(enemySpawn.size()) + "\n";                              // �� ���� ��ġ ����
        for (int i = 0; i < enemySpawn.size(); ++i)
        {
            out += to_string(enemySpawn[i].x) + " " + to_string(enemySpawn[i].y) + "\n"; // ���� ���� ��ġ
        }
        for (int i = 0; i < count.y; ++i)
        {
            for (int j = 0; j < count.x; ++j)
            {
                out += to_string(map[i][j]) + " "; // �� ����
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
    map.clear();
    map.resize(count.y);
    for (int i = 0; i < count.y; ++i)
    {
        for (int j = 0; j < count.x; ++j)
            map[i].push_back(MAP_FLOOR_TYPE1);
    }
}