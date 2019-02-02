#ifndef UPGRADESHOP_H
#define UPGRADESHOP_H

#include "UpgradeInterface.h"
#include "WindowSpaceManager.h"

class Game;

class UpgradeShop
{
public:
    UpgradeShop(Game& game, sf::RenderWindow& window, WindowSpaceManager::WindowRegionId windowRegion);

    void draw();
    void update(double dt);

    void setUpgradesInterfaces();
    const std::vector<UpgradeInterface>& upgradesInterfaces() const;
    void drawUpgradeInterface(UpgradeInterface& interface);

    Vec2F camera() const;
    void moveCamera(Vec2F displacement);
    void adjustCamera();

    const RectangleI& viewRect() const;

    void onMouseButtonPressed(const Vec2I& localCoords);

    float height() const;

    void onWindowResized(sf::Event& windowResizedEvent);
protected:
    Game& m_game;
    sf::RenderWindow& m_window;
    RectangleI m_viewRect;
    WindowSpaceManager::WindowRegionId m_windowRegion;
    std::vector<UpgradeInterface> m_upgradesInterfaces;
    Vec2F m_camera;
};

#endif // UPGRADESHOP_H
