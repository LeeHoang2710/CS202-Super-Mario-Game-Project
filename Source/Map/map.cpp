#include "map.h"

// Map::Map()
// {
//     blocktexture.loadFromFile("Images/TilesBackup.png");
//     // block.setTexture(blocktexture);
// }

Map::Map(bool resuming)
{
    blocktexture.loadFromFile(TILES);
    this->resuming = resuming;
}

// read map organization from a file
// the map file contains 3 layers: the first layer for blocks, the second layer for background details, the third layer for entity spawns
// corresponding numbers for certain objects will be shown below
void Map::readmap()
{
    projectionmap.clear();
    backgroundmap.clear();
    int height, width;
    int coordinates;
    std::ifstream mapfile("Data/maps.txt");
    mapfile >> height >> width;
    int count = width;
    for (int i = 0; i < height * width; i++)
    {
        mapfile >> coordinates;
        if (count == width)
        {
            count = 0;
            projectionmap.push_back(vector<int>({}));
        }
        projectionmap[projectionmap.size() - 1].push_back(coordinates);
        count++;
    }
    count = width;
    for (int i = 0; i < height * width; i++)
    {
        mapfile >> coordinates;
        if (count == width)
        {
            count = 0;
            backgroundmap.push_back(vector<int>({}));
        }
        backgroundmap[backgroundmap.size() - 1].push_back(coordinates);
        count++;
    }
    for (int i = 0; i < height * width; i++)
    {
        mapfile >> coordinates;
        if (count == width)
        {
            count = 0;
            entitymap.push_back(vector<int>({}));
        }
        entitymap[entitymap.size() - 1].push_back(coordinates);
        count++;
    }
}

// read map organization from a file
// the map file contains 3 layers: the first layer for blocks, the second layer for background details, the third layer for entity spawns
// corresponding numbers for certain objects will be shown below
void Map::readmap(std::string file)
{
    sf::Color c;
    int target;
    projectionmap.clear();
    backgroundmap.clear();
    layout.loadFromFile(file + "/background.png");
    for (int i = 0; i < layout.getSize().y; i++)
    {
        backgroundmap.push_back(vector<int>({}));
        for (int j = 0; j < layout.getSize().x; j++)
        {
            c = sf::Color(layout.getPixel(j, i));
            if (c == sf::Color(95, 205, 228))
                target = 0; // sky
            else if (c == sf::Color(255, 255, 255))
                target = 10;
            else
                target = 0;
            backgroundmap[i].push_back(target);
        }
    }
    layout.loadFromFile(file + "/layout.png");
    // std::ifstream mapfile(file);
    // mapfile >> height >> width;
    for (int i = 0; i < layout.getSize().y; i++)
    {
        projectionmap.push_back(vector<int>({}));
        for (int j = 0; j < layout.getSize().x; j++)
        {
            c = sf::Color(layout.getPixel(j, i));
            if (c == sf::Color(95, 205, 228))
                target = 0; // no block
            else if (c == sf::Color(143, 86, 59))
                target = 1; // wall
            else if (c == sf::Color(255, 242, 0))
                target = 2; // mystery box
            else if (c == sf::Color(98, 232, 112))
                target = 3; // vertical up pipe
            else if (c == sf::Color(38, 223, 57))
                target = 4; // horizontal left-ward pipe
            else if (c == sf::Color(153, 229, 80))
                target = 5; // flag pole
            else if (c == sf::Color(223, 113, 38))
                target = 6; // castle
            else if (c == sf::Color(238, 195, 154))
                target = 7; // mushroom tile
            else if (c == sf::Color(0, 0, 0))
                target = 8; // turret
            else if (c == sf::Color(217, 113, 15))
                target = 10;
            else
                target = 0;
            projectionmap[i].push_back(target);
        }
    }
}

vector<vector<int>> Map::getmap(int option)
{
    switch (option)
    {
    case 0:
        return projectionmap;
    default:
        return entitymap;
    }
}

