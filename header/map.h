#pragma once

#include "mario.h"

class Map
{
private:
    vector<vector<int>> projectionmap;
    vector<vector<int>> backgroundmap;
    vector<vector<int>> entitymap;
    Sprite block;
    Texture blocktexture;

public:
    Map();
    void readmap();
    void draw(RenderWindow &w, int MarioX, int MarioY);
};