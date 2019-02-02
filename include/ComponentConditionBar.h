#ifndef COMPONENTCONDITIONBAR_H
#define COMPONENTCONDITIONBAR_H

#include <vector>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../LibS/Geometry.h"

using namespace Geo;

class ComponentConditionBar
{
public:
    enum class Type
    {
        HeatBar,
        LifeBar,
        NumberOfTypes
    };
    ComponentConditionBar() = default;
    ComponentConditionBar(Type type, float condition);
    void draw(sf::VertexArray& vertexArray, const Vec2F& componentCenter);

    Type type() const;
    float condition() const;
protected:
    sf::Color lerpColor(const std::vector<sf::Color>& colors, float t) const;

    static const std::vector<sf::Color> m_colorThemes[(unsigned)Type::NumberOfTypes];
    static const float m_bestConditions[(unsigned)Type::NumberOfTypes];
    static constexpr int m_width = 28;
    static constexpr int m_height = 2;
    static constexpr int m_borderThickness = 2;
    static constexpr int m_topOffset = 12; //offset from componentCenter to bar center
    Type m_type;
    float m_condition;
};

#endif // COMPONENTCONDITIONBAR_H