void Map::createblock(int x, int y)
{
    if (projectionmap[y][x] == 0)
        return;
    Block *block = new Block;
    RenderManager::GetInstance().listEntity.push_back(block);
    block->scaleX = 1.0;
    block->scaleY = 1.0;
    block->xPos = x * BLOCK_WIDTH;
    block->yPos = y * BLOCK_HEIGHT;
    block->tag = "block";
    int leftcoor = 0, rightcoor = 0, rowsbelow = 0;
    // SpriteRenderer *sr = AddComponent<SpriteRenderer>(block);
    // sr->layer = 1;
    // sr->texture.loadFromFile("Images/TilesBackup.png");
    // sr->texture.setSmooth(true);
    // sr->sprite.setTexture(blocktexture);
    int xtex, ytex;
    switch (projectionmap[y][x])
    {
    case 0: // nothing
        xtex = 1;
        ytex = 7;
        block->name = "Nothing";
        break;
    case 1: // wall
        xtex = 1;
        ytex = 0;
        block->name = "Block";
        break;
    case 10: // wall
        xtex = 1;
        ytex = 0;
        block->name = "FloatingBlock";
        break;
    case 2: // mystery box
        xtex = 2;
        ytex = 1;
        block->name = "MysteryBox";
        break;
    case 3: // vertical up pipe
        if (projectionmap[y][x - 1] != 3)
        {
            if (y == 0)
            {
                xtex = 6;
                ytex = 2;
                break;
            }
            else if (projectionmap[y - 1][x] != 3)
            {
                xtex = 6;
                ytex = 1;
                break;
            }
            else
            {
                xtex = 6;
                ytex = 2;
                break;
            }
        }
        else
        {
            if (y == 0)
            {
                xtex = 7;
                ytex = 1;
                break;
            }
            else if (projectionmap[y - 1][x] != 3)
            {
                xtex = 7;
                ytex = 1;
                break;
            }
            else
            {
                xtex = 7;
                ytex = 2;
            }
        }
        block->name = "Pipe";
        break;
    case 4: // horizontal left-ward pipe
        if (y == 0)
            ytex = 3;
        else if (projectionmap[y - 1][x] != 4)
            ytex = 3;
        else
            ytex = 4;
        if (projectionmap[y][x - 1] != 4)
            xtex = 4;
        else
            xtex = 5;
        block->name = "Pipe";
        break;
    case 5: // flag pole
        block->name = "Flag";
        xtex = 7;
        if (y == 0)
        {
            ytex = 8;
            break;
        }
        else if (projectionmap[y - 1][x] != 5)
        {
            ytex = 8;
            break;
        }
        else
        {
            ytex = 9;
            break;
        }
    case 6: // castle
        leftcoor = 0;
        rightcoor = 0;
        for (int i = 1; i < 7; i++)
        {
            if (projectionmap[y][x - i] == 6)
                leftcoor++;
            else
                break;
        }
        for (int i = 1; i < 7; i++)
        {
            if (projectionmap[y][x + i] == 6)
                rightcoor++;
            else
                break;
        }
        xtex = (leftcoor + rightcoor) / 2 - abs(leftcoor - (leftcoor + rightcoor) / 2);
        for (rowsbelow = 1; rowsbelow < 6; rowsbelow++)
        {
            if (y == 14)
                break;
            if (projectionmap[y + rowsbelow][x] != 6)
                break;
        }
        if (rowsbelow % 2 == 0)
            ytex = 8;
        else
            ytex = 9;
        block->name = "Castle";
        break;
    case 7: // mushroom tile, not mushroom buff
        block->name = "MushroomTile";
        ytex = 0;
        if (projectionmap[y][x - 1] != 7)
            xtex = 4;
        else if (projectionmap[y][x + 1] != 7)
            xtex = 6;
        else
            xtex = 5;
        break;
    case 8:
        block->name = "Turret";
        xtex = 7;
        if (y == 0)
            ytex = 3;
        else if (projectionmap[y - 1][x] != 8)
            ytex = 3;
        else
            ytex = 4;
        break;

    default:
        xtex = 1;
        ytex = 7;
        break;
    }
    // sr->sprite.setTextureRect(sf::IntRect(xtex * BLOCK_WIDTH, ytex * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT));
    block->spritearea = sf::IntRect(xtex * BLOCK_WIDTH, ytex * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT);
    availableblocks.push_back(block);
    BoxCollider *bc = AddComponent<BoxCollider>(block);
    if (projectionmap[y][x] == 5)
    {
        bc->width = 64;
        bc->height = 64;
    }
    else if (projectionmap[y][x] == 6)
    {
        bc->width = 0;
        bc->height = 0;
    }
    else
    {
        bc->width = BLOCK_WIDTH;
        bc->height = BLOCK_HEIGHT;
    }
    RigidBody *rb = AddComponent<RigidBody>(block);
    bc->body = rb;
    rb->collider = bc;
    rb->isUsingGravity = false;
    rb->isStatic = true;
    rb->xVel = 0;
    rb->yVel = 0;
}

