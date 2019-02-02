#ifndef SEGMENTSTRUCTURE_H
#define SEGMENTSTRUCTURE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <string>
#include <map>

#include "Segment.h"

class SegmentStructure
{
    public:
        SegmentStructure() = default;
        SegmentStructure(const std::vector<Segment>& segments);
        SegmentStructure(const std::vector<Segment>& segments, const Vec2F& center); //with forced center (center will still change when adding segments)

        void addSegment(const Segment& segment);

        const Segment* pointedSegment(const Vec2F& localCoords) const;

        const Segment& segment(const std::string& name) const;
        Segment& segment(const std::string& name);

        void enableAllSegments();
        void disableAllSegments();
        void setAllSegmentsEnabled(bool enable);

        void draw(sf::VertexArray& vertexArray, Vec2F offset = Vec2F{0.0f, 0.0f}) const; //offset is to allow instancing

        const RectangleF& boundingRect() const;

        float width() const;
        float height() const;
        Vec2F size() const;

        void setCenter(const Vec2F& newCenter);
        const Vec2F& center() const;

        Vec2F offsetToBeCentered(const RectangleF& rect);
    protected:
        void recalculateBoundingRect();
        std::map<std::string, Segment> m_segments;
        RectangleF m_boundingRect;
        Vec2F m_center;
};

#endif // SEGMENTSTRUCTURE_H
