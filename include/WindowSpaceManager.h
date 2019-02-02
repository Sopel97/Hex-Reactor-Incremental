#ifndef WINDOWSPACEMANAGER_H
#define WINDOWSPACEMANAGER_H

#include <array>

#include "../LibS/Geometry.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "SegmentStructure.h"

using namespace Geo;

class ReactorGrid;
class ComponentShopDisplay;
class UserInterface;
class UpgradeShop;

class WindowSpaceManager
{
public:

    enum class WindowRegionId
    {
        TopPanel,
        BottomPanel,
        LeftPanel,
        RightPanel,
        TopPanelInner,
        BottomPanelInner,
        LeftPanelInner,
        RightPanelInner,
        ReactorGrid,
        UpgradesScreenPanel,
        UpgradesScreenPanelInner,
        NumberOfWindowRegions
    };
    enum class SceneId
    {
        Main,
        Upgrades
    };
    class WindowRegion
    {
public:
        WindowRegion() = default;
        WindowRegion(const RectangleI& rect, WindowRegionId regionId, const std::vector<SceneId>& sceneIds, int z = 0);
        WindowRegion(const WindowRegion&) = default;
        WindowRegion(WindowRegion&&) = default;

        WindowRegion& operator=(const WindowRegion&) = default;
        WindowRegion& operator=(WindowRegion&&) = default;

        Vec2I localCoords(const Vec2I& windowCoords) const;
        const RectangleI& rect() const;
        WindowRegionId regionId() const;
        const std::vector<SceneId>& parentScenesIds() const;
        bool isPresentOnScene(SceneId sceneId) const;
        int z() const;
    protected:
        RectangleI m_rect;
        WindowRegionId m_regionId;
        std::vector<SceneId> m_parentScenesIds;
        int m_z; //determines priority. Higher means bigger priority
    };
public:
    WindowSpaceManager(sf::RenderWindow& window);
    void onWindowResized(sf::Event& windowResizedEvent);
    void setViewToRect(const RectangleI& windowRect, const RectangleI& viewRect);
    void setViewToWindowRegion(WindowRegionId region, Vec2F camera = Vec2F{0.0f, 0.0f});

    void setDefaultView();
    void setRects();

    const std::map<SceneId, std::vector<RectangleI>>& walls() const;
    const std::vector<RectangleI>& wallsOnScene(SceneId scene) const;
    SceneId currentScene() const;
    void setCurrentScene(SceneId scene);

    RectangleI panelInnerRect(const RectangleI& rect);

    const std::map<WindowRegionId, WindowRegion>& windowRegions() const;
    const WindowRegion& windowRegion(WindowRegionId region) const;
    const WindowRegion* pointedWindowRegion(const Vec2I& windowCoords) const;
    const RectangleI& windowRegionRect(WindowRegionId region) const;

    SegmentStructure& sceneControls();
    const Vec2F& sceneControlsOffset();
protected:
    sf::RenderWindow& m_window;
    SceneId m_currentScene;
    std::map<SceneId, std::vector<RectangleI>> m_walls;

    std::map<WindowRegionId, WindowRegion> m_windowRegions;

    Vec2F m_sceneControlsOffset;
    SegmentStructure m_sceneControls;
};

#endif // WINDOWSPACEMANAGER_H
