#ifndef COMPONENTSHOP_H
#define COMPONENTSHOP_H

#include <vector>

#include "ComponentShopDisplay.h"

class ComponentShop
{
public:
    ComponentShop(Game& game, sf::RenderWindow& window);
    void addDisplay(const std::string& filePath, WindowSpaceManager::WindowRegionId windowRegion);

    void draw();
    void update(double dt);

    void processMouseInput(sf::Event& mouseEvent);

    const std::shared_ptr<Component> pointedComponent(const Vec2I& windowCoords) const;
    const std::shared_ptr<Component>& selectedComponent() const;
    void scrollDisplay(const Vec2I& mousePos, const Vec2F& offset);

    void onWindowResized(sf::Event& windowResizedEvent);
protected:
    Game& m_game;
    sf::RenderWindow& m_window;
    std::vector<ComponentShopDisplay> m_displays;
    std::shared_ptr<Component> m_selectedComponent;
};

#endif // COMPONENTSHOP_H
