#ifndef TILEMAP_H
#define TILEMAP_H

#include "Component.h"
#include "GameObject.h"
#include "TileSet.h"
#include <fstream>

class TileMap : public Component {
    public:
        TileMap(GameObject& associated, const char* file, TileSet* tileSet);
        void Load(const char* file);
        void SetTileSet(TileSet* tileSet);
        int& At(int x, int y, int z = 0);
        void Update(float dt);
        bool Is(const char* type);
        void Render();
        void RenderLayer(int layer, float cameraX = 0, float cameraY = 0);
        int GetWidth();
        int GetHeight();
        int GetDepth();
        float scale{1.0f};

    private:
        vector<int> tileMatrix;
        TileSet* tileSet;
        int mapWidth;
        int mapHeight;
        int mapDepth;
};

#endif // TILEMAP_H
