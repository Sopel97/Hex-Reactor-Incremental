#ifndef COMPONENTSTATICDATA_H
#define COMPONENTSTATICDATA_H

#include <string>
#include <memory>

#include "../LibS/Geometry.h"

#include "Bignum.h"

class Component;

using namespace Geo;

class ComponentStaticData //contains identification of a component as well as the component itself
//if needed make a similar class for Cells and other types of components
{
public:
    ComponentStaticData() = default;
    ComponentStaticData(const std::string& name, const std::string& description, const Vec2I& spritePos, const Vec2I& depletedSpritePos, const BigInt& price);
    ComponentStaticData(const ComponentStaticData& staticData) = default;
    ComponentStaticData(ComponentStaticData&& staticData) = default;

    ComponentStaticData& operator=(const ComponentStaticData& staticData) = default;
    ComponentStaticData& operator=(ComponentStaticData && staticData) = default;

    const std::string& name() const;
    const std::string& description() const;
    const Vec2I& spritePos() const;
    const Vec2I& depletedSpritePos() const;
    const BigInt& price() const;
private:
    std::string m_name;
    std::string m_description;
    Vec2I m_spritePos;
    Vec2I m_depletedSpritePos;
    BigInt m_price;
};

#endif // COMPONENTSTATICDATA_H
