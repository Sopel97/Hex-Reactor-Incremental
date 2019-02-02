#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../LibS/Geometry.h"

using namespace Geo;

class ScrollBar
{
public:
    enum class Side
    {
        Left,
        Right
    };
    ScrollBar() = delete;
    static void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const RectangleI& rect, float minScroll, float currentScroll, float maxScroll, Side side);
protected:
};

#endif // SCROLLBAR_H
