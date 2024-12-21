#ifndef MARIO_H
#define MARIO_H
#include "./PhysicsEngine/Managers/ComponentEntityManager.h"
#include "./PhysicsEngine/Components/BoxCollider.h"
#include "./PhysicsEngine/Components/RigidBody.h"
#include "./PhysicsEngine/Components/SpriteRenderer.h"
#include "./items.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#define MARIO "resource/Mario.png"
#define SUPERMARIO "resource/MarioSuper.png"
#define BLOCK_WIDTH 64
#define BLOCK_HEIGHT 64

class Item; // forward declaration
class Mario : public Entity
{
protected:
   Entity *mario = RenderManager::GetInstance().trackE;

   SpriteRenderer *marioSprite = AddComponent<SpriteRenderer>(mario);
   BoxCollider *marioCollider = AddComponent<BoxCollider>(mario);

   sf::Clock timer1, timer2, timer3;
   enum State
   {
      Small,
      Super,
      Fire
   } state = Small;

public:
   bool goRight, goLeft, goUp, firing, created;
   RigidBody *marioRigidBody = AddComponent<RigidBody>(mario);

   Mario(int x, int y);
   void moveRight();
   void moveLeft();
   void setRectForWalking(sf::IntRect &rect);
   void handleMovement();
   // void handleCollision(std::vector<std::unique_ptr<Enemy>> &enemies);
   void handlePowerUp();
   void update(std::vector<std::unique_ptr<Item>> &items);
   void stand();
};

class SuperMario : public Mario
{
public:
   SuperMario(int x, int y);
   void handlePowerUp();
   void handleDamage();
};

#endif // MARIO_H