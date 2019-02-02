#include "ComponentShop.h"
#include "Game.h"

ComponentShop::ComponentShop(Game& game, sf::RenderWindow& window) :
    m_game(game),
    m_window(window),
    m_selectedComponent(nullptr)
{
}

void ComponentShop::addDisplay(const std::string& filePath, WindowSpaceManager::WindowRegionId windowRegion)
{
    m_displays.emplace_back(m_game, m_window, *this, windowRegion, filePath);
}

void ComponentShop::processMouseInput(sf::Event& mouseEvent)
{
    if(mouseEvent.type == sf::Event::EventType::MouseButtonPressed)
    {
        Vec2I mousePos {mouseEvent.mouseButton.x, mouseEvent.mouseButton.y};
        auto pointedWindowRegion = m_game.windowSpaceManager().pointedWindowRegion(mousePos);
        if(pointedWindowRegion == nullptr) return;
        auto pointed = pointedComponent(mousePos);
        if(pointed != nullptr) m_selectedComponent = pointed;
    }
    else if(mouseEvent.type == sf::Event::EventType::MouseWheelMoved)
    {
        Vec2I mousePos {mouseEvent.mouseWheel.x, mouseEvent.mouseWheel.y};
        scrollDisplay(mousePos, Vec2F {0.0f, -mouseEvent.mouseWheel.delta * 15.0f});
    }
}

void ComponentShop::draw()
{
    for(auto& display : m_displays)
    {
        display.draw();
    }
}
void ComponentShop::update(double dt)
{
    for(auto& display : m_displays)
    {
        display.update(dt);
    }
}

const std::shared_ptr<Component> ComponentShop::pointedComponent(const Vec2I& windowCoords) const
{
    auto region = m_game.windowSpaceManager().pointedWindowRegion(windowCoords);
    if(region == nullptr) return nullptr;
    for(auto& display : m_displays)
    {
        if(region->regionId() == display.windowRegion())
        {
            return display.pointedComponent(region->localCoords(windowCoords));
        }
    }
    return nullptr;
}
void ComponentShop::scrollDisplay(const Vec2I& mousePos, const Vec2F& offset)
{
    auto region = m_game.windowSpaceManager().pointedWindowRegion(mousePos);
    if(region == nullptr) return;
    for(auto& display : m_displays)
    {
        if(region->regionId() == display.windowRegion())
        {
            display.moveCamera(offset);
        }
    }
}

const std::shared_ptr<Component>& ComponentShop::selectedComponent() const
{
    return m_selectedComponent;
}
void ComponentShop::onWindowResized(sf::Event& windowResizedEvent)
{
    for(auto& display : m_displays)
    {
        display.onWindowResized(windowResizedEvent);
    }
}
