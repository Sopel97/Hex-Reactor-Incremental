#ifndef UPGRADEINTERFACE_H
#define UPGRADEINTERFACE_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../LibS/Geometry.h"

#include <array>

#include "SegmentStructure.h"

class UpgradeBase;

using namespace Geo;

class UpgradeInterface
{
public:
    UpgradeInterface(UpgradeBase& upgrade, const Vec2I& offset);

    void draw(sf::RenderTarget& renderTarget);
    void update(double dt);

    void setWorldOffset(const Vec2I& newOffset);

    void onMouseButtonPressed(const Vec2I& localCoords);
    void clickSegment(const Segment& segment);

    void updateSegments();

    const UpgradeBase& upgrade() const;
    const Vec2I& worldOffset() const;

    static float width();
    static float height();
protected:
    UpgradeBase& m_upgrade;
    Vec2I m_worldOffset;
    static SegmentStructure m_segmentStructure; //the structure is the same, just segment states are updated between every draw. Ie. it's instanced for every interface on the fly
};

#endif // UPGRADEINTERFACE_H
