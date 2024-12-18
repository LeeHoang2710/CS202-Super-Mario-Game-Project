#include "character.h"

Character::Character(int x, int y, const std::string &texture1, const std::string &texture2)
{
    if (RenderManager::GetInstance().trackE == nullptr)
        RenderManager::GetInstance().trackE = new Entity();

    character = RenderManager::GetInstance().trackE;
    characterSprite = AddComponent<SpriteRenderer>(character);
    characterCollider = AddComponent<BoxCollider>(character);
    characterRigidBody = AddComponent<RigidBody>(character);

    goRight = goLeft = goUp = created = false;
    eatFlower = eatMushroom = touchEnemy = false;
    this->textureFile1 = texture1;
    this->textureFile2 = texture2;

    character->scaleX = 1.5;
    character->scaleY = 1.5;
    character->xPos = x;
    character->yPos = y;
    character->name = "character";

    characterSprite->layer = 2;
    if (state == Small || state == Super)
        characterSprite->texture.loadFromFile(textureFile1);
    else if (state == Fire)
        characterSprite->texture.loadFromFile(textureFile2);
    characterSprite->texture.setSmooth(true);
    characterSprite->sprite.setTexture(characterSprite->texture);
    characterSprite->sprite.setTextureRect(sf::IntRect(0, 96, 28, 32));
    characterCollider->width = 48;
    characterCollider->height = 48;

    characterCollider->body = characterRigidBody;
    characterRigidBody->collider = characterCollider;
    characterRigidBody->isStatic = false;
    characterRigidBody->isUsingGravity = true;
    characterRigidBody->xVel = 0, characterRigidBody->yVel = 0;
}

void Character::moveRight(float speed)
{
    goRight = true, goLeft = false;
    direction = Right;
    characterRigidBody->isStatic = false;
    characterRigidBody->AddForce(speed, 0.f);
    sf::IntRect rect = characterSprite->sprite.getTextureRect();
    if (characterRigidBody->xVel <= -1)
    {
        if (state == Small)
            rect.left = 132;
        else if (state == Super || state == Fire)
            rect.left = 129;
    }
    else
        setRectForWalking(rect);
    if (!characterRigidBody->isJumping)
        characterSprite->sprite.setTextureRect(rect);
}

void Character::moveLeft(float speed)
{
    goLeft = true, goRight = false;
    direction = Left;
    characterRigidBody->AddForce(-speed, 0.f);
    sf::IntRect rect = characterSprite->sprite.getTextureRect();
    if (characterRigidBody->xVel >= 1)
    {
        if (state == Small)
            rect.left = 1024 - 161;
        else if (state == Super || state == Fire)
            rect.left = 1024 - 164;
    }
    else
        setRectForWalking(rect);
    if (!characterRigidBody->isJumping)
        characterSprite->sprite.setTextureRect(rect);
}

void Character::setRectForWalking(sf::IntRect &rect)
{
    int maxLeft = 0, picWidth = 0;
    if (direction == Right)
    {
        if (state == Small)
            maxLeft = 99, picWidth = 33;
        else if (state == Super || state == Fire)
            maxLeft = 96, picWidth = 32;

        if (rect.left >= maxLeft)
            rect.left = picWidth;
        else
            rect.left += picWidth;
    }
    else if (direction == Left)
    {
        if (state == Small)
            maxLeft = 1024 - 99, picWidth = 32;
        else if (state == Super || state == Fire)
            maxLeft = 1024 - 102, picWidth = 30;

        if (rect.left <= maxLeft)
            rect.left = 1024 - picWidth;
        else
            rect.left -= picWidth;
    }
}

void Character::handleMovement(float speed)
{
    float waitingTime = 0.05;
    sf::IntRect rect = characterSprite->sprite.getTextureRect();

    if (timer1.getElapsedTime().asSeconds() > waitingTime)
    {
        if (characterRigidBody->isJumping)
        {
            if (direction == Right)
            {
                if (state == Small)
                    rect.left = 162.5;
                else if (state == Super || state == Fire)
                    rect.left = 161;
            }
            else if (direction == Left)
            {
                if (state == Small)
                    rect.left = 1024 - 189.5;
                else if (state == Super || state == Fire)
                    rect.left = 1024 - 191;
            }

            MarioGameManager::getInstance()->playSound(MarioGameManager::jump);
        }
        if (firing)
        {
            created = true;
            if (state == Super || state == Fire)
            {
                if (direction == Right)
                    rect.left = 224;
                else
                    rect.left = 1024 - 248;
            }
            MarioGameManager::getInstance()->playSound(MarioGameManager::fireball);
        }
        characterSprite->sprite.setTextureRect(rect);

        waitingTime += 0.07;
        if (timer2.getElapsedTime().asSeconds() > waitingTime)
        {
            if (goRight == goLeft)
                characterRigidBody->xVel = 0;
            else if (goRight)
                moveRight(speed);
            else if (goLeft)
                moveLeft(speed);

            timer2.restart();
        }
        timer1.restart();
    }
    stand();
}

