#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../LibS/Geometry.h"

using namespace Geo;

class Animation
{
public:
    Animation(const sf::Texture& texture, int frames, float timePerFrame, const Vec2F& pos, const Vec2F& firstFrame, const Vec2F& nextFrameOffset, const Vec2F& frameSize);
    Animation(const Animation& anim) = default;
    Animation(Animation&& anim) = default;
    Animation& operator=(const Animation& anim) = default;
    Animation& operator=(Animation && anim) = default;
    void update(float dt);
    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void appendToVertexArray(sf::VertexArray& vertexArray);

    const Vec2F& position() const;
    Vec2F framePosition(int i) const;
    const Vec2F& firstFrame() const;
    const Vec2F& nextFrameOffset() const;
    const Vec2F& frameSize() const;
    float timePassed() const;
    float timePerFrame() const;
    int frames() const;
    int currentFrame() const;
    const sf::Texture& texture() const;

    bool hasEnded() const;
protected:
    const sf::Texture* m_texture;
    int m_frames;
    float m_timePerFrame;
    float m_timePassed;
    Vec2F m_position;
    Vec2F m_firstFrame;
    Vec2F m_nextFrameOffset;
    Vec2F m_frameSize;
};

#endif // ANIMATION_H
