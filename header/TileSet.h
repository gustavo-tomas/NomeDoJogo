#ifndef TILESET_H
#define TILESET_H

#include "Sprite.h"
#include "GameObject.h"

class TileSet {
    public:
        TileSet(int tileWidth, int tileHeight, const char* file);
        void RenderTile(unsigned index, float x, float y, float scale = 1.0);
        int GetTileWidth();
        int GetTileHeight();

    private:
        Sprite* tileSet;
        int rows;
        int columns;
        int tileWidth;
        int tileHeight;
};

#endif // TILESET_H