#include "WindowSpaceManager.h"
#include "ReactorGrid.h"
#include "Root.h"
#include "UpgradePool.h"
#include "UserInterface.h"

#include <limits>

WindowSpaceManager::WindowRegion::WindowRegion(const RectangleI& rect, WindowSpaceManager::WindowRegionId regionId, const std::vector<WindowSpaceManager::SceneId>& sceneIds, int z) :
    m_rect(rect),
    m_regionId(regionId),
    m_parentScenesIds(sceneIds),
    m_z(z)
{

}
Vec2I WindowSpaceManager::WindowRegion::localCoords(const Vec2I& windowCoords) const
{
    return windowCoords - m_rect.min;
}
const RectangleI& WindowSpaceManager::WindowRegion::rect() const
{
    return m_rect;
}
WindowSpaceManager::WindowRegionId WindowSpaceManager::WindowRegion::regionId() const
{
    return m_regionId;
}
const std::vector<WindowSpaceManager::SceneId>& WindowSpaceManager::WindowRegion::parentScenesIds() const
{
    return m_parentScenesIds;
}
int WindowSpaceManager::WindowRegion::z() const
{
    return m_z;
}
bool WindowSpaceManager::WindowRegion::isPresentOnScene(WindowSpaceManager::SceneId sceneId) const
{
    return std::find(m_parentScenesIds.begin(), m_parentScenesIds.end(), sceneId) != m_parentScenesIds.end();
}
WindowSpaceManager::WindowSpaceManager(sf::RenderWindow& window) :
    m_window(window),
    m_currentScene(WindowSpaceManager::SceneId::Main),
    m_sceneControlsOffset {0.0f, 0.0f},
                      m_sceneControls
{
    std::vector<Segment>
    {
        Segment{
            "ReactorSelected",
            Vec2F{704, 217},
            Vec2F{213, 0.0f},
            Vec2F{228.0f, 51.0f},
            PolygonF
            {
                Vec2F{25, 0},
                Vec2F{227, 0},
                Vec2F{177, 50},
                Vec2F{25, 50},
                Vec2F{0, 25}
            }
        },
        Segment{
            "ReactorUnselected",
            Vec2F{704, 268},
            Vec2F{213, 0.0f},
            Vec2F{228.0f, 51.0f},
            PolygonF
            {
                Vec2F{25, 0},
                Vec2F{227, 0},
                Vec2F{177, 50},
                Vec2F{25, 50},
                Vec2F{0, 25}
            }
        },
        Segment{
            "UpgradesSelected",
            Vec2F{704, 319},
            Vec2F{391, 0.0f},
            Vec2F{228.0f, 51.0f},
            PolygonF
            {
                Vec2F{50, 0},
                Vec2F{202, 0},
                Vec2F{227, 25},
                Vec2F{202, 50},
                Vec2F{0, 50}
            }
        },
        Segment{
            "UpgradesUnselected",
            Vec2F{704, 370},
            Vec2F{391, 0.0f},
            Vec2F{228.0f, 51},
            PolygonF
            {
                Vec2F{50, 0},
                Vec2F{202, 0},
                Vec2F{227, 25},
                Vec2F{202, 50},
                Vec2F{0, 50}
            }
        },

        Segment{
            "ReactorOn",
            Vec2F{824, 166},
            Vec2F{0, 0},
            Vec2F{96, 51},
            PolygonF
            {
                Vec2F{25, 0},
                Vec2F{95, 0},
                Vec2F{70, 25},
                Vec2F{95, 50},
                Vec2F{25, 50},
                Vec2F{0, 25}
            }
        },
        Segment{
            "ReactorOff",
            Vec2F{923, 166},
            Vec2F{0, 0},
            Vec2F{96, 51},
            PolygonF
            {
                Vec2F{25, 0},
                Vec2F{95, 0},
                Vec2F{70, 25},
                Vec2F{95, 50},
                Vec2F{25, 50},
                Vec2F{0, 25}
            }
        },
        Segment{
            "AutoReplaceCellsOn",
            Vec2F{932, 268},
            Vec2F{71, 0},
            Vec2F{96, 51},
            PolygonF
            {
                Vec2F{25, 0},
                Vec2F{95, 0},
                Vec2F{70, 25},
                Vec2F{95, 50},
                Vec2F{25, 50},
                Vec2F{0, 25}
            }
        },
        Segment{
            "AutoReplaceCellsOff",
            Vec2F{932, 217},
            Vec2F{71, 0},
            Vec2F{96, 51},
            PolygonF
            {
                Vec2F{25, 0},
                Vec2F{95, 0},
                Vec2F{70, 25},
                Vec2F{95, 50},
                Vec2F{25, 50},
                Vec2F{0, 25}
            }
        },
        Segment{
            "AutoReplaceNeutronReflectorsOn",
            Vec2F{932, 370},
            Vec2F{142, 0},
            Vec2F{96, 51},
            PolygonF
            {
                Vec2F{25, 0},
                Vec2F{95, 0},
                Vec2F{70, 25},
                Vec2F{95, 50},
                Vec2F{25, 50},
                Vec2F{0, 25}
            }
        },
        Segment{
            "AutoReplaceNeutronReflectorsOff",
            Vec2F{932, 319},
            Vec2F{142, 0},
            Vec2F{96, 51},
            PolygonF
            {
                Vec2F{25, 0},
                Vec2F{95, 0},
                Vec2F{70, 25},
                Vec2F{95, 50},
                Vec2F{25, 50},
                Vec2F{0, 25}
            }
        }

    },
    Vec2F{419.0f, 25.0f}//forced center
}
{
    setRects();
}
void WindowSpaceManager::setRects()
{
    constexpr int uiTopPanelHeight = 250;
    constexpr int uiBottomPanelHeight = 150;
    constexpr int uiSidePanelsWidths = 300;
    constexpr int uiWallSize = 24;

    int windowWidth = m_window.getSize().x;
    int windowHeight = m_window.getSize().y;

    m_walls.clear();
    m_walls[WindowSpaceManager::SceneId::Main].emplace_back(Vec2I { -uiWallSize, uiTopPanelHeight}, windowWidth + 2 * uiWallSize, 24); //top
    m_walls[WindowSpaceManager::SceneId::Main].emplace_back(Vec2I { -uiWallSize, windowHeight - uiBottomPanelHeight - uiWallSize}, windowWidth + 2 * uiWallSize, 24); //bottom
    m_walls[WindowSpaceManager::SceneId::Main].emplace_back(Vec2I {uiSidePanelsWidths, uiTopPanelHeight}, 24, windowHeight - (uiTopPanelHeight + uiBottomPanelHeight)); //left
    m_walls[WindowSpaceManager::SceneId::Main].emplace_back(Vec2I {windowWidth - uiSidePanelsWidths - uiWallSize, uiTopPanelHeight}, 24, windowHeight - (uiTopPanelHeight + uiBottomPanelHeight)); //right

    m_walls[WindowSpaceManager::SceneId::Upgrades].emplace_back(Vec2I { -uiWallSize, uiTopPanelHeight}, windowWidth + 2 * uiWallSize, 24); //top

    m_windowRegions[WindowSpaceManager::WindowRegionId::ReactorGrid] = WindowSpaceManager::WindowRegion
    {
        RectangleI
        {
            Vec2I {uiSidePanelsWidths + uiWallSize, uiTopPanelHeight + uiWallSize},
            Vec2I {windowWidth, windowHeight} - Vec2I {uiSidePanelsWidths + uiWallSize, uiBottomPanelHeight + uiWallSize}
        },
        WindowSpaceManager::WindowRegionId::ReactorGrid,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Main
        },
        0
    };

    m_windowRegions[WindowSpaceManager::WindowRegionId::TopPanel] = WindowSpaceManager::WindowRegion
    {
        RectangleI
        {
            Vec2I {0, 0},
            windowWidth,
            uiTopPanelHeight
        },
        WindowSpaceManager::WindowRegionId::TopPanel,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Main,
            WindowSpaceManager::SceneId::Upgrades
        },
        0
    };
    m_windowRegions[WindowSpaceManager::WindowRegionId::BottomPanel] = WindowSpaceManager::WindowRegion
    {
        RectangleI
        {
            Vec2I {0, windowHeight - uiBottomPanelHeight},
            windowWidth,
            uiBottomPanelHeight
        },
        WindowSpaceManager::WindowRegionId::BottomPanel,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Main
        },
        0
    };
    m_windowRegions[WindowSpaceManager::WindowRegionId::LeftPanel] = WindowSpaceManager::WindowRegion
    {
        RectangleI
        {
            Vec2I {0, uiTopPanelHeight + uiWallSize},
            uiSidePanelsWidths,
            windowHeight - (uiTopPanelHeight + uiBottomPanelHeight) - 2 * uiWallSize
        },
        WindowSpaceManager::WindowRegionId::LeftPanel,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Main
        },
        0
    };
    m_windowRegions[WindowSpaceManager::WindowRegionId::RightPanel] = WindowSpaceManager::WindowRegion
    {
        RectangleI
        {
            Vec2I {windowWidth - uiSidePanelsWidths, uiTopPanelHeight + uiWallSize},
            uiSidePanelsWidths,
            windowHeight - (uiTopPanelHeight + uiBottomPanelHeight) - 2 * uiWallSize
        },
        WindowSpaceManager::WindowRegionId::RightPanel,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Main
        },
        0
    };
    m_windowRegions[WindowSpaceManager::WindowRegionId::UpgradesScreenPanel] = WindowSpaceManager::WindowRegion
    {
        RectangleI
        {
            Vec2I {0, uiTopPanelHeight + uiWallSize},
            windowWidth,
            windowHeight - uiTopPanelHeight - uiWallSize
        },
        WindowSpaceManager::WindowRegionId::UpgradesScreenPanel,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Upgrades
        },
        0
    };

    m_windowRegions[WindowSpaceManager::WindowRegionId::TopPanelInner] = WindowSpaceManager::WindowRegion
    {
        panelInnerRect(windowRegionRect(WindowSpaceManager::WindowRegionId::TopPanel)),
        WindowSpaceManager::WindowRegionId::TopPanelInner,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Main,
            WindowSpaceManager::SceneId::Upgrades
        },
        1
    };
    m_windowRegions[WindowSpaceManager::WindowRegionId::BottomPanelInner] = WindowSpaceManager::WindowRegion
    {
        panelInnerRect(windowRegionRect(WindowSpaceManager::WindowRegionId::BottomPanel)),
        WindowSpaceManager::WindowRegionId::BottomPanelInner,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Main
        },
        1
    };
    m_windowRegions[WindowSpaceManager::WindowRegionId::LeftPanelInner] = WindowSpaceManager::WindowRegion
    {
        panelInnerRect(windowRegionRect(WindowSpaceManager::WindowRegionId::LeftPanel)),
        WindowSpaceManager::WindowRegionId::LeftPanelInner,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Main
        },
        1
    };
    m_windowRegions[WindowSpaceManager::WindowRegionId::RightPanelInner] = WindowSpaceManager::WindowRegion
    {
        panelInnerRect(windowRegionRect(WindowSpaceManager::WindowRegionId::RightPanel)),
        WindowSpaceManager::WindowRegionId::RightPanelInner,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Main
        },
        1
    };
    m_windowRegions[WindowSpaceManager::WindowRegionId::UpgradesScreenPanelInner] = WindowSpaceManager::WindowRegion
    {
        panelInnerRect(windowRegionRect(WindowSpaceManager::WindowRegionId::UpgradesScreenPanel)),
        WindowSpaceManager::WindowRegionId::UpgradesScreenPanelInner,
        std::vector<SceneId>
        {
            WindowSpaceManager::SceneId::Upgrades
        },
        1
    };

    const auto& topPanelInnerRect = windowRegionRect(WindowSpaceManager::WindowRegionId::TopPanelInner);
    m_sceneControlsOffset.x = std::max(32.0f, m_sceneControls.offsetToBeCentered(topPanelInnerRect).x);
    m_sceneControlsOffset.y = (topPanelInnerRect.min.y + topPanelInnerRect.height()) - m_sceneControls.height() - 6;
}
WindowSpaceManager::SceneId WindowSpaceManager::currentScene() const
{
    return m_currentScene;
}
void WindowSpaceManager::setCurrentScene(WindowSpaceManager::SceneId scene)
{
    m_currentScene = scene;
}
void WindowSpaceManager::setViewToRect(const RectangleI& windowViewRect, const RectangleI& worldViewRect)
{
    Vec2F windowSize {(float)m_window.getSize().x, (float)m_window.getSize().y};
    Vec2F windowViewTopLeft = windowViewRect.min;
    Vec2F windowViewSize = windowViewRect.max - windowViewRect.min;
    Vec2F windowViewTopLeftRelativeToWindow = windowViewTopLeft / windowSize;
    Vec2F windowViewSizeRelativeToWindow = windowViewSize / windowSize;

    sf::FloatRect view(windowViewTopLeftRelativeToWindow.x, windowViewTopLeftRelativeToWindow.y, windowViewSizeRelativeToWindow.x, windowViewSizeRelativeToWindow.y);
    sf::View panelView(sf::Vector2f(worldViewRect.min.x + (worldViewRect.width()) / 2.0f, (worldViewRect.min.y + (worldViewRect.height()) / 2.0f)), sf::Vector2f(worldViewRect.width(), worldViewRect.height()));
    panelView.setViewport(view);
    m_window.setView(panelView);
}
void WindowSpaceManager::onWindowResized(sf::Event& windowResizedEvent)
{
    setRects();
}
void WindowSpaceManager::setViewToWindowRegion(WindowRegionId region, Vec2F camera)
{
    const RectangleI& rect = windowRegionRect(region);
    setViewToRect(rect, RectangleI(camera, rect.width(), rect.height()));
}
void WindowSpaceManager::setDefaultView()
{
    Vec2F windowSize {(float)m_window.getSize().x, (float)m_window.getSize().y};

    sf::FloatRect view(0.0f, 0.0f, 1.0f, 1.0f);
    sf::View panelView(sf::Vector2f((windowSize.x) / 2.0f, ((windowSize.y) / 2.0f)), sf::Vector2f(windowSize.x, windowSize.y));
    panelView.setViewport(view);
    m_window.setView(panelView);
}

