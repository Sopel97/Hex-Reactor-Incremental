#include "Animation.h"
#include "SFMLUtil.h"

#include <cmath>

Animation::Animation(const sf::Texture& texture, int frames, float timePerFrame, const Vec2F& pos, const Vec2F& firstFrame, const Vec2F& nextFrameOffset, const Vec2F& frameSize) :
    m_texture(&texture),
    m_frames(frames),
    m_timePerFrame(timePerFrame),
    m_timePassed(0.0f),
    m_position(pos),
    m_firstFrame(firstFrame),
    m_nextFrameOffset(nextFrameOffset),
    m_frameSize(frameSize)
{

}
void Animation::update(float dt)
{
    m_timePassed += dt;
}
void Animation::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    Vec2F framePos = framePosition(currentFrame());
    sf::Sprite sprite;
    sprite.setPosition(sf::Vector2f(m_position.x, m_position.y));
    sprite.setTexture(*m_texture);
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(framePos.x, framePos.y), sf::Vector2i(m_frameSize.x, m_frameSize.y)));
    renderTarget.draw(sprite, renderStates);
}
void Animation::appendToVertexArray(sf::VertexArray& vertexArray)
{
    Vec2F framePos = framePosition(currentFrame());

    SFMLUtil::appendQuadAsTriangleList(vertexArray, m_position, m_frameSize, framePos, m_frameSize);
}

Vec2F Animation::framePosition(int i) const
{
    return m_firstFrame + m_nextFrameOffset * currentFrame();
}
const Vec2F& Animation::position() const
{
    return m_position;
}
const Vec2F& Animation::firstFrame() const
{
    return m_firstFrame;
}
const Vec2F& Animation::nextFrameOffset() const
{
    return m_nextFrameOffset;
}
const Vec2F& Animation::frameSize() const
{
    return m_frameSize;
}
float Animation::timePassed() const
{
    return m_timePassed;
}
float Animation::timePerFrame() const
{
    return m_timePerFrame;
}
int Animation::frames() const
{
    return m_frames;
}
int Animation::currentFrame() const
{
    return std::min(m_frames - 1, std::max(0, (int)floor(m_timePassed / m_timePerFrame)));
}
const sf::Texture& Animation::texture() const
{
    return *m_texture;
}

bool Animation::hasEnded() const
{
    return m_timePassed > m_timePerFrame * m_frames;
}
