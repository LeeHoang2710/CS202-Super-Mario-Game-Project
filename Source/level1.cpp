#include "level1.h"
#include <iostream>
using namespace std;
Level1::Level1() : mario(100, 100), m("Data/Level1")
{
   m.blockgenerator(100, 12 * BLOCK_HEIGHT);
   sf::Color c;
   entitylayout.loadFromFile("Data/Level1/entity.png");
   for (int i = 0; i < entitylayout.getSize().y; i++)
    {
        for (int j = 0; j < entitylayout.getSize().x; j++)
        {
            c = sf::Color(entitylayout.getPixel(j, i));
            if (c == sf::Color(95, 205, 228)); //nothing
            else if (c == sf::Color(143, 86, 59))
                enemies.push_back(EnemyFactory::createEnemy("Goomba", j * BLOCK_WIDTH, i * BLOCK_HEIGHT));
            else if (c == sf::Color(153, 229, 80))
                enemies.push_back(EnemyFactory::createEnemy("Koopa", j * BLOCK_WIDTH, i * BLOCK_HEIGHT));
            else if (c == sf::Color(106, 190, 48))
                enemies.push_back(EnemyFactory::createEnemy("HammerBro", j * BLOCK_WIDTH, i * BLOCK_HEIGHT));
            else if (c == sf::Color(255, 255, 0))
                enemies.push_back(EnemyFactory::createEnemy("PiranhaPlant", j * BLOCK_WIDTH, i * BLOCK_HEIGHT));
        }
    }
   // enemies.push_back(EnemyFactory::createEnemy("Goomba", 300, 0));
   // enemies.push_back(EnemyFactory::createEnemy("Koopa", 400, 0));
   // enemies.push_back(EnemyFactory::createEnemy("PiranhaPlant", 500, 0));
   // enemies.push_back(EnemyFactory::createEnemy("HammerBro", 400, 700));
   // items.push_back(ItemFactory::createItem("Mushroom", 200, 700));
   // items.push_back(ItemFactory::createItem("Coin", 500, 700));
   // items.push_back(ItemFactory::createItem("Coin", 600, 700));
   // items.push_back(ItemFactory::createItem("Coin", 700, 700));
   items.push_back(ItemFactory::createItem("Flower", 350, 700));
};
void Level1::start()
{
   display = true;
}
void Level1::end()
{
   display = false;
}

void Level1::handleKeyPress()
{
   mario.goLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
   mario.goRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
   mario.firing = sf::Keyboard::isKeyPressed(sf::Keyboard::F);
   mario.marioRigidBody->isJumping = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
}

void Level1::execute()
{
   handleKeyPress();
   mario.update(items);
   for (auto &enemy : enemies)
   {
      enemy->move();
      enemy->collideWithMario(mario);
      enemy->fadingAnimation();
   }

   for (auto &item : items)
   {
      item->animation();
      item->fadeOut();
   }
}

void Level1::drawLevel()
{
   cout << "in draw level" << endl;
   m.draw(RenderManager::GetInstance().window);
}
