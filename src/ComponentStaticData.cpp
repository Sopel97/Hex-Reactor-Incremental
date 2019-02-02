#include "ComponentStaticData.h"

ComponentStaticData::ComponentStaticData(const std::string& name, const std::string& description, const Vec2I& spritePos, const Vec2I& depletedSpritePos, const BigInt& price) :
    m_name(name),
    m_description(description),
    m_spritePos(spritePos),
    m_depletedSpritePos(depletedSpritePos),
    m_price(price)
{

}
const std::string& ComponentStaticData::name() const
{
    return m_name;
}
const std::string& ComponentStaticData::description() const
{
    return m_description;
}

const Vec2I& ComponentStaticData::spritePos() const
{
    return m_spritePos;
}
const Vec2I& ComponentStaticData::depletedSpritePos() const
{
    return m_depletedSpritePos;
}
const BigInt& ComponentStaticData::price() const
{
    return m_price;
}
