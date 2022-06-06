#include "map.h"

using namespace std;

Map::Map()
{
    id = -1;
    size = {0, 0};
    count = {0, 0};
    map.resize(0);
    isEditMode = FALSE;
}
void Map::draw(HDC hdc)
{
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
        for (int i = 0; i < count.y; ++i)
        {
            for (int j = 0; j < count.x; ++j)
            {
                mapFile >> in;
                map[i][j] = stoi(in);
            }
        }
    }
    else
    {
        // 파일이 존재하지 않음
        return -1;
    }
    return id;
}

int Map::get_tile_type(POINT _pos)
{
    return map[_pos.y][_pos.x];
}

// if on EditMode
BOOL Map::get_isEditMode()
{
    return isEditMode;
}
void Map::on_editMode()
{
    ~isEditMode;
}
void Map::tile_change(POINT _pos, int _type)
{
    if (!isEditMode)
    {
        // 현재 수정모드가 아님
        return;
    }
    if (_pos.x > count.x || _pos.y > count.y)
    {
        // 맵 크기 범위 벗어난 좌표
        return;
    }
    map[_pos.y][_pos.x] = _type;
}
int Map::save()
{
    if (!isEditMode)
    {
        // 현재 수정모드가 아님
        return -1;
    }

    string fileName;
    string out;

    fileName = to_string(id) + ".txt";
    ofstream newMapFile;
    newMapFile.open(fileName, ios_base::out);
    if (newMapFile.is_open())
    {
        out = to_string(id) + "\n";
        out += to_string(count.x) + " " + to_string(count.y) + "\n";
        for (int i = 0; i < count.y; ++i)
        {
            for (int j = 0; j < count.x; ++j)
            {
                out += to_string(map[i][j]) + " ";
            }
            out += "\n";
        }
        newMapFile.write(out.c_str(), out.size());
        newMapFile.close();
    }
}