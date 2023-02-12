#include "../header/TileMap.h"
#include "../header/Game.h"
#include "../header/Camera.h"

TileMap::TileMap(GameObject& associated, const char* file, TileSet* tileSet) : Component(associated)
{
    Load(file);
    this->tileSet = tileSet;
}

void TileMap::Load(const char* file)
{
    ifstream fileStream(file);
    if (fileStream.is_open())
    {
        string line, delimiter = ",";
        fileStream >> line;
        bool first = true;
        unsigned count = 0;

        while (!fileStream.eof())
        {
            size_t pos = 0;
            while ((pos = line.find(delimiter)) != string::npos)
            {
                int data = stoi(line.substr(0, pos));
                
                // If reading for the first time, set map dimensions
                if (first && count == 0)
                {
                    mapWidth = data;
                    count++;
                }

                else if (first && count == 1)
                {
                    mapHeight = data;
                    count++;
                }
                
                else if (first && count == 2)
                {
                    mapDepth = data;
                    first = false;
                }

                // Otherwise saves tiles
                else
                    tileMatrix.push_back(data - 1);

                line.erase(0, pos + delimiter.length());
            }
            fileStream >> line;
        }
        fileStream.close();
    }
    else
        cout << "Unable to open TileMap file" << endl;
}

void TileMap::SetTileSet(TileSet* tileSet)
{
    this->tileSet = tileSet;
}

int& TileMap::At(int x, int y, int z)
{
    // Index = (W * H * Z) + (W * Y) + X
    unsigned index = x + (mapWidth * y) + (mapWidth * mapHeight * z);
    return tileMatrix[index];
}

void TileMap::RenderLayer(int layer, float cameraX, float cameraY)
{
    unsigned startPos = mapWidth * mapHeight * layer;
    unsigned endPos = mapWidth * mapHeight * (layer + 1);
    unsigned countCol = 0, countRow = 0;

    for (auto i = startPos; i < endPos; i++)
    {
        tileSet->RenderTile(
            tileMatrix[i],
            floor(tileSet->GetTileWidth() * countCol * scale) - cameraX,
            floor(tileSet->GetTileHeight() * countRow * scale) - cameraY,
            scale
        );
        countCol = (countCol + 1) % mapWidth;

        if (countCol == 0)
            countRow++;
    }
}

void TileMap::Render()
{
    for (auto i = 0; i < mapDepth; i++)
        RenderLayer(i, Camera::pos.x * (i + 1), Camera::pos.y * (i + 1));
}

void TileMap::Update(float dt)
{

}

bool TileMap::Is(const char* type)
{
    string str_type = type;
    return (str_type == "TileMap");
}

int TileMap::GetWidth()
{
    return mapWidth;
}

int TileMap::GetHeight()
{
    return mapHeight;
}

int TileMap::GetDepth()
{
    return mapDepth;
}