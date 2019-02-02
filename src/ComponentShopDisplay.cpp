#include "ComponentShopDisplay.h"

#include "Root.h"
#include "ComponentFactory.h"
#include "Game.h"
#include "Component.h"
#include "SFMLUtil.h"
#include "PlayerStats.h"
#include "ComponentShop.h"
#include "ScrollBar.h"

ComponentShopDisplay::ComponentShopDisplay(Game& game, sf::RenderWindow& window, ComponentShop& componentShop, WindowSpaceManager::WindowRegionId windowRegion, const std::string& configPath) :
    m_game(game),
    m_window(window),
    m_parentComponentShop(componentShop),
    m_windowRegion(windowRegion),
    m_camera(0.0f, 0.0f),
    m_lastRow(0)
{
    m_viewRect = game.windowSpaceManager().windowRegionRect(windowRegion);
    ComponentFactory& componentFactory = game.componentFactory();
    Configuration config(configPath);
    ConfigurationNode shopConfig = config["shop"];
    ConfigurationNode rows = shopConfig["rows"];
    unsigned numberOfRows = rows.length();
    for(unsigned i = 1; i <= numberOfRows; ++i)
    {
        ConfigurationNode row = rows[i];
        unsigned rowNumber = row["row"].getDefault<int>(i - 1);
        ConfigurationNode components = row["components"];
        BigInt moneyToUnlock = std::stoll(row["moneyToUnlock"].getDefault<std::string>("0"));
        m_displayRows[rowNumber].moneyToUnlock = moneyToUnlock;
        unsigned numberOfComponents = components.length();
        for(unsigned j = 1; j <= numberOfComponents; ++j)
        {
            std::string componentName = components[j].get<std::string>();
            registerComponent(componentFactory.component(componentName), rowNumber);
        }
    }
}
void ComponentShopDisplay::update(double dt)
{
    PlayerStats& playerStats = m_game.playerStats();
    BigInt playerMoney = playerStats.money();
    for(auto& entry : m_displayRows)
    {
        auto& displayRow = entry.second;
        if(playerMoney >= displayRow.moneyToUnlock)
        {
            displayRow.isUnlocked = true;
        }
    }
}
const RectangleI& ComponentShopDisplay::viewRect() const
{
    return m_viewRect;
}
WindowSpaceManager::WindowRegionId ComponentShopDisplay::windowRegion() const
{
    return m_windowRegion;
}
void ComponentShopDisplay::draw()
{
    m_game.windowSpaceManager().setViewToWindowRegion(m_windowRegion, m_camera);
    PlayerStats& playerStats = m_game.playerStats();
    BigInt playerMoney = playerStats.money();
    auto isVisible = [] (const RectangleI & rect, const Vec2F & pos) -> bool
    {
        return rect.intersects(RectangleI(pos, 32, 32));
    };
    const sf::Texture& texture = Root::instance().resources().textures.spritesheet;
    const auto& renderStates = Root::instance().renderStates();
    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles);
    for(const auto& entry : m_displayRows)
    {
        const auto& displayRow = entry.second;
        const unsigned& rowNumber = entry.first;
        Vec2F firstComponentPos = calculateFirstComponentPosition(rowNumber, displayRow.components.size());
        int i = 0;
        for(const auto& component : displayRow.components)
        {
            bool canAfford = playerMoney >= component->price();

            if(component == nullptr) continue;

            Vec2F pos = firstComponentPos + Vec2F {(float)i* (m_colMargin + m_componentSize), 0.0f};
            if(!isVisible(m_viewRect, pos + m_viewRect.min - m_camera)) continue;
            sf::Color backgroundColor = sf::Color(255, 255, 255, 48);
            auto& selectedComponent = m_parentComponentShop.selectedComponent();
            if(selectedComponent != nullptr)
            {
                std::string selectedComponentName = selectedComponent->name();
                if(selectedComponentName == component->name())
                {
                    if(canAfford) backgroundColor = sf::Color(0, 255, 0, 128);
                    else backgroundColor = sf::Color(255, 0, 0, 128);
                }
            }

            Vec2F position = pos - Vec2F {2.0f, 2.0f};
            Vec2F size(36.0f, 36.0f);

            SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, Vec2F {0.0f, 0.0f}, size, backgroundColor);

            ++i;
        }
    }
    m_window.draw(vertexArray, renderStates);

    vertexArray.clear();
    for(const auto& entry : m_displayRows)
    {
        const auto& displayRow = entry.second;
        const unsigned& rowNumber = entry.first;
        Vec2F firstComponentPos = calculateFirstComponentPosition(rowNumber, displayRow.components.size());
        bool isUnlocked = displayRow.isUnlocked;
        int i = 0;
        for(const auto& component : displayRow.components)
        {
            bool canAfford = playerMoney >= component->price();
            if(component == nullptr) continue;

            Vec2F position = firstComponentPos + Vec2F {(float)i* (m_colMargin + m_componentSize), 0.0f};
            if(!isVisible(m_viewRect, position + m_viewRect.min - m_camera)) continue;
            sf::Color color;
            if(isUnlocked)
            {
                if(canAfford) color = sf::Color::White;
                else color = sf::Color(64, 64, 64);
            }
            else
            {
                color = sf::Color::Black;
            }

            Vec2F spritePosition = component->spritePosition();
            Vec2F size(32.0f, 32.0f);

            SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, spritePosition, size, color);
            if(!isUnlocked) SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, Vec2F {804.0f, 134.0f}, size, sf::Color::White);

            ++i;
        }
    }
    Root::instance().setActiveTexture(texture);
    m_window.draw(vertexArray, renderStates);
    Root::instance().unsetActiveTexture();

    m_game.windowSpaceManager().setDefaultView();

    ScrollBar::draw(m_window, Root::instance().renderStates(), m_game.windowSpaceManager().windowRegionRect(m_windowRegion), 0.0f, m_camera.y, height(), ScrollBar::Side::Left);
}

