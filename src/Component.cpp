#include "Component.h"
#include "ComponentConditionBar.h"
#include "Reactor.h"
#include "BinaryStream.h"

/*
    Common Data
*/

Component::CommonData::CommonData(ConfigurationNode& config)
{
    m_name = config["name"].get<std::string>();
    m_onShopDisplayDescription = config["onShopDisplayDescription"].getDefault<std::string>("");
    m_onGridDescription = config["onGridDescription"].getDefault<std::string>("");
    m_spritePosition = Vec2I{config["sprite"][1].getDefault<int>(0), config["sprite"][2].getDefault<int>(0)};
    m_depletedSpritePosition = Vec2I{config["depletedSprite"][1].getDefault<int>(m_spritePosition.x), config["depletedSprite"][2].getDefault<int>(m_spritePosition.y)};;
    m_basePrice = stoll(config["basePrice"].getDefault<std::string>("0"));
    m_baseHeatCapacity = stoll(config["baseHeatCapacity"].getDefault<std::string>("-1"));
    m_onDepletedDescription = config["onDepletedDescription"].getDefault<std::string>("Depleted");
}

void Component::CommonData::updateBonuses(UpgradePool& upgradePool)
{

}
const std::string& Component::CommonData::name() const
{
    return m_name;
}
const std::string& Component::CommonData::onShopDisplayDescription() const
{
    return m_onShopDisplayDescription;
}
const std::string& Component::CommonData::onGridDescription() const
{
    return m_onGridDescription;
}
const std::string& Component::CommonData::onDepletedDescription() const
{
    return m_onDepletedDescription;
}

const Vec2I& Component::CommonData::spritePosition() const
{
    return m_spritePosition;
}
const Vec2I& Component::CommonData::depletedSpritePosition() const
{
    return m_depletedSpritePosition;
}
const BigInt& Component::CommonData::basePrice() const
{
    return m_basePrice;
}
const BigInt& Component::CommonData::baseHeatCapacity() const
{
    return m_baseHeatCapacity;
}

/*
    Component
*/

Component::Component(Component::Type type) :
    m_type(type),
    m_baseValue(0),
    m_storedHeat(0)
{

}
void Component::generatePowerAndHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood)
{

}
void Component::exchangeHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood)
{

}
void Component::dissipateHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood)
{

}
void Component::tick(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood)
{

}
void Component::resetToBaseState()
{

}
void Component::updateAfterUpgradesChanged()
{

}
const std::string& Component::name() const
{
    return commonData()->name();
}
const std::string& Component::onShopDisplayDescription() const
{
    return commonData()->onShopDisplayDescription();
}
const std::string& Component::onGridDescription() const
{
    if(isDepleted()) return onDepletedDescription();
    return commonData()->onGridDescription();
}
const std::string& Component::onDepletedDescription() const
{
    return commonData()->onDepletedDescription();
}
const Vec2I& Component::spritePosition() const
{
    return commonData()->spritePosition();
}
const Vec2I& Component::depletedSpritePosition() const
{
    return commonData()->depletedSpritePosition();
}
Component::Type Component::type() const
{
    return m_type;
}
const BigInt& Component::basePrice() const
{
    return commonData()->basePrice();
}
const BigInt& Component::baseValue() const
{
    return m_baseValue;
}
BigInt Component::sellValue() const
{
    if(isDepleted()) return 0;
    return m_baseValue * BigInt(condition() * 100.0f) / 100;
}
const BigInt& Component::storedHeat() const
{
    return m_storedHeat;
}
const BigInt& Component::baseHeatCapacity() const
{
    return commonData()->baseHeatCapacity();
}

BigInt Component::price() const
{
    return basePrice();
}
BigInt Component::heatCapacity() const
{
    return baseHeatCapacity();
}
BigInt Component::heatToEmitOnMelting() const
{
    if(heatCapacity() == -1) return 0;
    return std::max(0ll, m_storedHeat - heatCapacity());
}

bool Component::isMelting() const
{
    if(!canStoreHeat()) return false;
    return m_storedHeat > baseHeatCapacity();
}
bool Component::canStoreHeat() const
{
    return heatCapacity() != -1;
}
bool Component::acceptsHeat() const
{
    return canStoreHeat();
}

void Component::setBaseValue(const BigInt& baseValue)
{
    m_baseValue = baseValue;
}
void Component::addHeat(const BigInt& amount)
{
    m_storedHeat += amount;
    if(m_storedHeat < 0) m_storedHeat = 0;
}

std::string Component::formattedOnShopDisplayDescription() const
{
    return formattedDescription(onShopDisplayDescription());
}
std::string Component::formattedOnGridDescription(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const
{
    return formattedDescription(onGridDescription());
}
BigInt Component::reactorPowerCapacity() const
{
    return 0;
}
BigInt Component::reactorHeatCapacity() const
{
    return 0;
}

float Component::powerOutputMultiplier() const
{
    return 1.0f;
}
float Component::additivePowerOutputMultiplier() const
{
    return 0.0f;
}
float Component::heatOutputMultiplier() const
{
    return 1.0f;
}
float Component::additiveHeatOutputMultiplier() const
{
    return 0.0f;
}

float Component::condition() const
{
    return 1.0f;
}
float Component::conditionBarPercent() const
{
    return 1.0f;
}
bool Component::hasConditionBar() const
{
    return false;
}
ComponentConditionBar Component::conditionBar() const
{
    return ComponentConditionBar();
}

bool Component::canReplace(const std::shared_ptr<Component>& old) const
{
    if(old == nullptr) return true;
    return ((old->m_type == m_type) && ((name() != old->name()) || old->isDepleted()));
}
Component::~Component()
{

}

void Component::serialize(BinaryStream& stream) const
{
    stream << m_baseValue;
    stream << m_storedHeat;
}
void Component::deserialize(BinaryStream& stream)
{
    stream >> m_baseValue;
    stream >> m_storedHeat;
}
