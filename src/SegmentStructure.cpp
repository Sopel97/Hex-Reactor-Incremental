#include "SegmentStructure.h"
#include "SFMLUtil.h"

SegmentStructure::SegmentStructure(const std::vector<Segment>& segments)
{
    for(const auto& segment : segments)
    {
        addSegment(segment);
    }
    m_center = Vec2F{width()/2.0f, height()/2.0f};
}
SegmentStructure::SegmentStructure(const std::vector<Segment>& segments, const Vec2F& center)
{
    for(const auto& segment : segments)
    {
        addSegment(segment);
    }
    m_center = center;
}
void SegmentStructure::addSegment(const Segment& segment)
{
    m_segments.insert(std::make_pair(segment.name(), segment));
    recalculateBoundingRect();
}

const Segment* SegmentStructure::pointedSegment(const Vec2F& localCoords) const
{
    for(const auto& segmentPair : m_segments)
    {
        const Segment& segment = segmentPair.second;
        if(!segment.isEnabled()) continue;
        const auto& segmentVolume = segment.volume();
        if(segmentVolume.intersects(localCoords - segment.position()))
        {
            return &segment;
        }
    }
    return nullptr;
}

const RectangleF& SegmentStructure::boundingRect() const
{
    return m_boundingRect;
}
float SegmentStructure::width() const
{
    return m_boundingRect.width();
}
float SegmentStructure::height() const
{
    return m_boundingRect.height();
}
Vec2F SegmentStructure::size() const
{
    return Vec2F {width(), height()};
}

void SegmentStructure::recalculateBoundingRect()
{
    float minX = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();

    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();

    for(const auto& segment : m_segments)
    {
        Vec2F min = segment.second.position();
        Vec2F max = min + segment.second.size();

        minX = std::min(min.x, minX);
        minY = std::min(min.y, minY);

        maxX = std::max(max.x, maxX);
        maxY = std::max(max.y, maxY);
    }
    m_boundingRect = RectangleF(Vec2F {minX, minY}, Vec2F {maxX, maxY});
}
const Segment& SegmentStructure::segment(const std::string& name) const
{
    return m_segments.at(name);
}
Segment& SegmentStructure::segment(const std::string& name)
{
    return m_segments.at(name);
}
void SegmentStructure::enableAllSegments()
{
    setAllSegmentsEnabled(true);
}
void SegmentStructure::disableAllSegments()
{
    setAllSegmentsEnabled(false);
}
void SegmentStructure::setAllSegmentsEnabled(bool enable)
{
    for(auto& segmentPair : m_segments)
    {
        Segment& segment = segmentPair.second;
        segment.setEnabled(enable);
    }
}
void SegmentStructure::draw(sf::VertexArray& vertexArray, Vec2F offset) const
{
    for(const auto& segmentPair : m_segments)
    {
        const Segment& segment = segmentPair.second;
        if(!segment.isEnabled()) continue;
        Vec2F position = segment.position() + offset;
        Vec2F size = segment.size();
        Vec2F spritePosition = segment.spritePosition();

        SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, spritePosition, size);
    }
}
void SegmentStructure::setCenter(const Vec2F& newCenter)
{
    m_center = newCenter;
}
const Vec2F& SegmentStructure::center() const
{
    return m_center;
}

Vec2F SegmentStructure::offsetToBeCentered(const RectangleF& rect)
{
    return rect.min + Vec2F{rect.width(), rect.height()}/2.0f - m_center;
}
