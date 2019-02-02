#include "ComponentConditionBar.h"
#include "SFMLUtil.h"

const std::vector<sf::Color> ComponentConditionBar::m_colorThemes[(unsigned)ComponentConditionBar::Type::NumberOfTypes]
{
    {sf::Color::Green, sf::Color::Yellow, sf::Color::Red},
    {sf::Color::Red, sf::Color::Yellow, sf::Color::Green}
};

const float ComponentConditionBar::m_bestConditions[(unsigned)Type::NumberOfTypes]
{
    0.0f,
    1.0f
};

ComponentConditionBar::ComponentConditionBar(Type type, float condition) :
    m_type(type),
    m_condition(std::max(std::min(condition, 1.0f), 0.0f))
{

}
void ComponentConditionBar::draw(sf::VertexArray& vertexArray, const Vec2F& componentCenter) //will be appended to vertexArray
{
    if(m_condition == m_bestConditions[(unsigned)m_type]) return; //this should be ==

    Vec2F position(componentCenter.x - (m_width + m_borderThickness * 2.0f) / 2.0f, componentCenter.y + m_topOffset - (m_height + m_borderThickness * 2.0f) / 2.0f);
    Vec2F size(m_width + m_borderThickness * 2.0f, m_height + m_borderThickness * 2.0f);
    sf::Color color(0, 0, 0, 240);

    SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, Vec2F {0.0f, 0.0f}, size, color);

    position += Vec2F(m_borderThickness, m_borderThickness);
    color = lerpColor(m_colorThemes[(unsigned)m_type], m_condition);
    size -= Vec2F(m_borderThickness * 2, m_borderThickness * 2);
    size.x *= m_condition;

    SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, Vec2F {0.0f, 0.0f}, size, color);
}

ComponentConditionBar::Type ComponentConditionBar::type() const
{
    return m_type;
}
float ComponentConditionBar::condition() const
{
    return m_condition;
}
sf::Color ComponentConditionBar::lerpColor(const std::vector<sf::Color>& colors, float t) const
{
    int numberOfColors = colors.size();
    int numberOfColorRanges = numberOfColors - 1;

    if(t >= 1.0f) return colors.back();
    if(t <= 0.0f) return colors.front();

    int firstColorIndex = numberOfColorRanges * t;
    int secondColorIndex = firstColorIndex + 1;
    float oneColorLength = (1.0f / numberOfColorRanges);
    float t1 = (t - firstColorIndex * oneColorLength) / oneColorLength;

    const sf::Color& firstColor = colors[firstColorIndex];
    const sf::Color& secondColor = colors[secondColorIndex];

    int r0 = firstColor.r;
    int g0 = firstColor.g;
    int b0 = firstColor.b;
    int r1 = secondColor.r;
    int g1 = secondColor.g;
    int b1 = secondColor.b;

    return sf::Color(r0 + (r1 - r0) * t1, g0 + (g1 - g0) * t1, b0 + (b1 - b0) * t1);
}
