#pragma once

// #include "mario.h"
#include <fstream>
#include <memory>
#include "enemy.h"

class Block: public Entity
{
public:
    sf::IntRect spritearea;
    Block(): Entity(){}
    void initiate(int, int, int);
};

class Map
{
private:
    vector<vector<int>> projectionmap;
    vector<vector<int>> backgroundmap;
    vector<vector<int>> entitymap;
    int xstart, ystart, offset;
    sf::Texture blocktexture;
    sf::Sprite sprite;
    std::vector<Block*> availableblocks;
    sf::Image layout;
public:
    bool left = false, right = false;
    Map();
    Map(std::string file);
    void readmap();
    void readmap(std::string file);
    void draw(sf::RenderWindow &w, int MarioX, int MarioY);
    void draw(sf::RenderWindow& w);
    vector<vector<int>> getmap(int option);
    int getblockstate(int x, int y)
    {
        return projectionmap[y][x];
    }
    // function creates a list of block entities
    void createblock(int marioX, int marioY);
    void blockgenerator(int, int);
    // function checks for entity availability within map sight
    // map nudge functions, default 5 pixels
    void moveleft(float step);
    void moveright(float step);
};



class blockshared
{
    public:
        sf::Sprite sprite;
        sf::Texture texture;
        blockshared()
        {
            
        }
};