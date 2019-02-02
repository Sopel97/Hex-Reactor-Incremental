#include "UpgradeShop.h"
#include "Root.h"
#include "UpgradePool.h"
#include "ScrollBar.h"

UpgradeShop::UpgradeShop(Game& game, sf::RenderWindow& window, WindowSpaceManager::WindowRegionId windowRegion) :
    m_game(game),
    m_window(window),
    m_windowRegion(windowRegion),
    m_camera(0.0f, 0.0f)
{
    m_viewRect = m_game.windowSpaceManager().windowRegionRect(windowRegion);
    setUpgradesInterfaces();
}
void UpgradeShop::draw() //objects displayed are temporary. Later it will take data from Upgrades class instance (there will be a camera also)
{
    m_game.windowSpaceManager().setViewToWindowRegion(m_windowRegion, m_camera);
    for(auto& upgradeInterface : m_upgradesInterfaces)
    {
        drawUpgradeInterface(upgradeInterface);
    }
    m_game.windowSpaceManager().setDefaultView();
    ScrollBar::draw(m_window, Root::instance().renderStates(), m_game.windowSpaceManager().windowRegionRect(m_windowRegion), 0.0f, m_camera.y, height(), ScrollBar::Side::Left);
}
void UpgradeShop::update(double dt)
{
    for(auto& interface : m_upgradesInterfaces)
    {
        interface.update(dt);
    }
}
void UpgradeShop::drawUpgradeInterface(UpgradeInterface& interface)
{
    interface.draw(m_window);
}

void UpgradeShop::setUpgradesInterfaces()
{
    constexpr int topMargin = 0;
    constexpr int verticalSpacing = 36;
    constexpr int horizontalSpacing = 36;
    int viewRectWidth = m_viewRect.width();
    m_upgradesInterfaces = UpgradePool::instance().createInterfaceForEveryUpgrade();
    int numberOfColumns = (viewRectWidth + horizontalSpacing) / (UpgradeInterface::width() + horizontalSpacing);
    int currentRow = 0;
    int currentColumn = 0;
    for(auto& interface : m_upgradesInterfaces)
    {
        float left = (viewRectWidth / 2) - ((UpgradeInterface::width() + horizontalSpacing) * numberOfColumns - horizontalSpacing) / 2 + (UpgradeInterface::width() + horizontalSpacing) * currentColumn;
        interface.setWorldOffset(Vec2I (left, topMargin + currentRow * (UpgradeInterface::height() + verticalSpacing)));
        ++currentColumn;
        if(currentColumn >= numberOfColumns)
        {
            currentColumn = 0;
            ++currentRow;
        }
    }
}
void UpgradeShop::moveCamera(Vec2F displacement)
{
    m_camera += displacement;
    adjustCamera();
}
Vec2F UpgradeShop::camera() const
{
    return m_camera;
}
float UpgradeShop::height() const
{
    return m_upgradesInterfaces.back().worldOffset().y + UpgradeInterface::height();
}

void UpgradeShop::adjustCamera()
{
    float maxCameraY = height() - m_viewRect.height();
    if(m_camera.y > maxCameraY) m_camera.y = maxCameraY;
    if(m_camera.y < 0.0f) m_camera.y = 0.0f;
}
const RectangleI& UpgradeShop::viewRect() const
{
    return m_viewRect;
}
void UpgradeShop::onMouseButtonPressed(const Vec2I& localCoords)
{
    Vec2I localCoordsWithCameraOffset = localCoords + m_camera;
    for(auto& upgradeInterface : m_upgradesInterfaces)
    {
        if(RectangleI(upgradeInterface.worldOffset(), UpgradeInterface::width(), UpgradeInterface::height()).intersects(localCoordsWithCameraOffset))
        {
            upgradeInterface.onMouseButtonPressed(localCoordsWithCameraOffset - upgradeInterface.worldOffset());
            break;
        }
    }
}
void UpgradeShop::onWindowResized(sf::Event& windowResizedEvent)
{
    m_viewRect = m_game.windowSpaceManager().windowRegionRect(m_windowRegion);
    setUpgradesInterfaces();
    adjustCamera();
}
