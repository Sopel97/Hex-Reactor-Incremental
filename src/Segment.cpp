#include "Segment.h"

Segment::Segment(const std::string& name, const Vec2F& spritePosition, const Vec2F& position, const Vec2F& size, const PolygonF& volume) :
    m_name(name),
    m_spritePosition(spritePosition),
    m_position(position),
    m_size(size),
    m_volume(volume),
    m_isEnabled(true)
{

}

const std::string& Segment::name() const
{
    return m_name;
}
const Vec2F& Segment::spritePosition() const
{
    return m_spritePosition;
}
const Vec2F& Segment::position() const
{
    return m_position;
}
const Vec2F& Segment::size() const
{
    return m_size;
}
const PolygonF& Segment::volume() const
{
    return m_volume;
}
bool Segment::isEnabled() const
{
    return m_isEnabled;
}
void Segment::enable()
{
    m_isEnabled = true;
}
void Segment::disable()
{
    m_isEnabled = false;
}
void Segment::setEnabled(bool enable)
{
    m_isEnabled = enable;
}