void Character::stand()
{
    if (!goRight && !goLeft && !characterRigidBody->isJumping && !firing)
    {
        if (state == Small)
        {
            if (direction == Right)
                characterSprite->sprite.setTextureRect(sf::IntRect(0, 96, 28, 32));
            else if (direction == Left)
                characterSprite->sprite.setTextureRect(sf::IntRect(996, 96, 28, 32));
        }

        else if (state == Super || state == Fire)
        {
            if (direction == Right)
                characterSprite->sprite.setTextureRect(sf::IntRect(0, 36, 28, 60));
            else if (direction == Left)
                characterSprite->sprite.setTextureRect(sf::IntRect(1024 - 28, 36, 28, 60));
        }
    }
}

void Character::animation1(float duration, float interval, std::function<void()> onComplete, bool &finished)
{
    static sf::Clock blinkTimer;
    float elapsed = blinkTimer.getElapsedTime().asMilliseconds();
    characterCollider->width = 48;
    characterCollider->height = 86;

    if (elapsed > duration)
    {
        characterSprite->sprite.setColor(sf::Color::White); // Reset to normal
        blinkTimer.restart();                               // Reset timer for next use
        finished = false;
        if (onComplete)
            onComplete(); // Call the completion callback
        return;
    }

    // Toggle visibility
    if (duration == 1000)
    {
        if (int(elapsed / interval) % 2 == 0)
            characterSprite->sprite.setTextureRect(sf::IntRect(0, 96, 28, 32));
        else
            characterSprite->sprite.setTextureRect(sf::IntRect(0, 36, 28, 60));
    }
    else if (duration == 1200)
    {
        if (int(elapsed / interval) % 2 == 0)
            characterSprite->texture.loadFromFile(textureFile1);

        else
            characterSprite->texture.loadFromFile(textureFile2);

        characterSprite->texture.setSmooth(true);
        characterSprite->sprite.setTexture(characterSprite->texture);
        characterSprite->sprite.setTextureRect(sf::IntRect(0, 36, 28, 60));
    }
}

void Character::animation2(float duration, float interval, std::function<void()> onComplete, bool &finished, State &state)
{
    static sf::Clock blinkTimer;
    float elapsed = blinkTimer.getElapsedTime().asMilliseconds();
    characterCollider->width = 48;
    characterCollider->height = 48;

    if (elapsed > duration)
    {
        characterSprite->sprite.setColor(sf::Color::White); // Reset to normal
        blinkTimer.restart();                               // Reset timer for next use
        finished = false;
        if (onComplete)
            onComplete(); // Call the completion callback
        return;
    }

    if (state == Small)
    {
        if (int(elapsed / interval) % 2 == 0)
            characterSprite->sprite.setColor(sf::Color::Transparent);
        else
            characterSprite->sprite.setColor(sf::Color::White);
    }
    else if (state == Super || state == Fire)
    {
        if (int(elapsed / interval) % 2 == 0)
            characterSprite->sprite.setTextureRect(sf::IntRect(0, 36, 28, 60));
        else
            characterSprite->sprite.setTextureRect(sf::IntRect(0, 96, 28, 32));
    }
}

void Character::handlePowerUp()
{
    characterCollider->OnCollisionEnter = [this](BoxCollider *collider)
    {
        if (collider->body->GetOwner()->name == "mushroom")
        {
            // Animate Character and handle state transition
            eatMushroom = true;
            stateTimer.restart();
            collider->body->SetActive(false);
            MarioGameManager::getInstance()->playSound(MarioGameManager::powerup);
            logEvent("Mushroom collected", character->xPos, character->yPos);
            MarioGameManager::getInstance()->addScore(MarioGameManager::Mushroom);
        }
        else if (collider->body->GetOwner()->name == "coin")
        {
            MarioGameManager::getInstance()->addCoin();
            MarioGameManager::getInstance()->playSound(MarioGameManager::add_coin);
            collider->body->SetActive(false);
            MarioGameManager::getInstance()->addScore(MarioGameManager::Coin);
            logEvent("Coin collected", character->xPos, character->yPos);
        }
        else if (collider->body->GetOwner()->name == "flower")
        {
            eatFlower = true;
            stateTimer.restart();
            collider->body->SetActive(false);
            MarioGameManager::getInstance()->playSound(MarioGameManager::powerup);
            logEvent("Flower collected", character->xPos, character->yPos);
            MarioGameManager::getInstance()->addScore(MarioGameManager::Flower);
        }
    };
    if (eatMushroom)
    {
        animation1(1000, 100, [this]()
                   {
                     // Transition to Super Character state
                     state = Super;
                     characterSprite->sprite.setTextureRect(sf::IntRect(0, 36, 28, 60)); }, eatMushroom);
    }
    if (eatFlower)
    {
        animation1(1200, 100, [this]()
                   {
                     // Transition to Fire Character state
                     state = Fire;
                     characterSprite->texture.loadFromFile(textureFile2);
                     characterSprite->texture.setSmooth(true);
                     characterSprite->sprite.setTexture(characterSprite->texture);
                     characterSprite->sprite.setTextureRect(sf::IntRect(0, 36, 28, 60)); }, eatFlower);
    }
}

