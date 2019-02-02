#ifndef USERINTERFACE_H
#define USERINTERFACE_H


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "../LibS/Geometry.h"
#include "SegmentStructure.h"

class Game;
class UpgradeInterface;

using namespace Geo;

class UserInterface
{
public:
    UserInterface(Game& game, sf::RenderWindow& window);
    void draw();
    void drawFramerate();
    void drawWalls(const std::vector<RectangleI>& sceneWalls);
    void drawSceneControls();

    void drawPanel(const RectangleI& rect);
    void drawPanelBackground(const RectangleI& rect);
    void drawPanelGradientBorder(const RectangleI& rect);
    void drawPanelScrews(const RectangleI& rect);
    void drawPanelFrame(const RectangleI& rect);

    void drawPanels(const std::vector<RectangleI>& rects);
    void drawPanelsBackgrounds(const std::vector<RectangleI>& rects);
    void drawPanelsGradientBorders(const std::vector<RectangleI>& rects);
    void drawPanelsScrews(const std::vector<RectangleI>& rects);
    void drawPanelsFrames(const std::vector<RectangleI>& rects);

    void drawMainScene();
    void drawMainScenePanels();

    void drawUpgradesScene();

    void drawReactorBars();
    int segmentedBarsWidth(const RectangleI& textureRect, float width) const;
    void drawSegmentedBar(const sf::Texture& texture, const RectangleI& litTexture, const RectangleI& unlitTexture, const Vec2F& center, float width, float litPercentage, const std::string& text); //width may vary because it is rounded to te precision of texture

    void onMouseButtonPressed(const Vec2I& windowCoords);
    void clickSegment(const Segment& segment);

    void onWindowResized(sf::Event& windowResizedEvent);
protected:
    class UIWall
    {
    public:
        struct UIWallIntersection
        {
            Vec2I topLeft;
            int leftExcess; //excess means how much of the wall is hanging off from 24x24 rect stating at topLeft. It may be negative.
            int rightExcess;
            int topExcess;
            int bottomExcess;
        };
        enum class Orientation
        {
            Horizontal,
            Vertical
        };
        UIWall(const Vec2I& topLeft, int size, Orientation orientation) : m_orientation(orientation)
        {
            int width = 24;
            int height = 24;
            if(m_orientation == Orientation::Horizontal) width = size;
            else height = size; //Vertical
            m_wallRect = RectangleI(topLeft, width, height);
        }
        UIWall(const RectangleI& rect) : m_wallRect(rect)
        {
            int width = rect.width();
            if(width == 24) m_orientation = Orientation::Vertical;
            else m_orientation = Orientation::Horizontal;
        }
        void move(const Vec2I& v)
        {
            m_wallRect += v;
        }
        bool intersects(const UIWall& other) const
        {
            if(other.m_orientation == m_orientation) return false;
            return m_wallRect.intersects(other.m_wallRect);
        }
        UIWallIntersection intersection(const UIWall& other) const
        {
            if(other.m_orientation == m_orientation || !intersects(other)) return UIWallIntersection();
            const UIWall& vertical = m_orientation == Orientation::Vertical ? *this : other;
            const UIWall& horizontal = m_orientation == Orientation::Horizontal ? *this : other;
            int xs[4] = {m_wallRect.min.x, m_wallRect.max.x, other.m_wallRect.min.x, other.m_wallRect.max.x};
            int ys[4] = {m_wallRect.min.y, m_wallRect.max.y, other.m_wallRect.min.y, other.m_wallRect.max.y};
            std::sort(std::begin(xs), std::end(xs));
            std::sort(std::begin(ys), std::end(ys));
            Vec2I intersectionTopLeft = {xs[1], ys[1]};
            if(xs[2] - intersectionTopLeft.x != 24) intersectionTopLeft.x = vertical.topLeft().x;
            if(ys[2] - intersectionTopLeft.y != 24) intersectionTopLeft.y = horizontal.topLeft().y;
            int leftExcess = vertical.topLeft().x - horizontal.topLeft().x;
            int rightExcess = horizontal.m_wallRect.max.x - vertical.m_wallRect.max.x;
            int topExcess = horizontal.topLeft().y - vertical.topLeft().y;
            int bottomExcess = vertical.m_wallRect.max.y - horizontal.m_wallRect.max.y;
            return UIWallIntersection {intersectionTopLeft, leftExcess, rightExcess, topExcess, bottomExcess};
        }
        const Vec2I& topLeft() const
        {
            return m_wallRect.min;
        }
        const Orientation orientation() const
        {
            return m_orientation;
        }
        const RectangleI& wallRect() const
        {
            return m_wallRect;
        }
        const int size() const
        {
            if(m_orientation == Orientation::Horizontal) return m_wallRect.width();
            else return m_wallRect.height(); //Vertical
        }
        const int width() const
        {
            return m_wallRect.width();
        }
        const int height() const
        {
            return m_wallRect.height();
        }
    private:
        Orientation m_orientation;
        RectangleI m_wallRect;
    };
    Game& m_game;
    sf::RenderWindow& m_window;
};

#endif // USERINTERFACE_H
