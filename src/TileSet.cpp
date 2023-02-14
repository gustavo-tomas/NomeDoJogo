#include "../header/TileSet.h"

TileSet::TileSet(int tileWidth, int tileHeight, const char* file)
{
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;

    GameObject* go = new GameObject();
    tileSet = new Sprite(*go, file);

    rows = tileSet->GetHeight() / this->tileHeight;
    columns = tileSet->GetWidth() / this->tileWidth;
}

void TileSet::RenderTile(unsigned index, float x, float y, float scale)
{
    // 0 <= index <= numOfTiles - 1
    if (index >= 0 && index <= (unsigned) rows * columns - 1)
    {
        unsigned row = (index / columns) * tileHeight;
        unsigned column = (index % columns) * tileWidth;
        
        tileSet->SetClip(column, row, tileWidth, tileHeight);
        tileSet->Render(x, y, ceil(tileWidth * scale), ceil(tileHeight * scale));

    }
}

int TileSet::GetTileWidth()
{
    return tileWidth;
}

int TileSet::GetTileHeight()
{
    return tileHeight;    
}