void Character::handleEnemy()
{
    if (touchEnemy)
    {
        if (state == Small)
        {
            animation2(1000, 50, [this]()
                       { MarioGameManager::getInstance()->marioDies(); }, touchEnemy, state);
        }
        else if (state == Super || state == Fire)
        {
            animation2(1000, 50, [this]()
                       {
            state = Small;
            characterSprite->sprite.setTextureRect(sf::IntRect(0, 96, 28, 32)); }, touchEnemy, state);
        }
    }
}

void Character::update(std::vector<std::unique_ptr<Item>> &items, float speed)
{
    freeFall();
    handleMovement(speed);
    handlePowerUp();
    handleEnemy();
    if (firing && created && timer3.getElapsedTime().asSeconds() > 1)
    {
        bool bulletDirection = (direction == Right);
        if (bulletDirection)
            items.push_back(ItemFactory::createItem("Fireball", character->xPos + characterCollider->width + 1, character->yPos, bulletDirection));
        else
            items.push_back(ItemFactory::createItem("Fireball", character->xPos - 20, character->yPos, bulletDirection));
        created = false;
        timer3.restart();
    }
    if (stateTimer.getElapsedTime().asSeconds() > 60)
    {
        if (state != Small)
        {
            state = Small;
            characterSprite->sprite.setTextureRect(sf::IntRect(0, 96, 28, 32));
            characterCollider->width = 48;
            characterCollider->height = 48;
        }
    }
}

GameStateMemento Character::saveState()
{
    std::vector<Entity *> entities = RenderManager::GetInstance().listEntity;
    MarioState characterState;
    characterState.xPos = character->xPos;
    characterState.yPos = character->yPos;
    characterState.name = character->tag;
    characterState.lives = MarioGameManager::getInstance()->getLives();
    characterState.coins = MarioGameManager::getInstance()->getCoins();
    characterState.score = MarioGameManager::getInstance()->getScore();
    characterState.time = MarioGameManager::getInstance()->getTimeRemaining();
    switch (state)
    {
    case Small:
        characterState.state = "Small";
        break;
    case Super:
        characterState.state = "Super";
        break;
    case Fire:
        characterState.state = "Fire";
        break;
    }
    return GameStateMemento(characterState, entities);
}

void Character::restoreState(const GameStateMemento &memento)
{
    character->xPos = memento.marioState.xPos;
    character->yPos = memento.marioState.yPos;
    character->tag = memento.marioState.name;
    MarioGameManager::getInstance()->setLives(memento.marioState.lives);
    MarioGameManager::getInstance()->setCoins(memento.marioState.coins);
    MarioGameManager::getInstance()->setScore(memento.marioState.score);
    MarioGameManager::getInstance()->setTimeRemaining(memento.marioState.time);
    if (memento.marioState.state == "Small")
        state = Small;
    else if (memento.marioState.state == "Super")
        state = Super;
    else if (memento.marioState.state == "Fire")
        state = Fire;

    if (state == Fire)
    {
        characterSprite->texture.loadFromFile(textureFile2);
        characterSprite->texture.setSmooth(true);
        characterSprite->sprite.setTexture(characterSprite->texture);
    }
    if (state == Super || state == Fire)
    {
        characterSprite->sprite.setTextureRect(sf::IntRect(0, 36, 28, 60));
        characterCollider->width = 48;
        characterCollider->height = 86;
    }
    RenderManager::GetInstance().listEntity = memento.entities;
}

void Character::freeFall()
{
    if (characterRigidBody->GetOwner()->yPos >= 15 * BLOCK_HEIGHT)
    {
        character->xPos = 100;
        character->yPos = 100;
        touchEnemy = true;
    }
}

Mario::Mario(int x, int y) : Character(x, y, MARIO, SUPERMARIO)
{
    character->tag = "mario";
}

Luigi::Luigi(int x, int y) : Character(x, y, LUIGI, SUPERLUIGI)
{
    character->tag = "luigi";
}