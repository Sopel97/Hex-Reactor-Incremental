#ifndef COMPONENTSHOPDISPLAY_H
#define COMPONENTSHOPDISPLAY_H

#include <vector>
#include <map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../LibS/Geometry.h"

#include "Configuration.h"
#include "Component.h"
#include "Bignum.h"
#include "WindowSpaceManager.h"

#include <memory>

class Game;
class ComponentShop;

using namespace Geo;

class ComponentShopDisplay
{
public:
    ComponentShopDisplay(Game& game, sf::RenderWindow& window, ComponentShop& componentShop, WindowSpaceManager::WindowRegionId windowRegion, const std::string& configPath);

    void draw();
    void update(double dt);

    const RectangleI& viewRect() const;
    WindowSpaceManager::WindowRegionId windowRegion() const;

    void registerComponent(const std::shared_ptr<Component>& component, unsigned row);

    const std::shared_ptr<Component> pointedComponent(const Vec2I& localCoords) const;

    const Vec2F& camera() const;
    void updateCamera();
    void moveCamera(const Vec2F& displacement);
    void adjustCamera();

    void onWindowResized(sf::Event& windowResizedEvent);
    void setViewRect(const RectangleI& rect);

    int rowMargin() const;
    int colMargin() const;

    int height() const;

protected:
    struct DisplayRow
    {
        DisplayRow() = default;
        std::vector<std::shared_ptr<Component>> components;
        BigInt moneyToUnlock;
        bool isUnlocked = false;
    };
    Vec2F calculateFirstComponentPosition(unsigned row, unsigned numberOfComponents) const;

    static const int m_rowMargin = 8;
    static const int m_colMargin = 8;
    static const int m_topMargin = 2; //to make some space for background on first row
    static const int m_bottomMargin = 2;
    static const int m_componentSize = 32;

    Game& m_game;
    sf::RenderWindow& m_window;
    ComponentShop& m_parentComponentShop;
    std::map<unsigned, DisplayRow> m_displayRows; //map of rows of items (map since rows may not be continuous)
    WindowSpaceManager::WindowRegionId m_windowRegion;
    RectangleI m_viewRect;
    Vec2F m_camera; //vertical scrolling may be enough, but why not
    unsigned m_lastRow;
};

#endif // COMPONENTSHOPDISPLAY_H
