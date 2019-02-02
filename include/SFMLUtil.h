#ifndef SFMLUTIL_H
#define SFMLUTIL_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../LibS/Geometry.h"
using namespace Geo;

class SFMLUtil final
{
public:
    SFMLUtil() = delete;
//    static void appendQuadAsTriangleList(sf::VertexArray& vertexArray, const Vec2F& position, const Vec2F& size, const Vec2F& spritePosition, const Vec2F& spriteSize, sf::Color color = sf::Color::White);
    static void appendQuadAsTriangleList(sf::VertexArray& vertexArray, const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& spritePosition, const sf::Vector2f& spriteSize, sf::Color color = sf::Color::White);
protected:
};

#endif // SFMLUTIL_H
