#include "ScrollBar.h"

void ScrollBar::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const RectangleI& rect, float minScroll, float currentScroll, float maxScroll, Side side)
{
    constexpr int scrollBarBackgroundWidth = 8;
    constexpr int scrollBarBackgroundThickness = 2;
    constexpr float minScrollBarHeight = 8.0f;

    if(maxScroll <= rect.height()) return;

    sf::Vector2f position = (side == ScrollBar::Side::Left ? sf::Vector2f(rect.min.x, rect.min.y) : sf::Vector2f(rect.max.x - scrollBarBackgroundWidth, rect.min.y));
    sf::RectangleShape background;
    background.setFillColor(sf::Color(255,255,255,128));
    background.setPosition(position);
    background.setSize(sf::Vector2f(scrollBarBackgroundWidth, rect.height()));
    renderTarget.draw(background, renderStates);

    sf::Vector2f offset(0.0f, 0.0f);
    float scrollBarHeight = std::max(minScrollBarHeight, ((float)rect.height()-2*scrollBarBackgroundThickness)*((float)rect.height() / ((maxScroll - minScroll))));
    offset.y = ((float)rect.height()-2*scrollBarBackgroundThickness) * (currentScroll / (maxScroll - minScroll));

    sf::RectangleShape scrollBar;
    scrollBar.setFillColor(sf::Color(0,0,0,128));
    scrollBar.setPosition(position+sf::Vector2f(scrollBarBackgroundThickness,scrollBarBackgroundThickness)+offset);
    scrollBar.setSize(sf::Vector2f(scrollBarBackgroundWidth - 2*scrollBarBackgroundThickness, scrollBarHeight));
    renderTarget.draw(scrollBar, renderStates);

}