void ComponentShopDisplay::registerComponent(const std::shared_ptr<Component>& component, unsigned row)
{
    m_displayRows[row].components.push_back(component);
    m_lastRow = std::max(row, m_lastRow);
}

const std::shared_ptr<Component> ComponentShopDisplay::pointedComponent(const Vec2I& localCoords) const
{
    if(localCoords.y < 0.0f || localCoords.y > m_viewRect.height()) return nullptr; //to avoid checking not rendered places
    int rowInteger = (localCoords.y + m_camera.y) / (m_componentSize + m_rowMargin);
    int rowRemainder = (int)(localCoords.y + m_camera.y) % (m_componentSize + m_rowMargin);
    if(rowRemainder > m_componentSize || rowInteger < 0) return nullptr;
    auto iter = m_displayRows.find(rowInteger);
    if(iter != m_displayRows.end())
    {
        const auto& displayRow = iter->second;
        if(!displayRow.isUnlocked) return nullptr;

        auto& rowComponents = displayRow.components;
        int numberOfComponents = rowComponents.size();
        Vec2F firstComponentPosition = calculateFirstComponentPosition(rowInteger, numberOfComponents);
        int firstComponentX = firstComponentPosition.x;
        if(localCoords.x < firstComponentX) return nullptr;
        int remainder = (localCoords.x - firstComponentX) % (m_colMargin + m_componentSize);
        int integer = (localCoords.x - firstComponentX) / (m_colMargin + m_componentSize);
        if(remainder > m_componentSize || integer >= numberOfComponents || integer < 0) return nullptr;
        else return rowComponents[integer];
    }
    return nullptr;
}
const Vec2F& ComponentShopDisplay::camera() const
{
    return m_camera;
}
void ComponentShopDisplay::updateCamera()
{
    moveCamera(Vec2F {0.0f, 0.0f});
}
void ComponentShopDisplay::moveCamera(const Vec2F& displacement)
{
    m_camera += displacement;
    adjustCamera();
}
void ComponentShopDisplay::adjustCamera()
{
    int h = height();
    int viewHeight = m_viewRect.height();
    m_camera.x = 0.0f; //until side scrolling will be done somehow
    if(m_camera.y < 0.0f) m_camera.y = 0.0f;
    if(m_camera.y > h - viewHeight) m_camera.y = h - viewHeight;

    if(viewHeight > h) m_camera.y = 0.0f;
}
Vec2F ComponentShopDisplay::calculateFirstComponentPosition(unsigned row, unsigned numberOfComponents) const
{
    unsigned viewWidth = m_viewRect.width();
    unsigned componentsWidth = numberOfComponents * (m_colMargin + m_componentSize) - m_colMargin;
    Vec2F center {(float)(viewWidth / 2.0f), (float)(row * (m_componentSize + m_rowMargin))};
    return center - Vec2F {componentsWidth / 2.0f, -m_topMargin};
}
void ComponentShopDisplay::onWindowResized(sf::Event& windowResizedEvent)
{
    setViewRect(m_game.windowSpaceManager().windowRegionRect(m_windowRegion));
    adjustCamera();
}
void ComponentShopDisplay::setViewRect(const RectangleI& rect)
{
    m_viewRect = rect;
}
int ComponentShopDisplay::rowMargin() const
{
    return m_rowMargin;
}
int ComponentShopDisplay::colMargin() const
{
    return m_colMargin;
}
int ComponentShopDisplay::height() const
{
    return (m_lastRow + 1) * (m_componentSize + m_rowMargin) - m_rowMargin + m_topMargin + m_bottomMargin;
}
