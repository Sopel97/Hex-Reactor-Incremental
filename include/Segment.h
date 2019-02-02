#ifndef SEGMENT_H
#define SEGMENT_H

#include "../LibS/Geometry.h"

#include <string>

using namespace Geo;

class Segment
{
public:
    Segment() = default;
    Segment(const Segment&) = default;
    Segment(Segment&&) = default;
    Segment(const std::string& name, const Vec2F& spritePosition, const Vec2F& position, const Vec2F& size, const PolygonF& volume);

    Segment& operator = (const Segment&) = default;
    Segment& operator = (Segment &&) = default;

    const std::string& name() const;
    const Vec2F& spritePosition() const;
    const Vec2F& position() const;
    const Vec2F& size() const;
    const PolygonF& volume() const;
    bool isEnabled() const;

    void enable();
    void disable();
    void setEnabled(bool enable);

protected:
    std::string m_name;
    Vec2F m_spritePosition;
    Vec2F m_position;
    Vec2F m_size;
    PolygonF m_volume;
    bool m_isEnabled;
};

#endif // SEGMENT_H
