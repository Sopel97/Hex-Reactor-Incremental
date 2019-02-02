#ifndef GAME_H
#define GAME_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../LibS/Geometry.h"

#include "Reactor.h"
#include "ComponentFactory.h"
#include "WindowSpaceManager.h"
#include "UserInterface.h"
#include "ComponentShop.h"
#include "PlayerStats.h"
#include "UpgradeShop.h"

using namespace Geo;

class Game
{
public:
    Game(sf::RenderWindow& window);

    void start();
    void tick();
    void reactorTick();

    bool isReactorEnabled() const;
    void enableReactor();
    void disableReactor();

    bool isAutoCellReplacingEnabled() const;
    void enableAutoCellReplacing();
    void disableAutoCellReplacing();

    bool isAutoNeutronReflectorReplacingEnabled() const;
    void enableAutoNeutronReflectorReplacing();
    void disableAutoNeutronReflectorReplacing();

    void draw();

    void processKeyboardInput(sf::Event& keyEvent);
    void processAsyncKeyboardInput();

    void processMouseInput(sf::Event& mouseEvent);
    void processAsyncMouseInput();

    void sweepPlaceComponents(const Vec2I& first, const Vec2I& second);
    void sweepRemoveComponents(const Vec2I& first, const Vec2I& second);


    void onWindowResized(sf::Event& windowResizedEvent);

    int lastFPSMeasured() const;

    const Reactor& reactor() const;
    Reactor& reactor();

    const ComponentFactory& componentFactory() const;
    ComponentFactory& componentFactory();

    const PlayerStats& playerStats() const;
    PlayerStats& playerStats();

    const WindowSpaceManager& windowSpaceManager() const;
    WindowSpaceManager& windowSpaceManager();

    float lastFrameTime() const;

    bool buyAndPlaceSelectedComponent(const Vec2I& hex);
    bool sellAndRemoveComponent(const Vec2I& hex);

    std::string descriptionToShow() const;

    void saveGame(const std::string& fileName) const;
    void loadGame(const std::string& fileName);

private:
    sf::RenderWindow& m_window;
    PlayerStats m_playerStats;
    WindowSpaceManager m_windowSpaceManager;
    ComponentFactory m_componentFactory;
    UserInterface m_userInterface;
    ComponentShop m_componentShop;
    UpgradeShop m_upgradeShop;
    Reactor m_reactor;
    double m_tickTime; //these two are in MILISECONDS
    double m_reactorTickTime;
    bool m_isReactorEnabled;
    bool m_isAutoCellReplacingEnabled;
    bool m_isAutoNeutronReflectorReplacingEnabled;
    sf::Clock m_clock;
    int m_framesSinceLastFPSMeasure;
    int m_lastFPSMeasureTime;
    int m_lastFPSMeasured;
    float m_lastFrameTime;
    Vec2I m_lastMousePos;
};

#endif // GAME_H