const std::map<WindowSpaceManager::SceneId, std::vector<RectangleI>>& WindowSpaceManager::walls() const
{
    return m_walls;
}
const std::vector<RectangleI>& WindowSpaceManager::wallsOnScene(WindowSpaceManager::SceneId scene) const
{
    return m_walls.at(scene);
}
RectangleI WindowSpaceManager::panelInnerRect(const RectangleI& rect)
{
    constexpr int panelMargin = 32;
    RectangleI rectCopy = rect;
    rectCopy.min += Vec2I {panelMargin, panelMargin};
    rectCopy.max -= Vec2I {panelMargin, panelMargin};
    return rectCopy;
}

const std::map<WindowSpaceManager::WindowRegionId, WindowSpaceManager::WindowRegion>& WindowSpaceManager::windowRegions() const
{
    return m_windowRegions;
}
const WindowSpaceManager::WindowRegion& WindowSpaceManager::windowRegion(WindowSpaceManager::WindowRegionId region) const
{
    return m_windowRegions.at(region);
}
const WindowSpaceManager::WindowRegion* WindowSpaceManager::pointedWindowRegion(const Vec2I& windowCoords) const
{
    const WindowRegion* candidate = nullptr;
    int highestZ = std::numeric_limits<int>::min();
    for(const auto& windowRegion : m_windowRegions)
    {
        const WindowRegion& region = windowRegion.second;
        if(!region.isPresentOnScene(m_currentScene)) continue;
        if(region.z() < highestZ) continue;
        if(region.rect().intersects(windowCoords))
        {
            candidate = &region;
            highestZ = region.z();
        }
    }
    return candidate;
}
SegmentStructure& WindowSpaceManager::sceneControls()
{
    return m_sceneControls;
}
const Vec2F& WindowSpaceManager::sceneControlsOffset()
{
    return m_sceneControlsOffset;
}
const RectangleI& WindowSpaceManager::windowRegionRect(WindowRegionId region) const
{
    return windowRegion(region).rect();
}