void Map::createbackgroundblock(int x, int y)
{
    // Block *block = new Block;
    // block->scaleX = 1.0;
    // block->scaleY = 1.0;
    // block->xPos = x * BLOCK_WIDTH;
    // block->yPos = y * BLOCK_HEIGHT;
    // block->name = "BackgroundBlock";
    // int xtex, ytex;
    // switch (backgroundmap[y][x])
    // {
    // case 0: // sky
    //     xtex = 1;
    //     ytex = 7;
    //     break;
    // case 10: // cloud
    //     if (y == 0)
    //         ytex = 5;
    //     else if (backgroundmap[y - 1][x] != 10)
    //         ytex = 5;
    //     else
    //         ytex = 6;
    //     if (backgroundmap[y][x - 1] != 10)
    //         xtex = 2;
    //     else if (backgroundmap[y][x + 1] != 10)
    //         xtex = 4;
    //     else
    //         xtex = 3;
    //     break;
    // default:
    //     break;
    // }
    // block->spritearea = sf::IntRect(xtex * BLOCK_WIDTH, ytex * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT);
    // backgroundblocks.push_back(block);
}

void Map::draw(sf::RenderWindow &w)
{
    Camera::GetInstance().posX = RenderManager::GetInstance().trackE->xPos - 200;
    // for (Block *i : backgroundblocks)
    // {
    //     sprite.setTexture(blocktexture);
    //     sprite.setTextureRect(sf::IntRect(1 * BLOCK_WIDTH, 7 * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT));
    //     sprite.setPosition(i->xPos, i->yPos);
    //     w.draw(sprite);
    //     sprite.setTexture(blocktexture);
    //     sprite.setTextureRect(i->spritearea);
    //     sprite.setPosition(i->xPos - Camera::GetInstance().posX, i->yPos);
    //     w.draw(sprite);
    // }
    backgroundsprite.setPosition(-Camera::GetInstance().posX, 0);
    w.draw(backgroundsprite);
    for (Block *i : availableblocks)
    {
        sprite.setTexture(blocktexture);
        if (i->isTouch)
            sprite.setTextureRect(sf::IntRect(192, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
        else
            sprite.setTextureRect(i->spritearea);
        sprite.setPosition(i->xPos - Camera::GetInstance().posX, i->yPos);
        w.draw(sprite);
    }
}

void Map::moveleft(float step)
{
    if (xstart == 0 && offset == 0)
        return;
    for (Entity *e : availableblocks)
    {
        e->xPos += step;
    }
    offset -= step;
    if (offset < 0)
    {
        if (xstart == 0)
        {
            for (Entity *e : availableblocks)
            {
                e->xPos += offset;
            }
            offset = 0;
        }
        else
        {
            offset += BLOCK_WIDTH;
            xstart--;
        }
    }
}

void Map::moveright(float step)
{
    if (xstart == projectionmap[0].size() - 24 && offset == 0)
        return;
    for (Entity *e : availableblocks)
    {
        e->xPos -= step;
    }
    offset += step;
    if (offset > BLOCK_WIDTH)
    {
        xstart++;
        offset -= BLOCK_WIDTH;
        if (xstart == projectionmap[0].size() - 24)
        {
            for (Entity *e : availableblocks)
            {
                e->xPos -= offset;
            }
            offset = 0;
        }
    }
}

void Map::blockgenerator(int MarioX, int MarioY)
{
    xstart = 0;
    ystart = 0;
    if (MarioX <= 8 * BLOCK_WIDTH)
    {
        xstart = 0;
        offset = 0;
    }
    else if (MarioX > 78 * BLOCK_WIDTH)
    {
        xstart = 59;
        offset = 0;
    }
    else
    {
        xstart = MarioX / BLOCK_WIDTH - 8;
        offset = MarioX % BLOCK_WIDTH;
    }
    for (int i = 0; i < projectionmap.size(); i++)
        for (int j = xstart; j < projectionmap[0].size(); j++)
        {
            createbackgroundblock(j, i);
            createblock(j, i);
        }
}

void Map::loadmap(int level, int MarioX, int MarioY)
{
    std::string whichlevel;
    switch (level)
    {
    case 1:
        whichlevel = "Data/Level1";
        break;
    case 2:
        whichlevel = "Data/Level2";
        break;
    case 3:
        whichlevel = "Data/Level3";
        break;
    }
    readmap(whichlevel);
    blockgenerator(MarioX, MarioY);
    backgroundtexture.loadFromFile(BACKGROUND);
    backgroundtexture.setSmooth(true);
    backgroundsprite.setTexture(backgroundtexture);
    backgroundsprite.setScale(3.75f, 3.75f);
}
