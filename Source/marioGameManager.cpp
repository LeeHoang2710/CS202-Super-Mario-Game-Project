#include "marioGameManager.h"

MarioGameManager *MarioGameManager::instance = nullptr;
MarioGameManager::GameState MarioGameManager::gameState = MarioGameManager::GameState::menu;

MarioGameManager::MarioGameManager()
{
    menuManager = new MenuManager();
    GUIManager = new GUI();
    level = new Level(3);
    initScoreMap();
}

void MarioGameManager::initScoreMap()
{
    scoreMap[Coin] = 200;
    scoreMap[Mushroom] = 1000;
    scoreMap[Flower] = 1000;
    scoreMap[Star] = 1000;
}

MarioGameManager *MarioGameManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new MarioGameManager();
    }
    return instance;
}

MarioGameManager::~MarioGameManager()
{
    delete menuManager;
    delete GUIManager;
    delete instance;
}

MenuManager *MarioGameManager::getMenuManager()
{
    return this->menuManager;
}

GUI *MarioGameManager::getGUI()
{
    return this->GUIManager;
}

void MarioGameManager::updateGUI()
{
    getGUI()->setCoin(marioCoins);
    getGUI()->setLives(marioLives);
    getGUI()->setTimeRemaining(timeRemaining / 1000);
    getGUI()->setScore(score);
}

void MarioGameManager::run()
{
    setState(GameState::menu);
}
void MarioGameManager::draw(sf::RenderWindow &w)
{
    switch (gameState)
    {
    case GameState::menu:
        menuManager->draw(w);
        break;
    case GameState::playing:
        if (level)
            level->drawLevel();
        RenderManager::GetInstance().Update();
        updateGUI();
        getGUI()->draw(w);
        break;
    case GameState::pause:
        RenderManager::GetInstance().Update();
        getGUI()->draw(w);
        break;
    }
}

void MarioGameManager::handleEvents(sf::RenderWindow &w, sf::Event &ev)
{
    switch (gameState)
    {
    case GameState::menu:
        menuManager->handleEvents(w, ev);
        break;
    case GameState::playing:
        if (ev.type == sf::Event::KeyPressed)
        {
            if (ev.key.code == sf::Keyboard::P)
            {
                togglePause();
            }
        }
        break;
    case GameState::pause:
        if (ev.type == sf::Event::KeyPressed)
        {
            if (ev.key.code == sf::Keyboard::P)
            {
                togglePause();
            }
        }
        break;
    }
}

void MarioGameManager::addScore(ScoreID scoreId)
{
    score += scoreMap[scoreId];
}

void MarioGameManager::addCoin()
{
    ++marioCoins;
    if (marioCoins >= 40)
    {
        marioCoins = 0;
        addLive();
    }
    // addScore(ScoreID::Coin);
    // getGUI()->setCoin(marioCoins);
}

void MarioGameManager::addLive()
{
    ++marioLives;
}

void MarioGameManager::setState(GameState gameState)
{
    this->gameState = gameState;
}

void MarioGameManager::updateGameState(int delta_time, sf::Event &ev)
{
    switch (gameState)
    {
    case GameState::playing:
        playMusic(MarioGameManager::overworld);
        timeRemaining -= delta_time;
        if (level)
            level->execute();
        break;
    case GameState::levelOver:

        break;
    case GameState::gameOver:
        setState(GameState::menu);
        break;
    case GameState::pause:

        break;
    case GameState::menu:
        break;
    }
}
