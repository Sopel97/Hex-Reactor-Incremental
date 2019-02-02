#include "SFMLUtil.h"
/*
void SFMLUtil::appendQuadAsTriangleList(sf::VertexArray& vertexArray, const Vec2F& position, const Vec2F& size, const Vec2F& spritePosition, const Vec2F& spriteSize, sf::Color color)
{
    const sf::Vector2f p(position.x, position.y);
    const sf::Vector2f s(size.x, size.y);
    const sf::Vector2f sp(spritePosition.x, spritePosition.y);
    const sf::Vector2f ss(spriteSize.x, spriteSize.y);

    vertexArray.append(sf::Vertex(p, color, sp));
    vertexArray.append(sf::Vertex(p + sf::Vector2f(0.0f, s.y), color, sp + sf::Vector2f(0.0f, ss.y)));
    vertexArray.append(sf::Vertex(p + sf::Vector2f(s.x, s.y), color, sp + sf::Vector2f(ss.x, ss.y)));

    vertexArray.append(sf::Vertex(p, color, sp));
    vertexArray.append(sf::Vertex(p + sf::Vector2f(s.x, s.y), color, sp + sf::Vector2f(ss.x, ss.y)));
    vertexArray.append(sf::Vertex(p + sf::Vector2f(s.x, 0.0f), color, sp + sf::Vector2f(ss.x, 0.0f)));
}*/

void SFMLUtil::appendQuadAsTriangleList(sf::VertexArray& vertexArray, const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& spritePosition, const sf::Vector2f& spriteSize, sf::Color color)
{
    vertexArray.append(sf::Vertex(position, color, spritePosition));
    vertexArray.append(sf::Vertex(position + sf::Vector2f(0.0f, size.y), color, spritePosition + sf::Vector2f(0.0f, spriteSize.y)));
    vertexArray.append(sf::Vertex(position + sf::Vector2f(size.x, size.y), color, spritePosition + sf::Vector2f(spriteSize.x, spriteSize.y)));

    vertexArray.append(sf::Vertex(position, color, spritePosition));
    vertexArray.append(sf::Vertex(position + sf::Vector2f(size.x, size.y), color, spritePosition + sf::Vector2f(spriteSize.x, spriteSize.y)));
    vertexArray.append(sf::Vertex(position + sf::Vector2f(size.x, 0.0f), color, spritePosition + sf::Vector2f(spriteSize.x, 0.0f)));
}
