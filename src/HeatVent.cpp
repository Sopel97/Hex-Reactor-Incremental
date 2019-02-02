#include "HeatVent.h"
#include "ComponentConditionBar.h"
#include "StringFormatter.h"
#include "TextRenderer.h"

/*
    Common data
*/

HeatVent::CommonData::CommonData(ConfigurationNode& config) :
    Component::CommonData(config)
{
    m_baseHeatDissipationRate = std::stoll(config["baseHeatDissipationRate"].get<std::string>());
}
const BigInt& HeatVent::CommonData::baseHeatDissipationRate() const
{
    return m_baseHeatDissipationRate;
}

/*
    Heat Vent
*/

HeatVent::HeatVent(std::shared_ptr<HeatVent::CommonData>& commonData) :
    Component(Component::Type::HeatVent),
    m_commonData(commonData)
{

}

HeatVent::HeatVent(ConfigurationNode& config) :
    Component(Component::Type::HeatVent),
    m_commonData(std::make_shared<HeatVent::CommonData>(config))
{
}
HeatVent::HeatVent(const HeatVent& heatVent) :
    Component(Component::Type::HeatVent),
    m_commonData(heatVent.m_commonData)
{

}
void HeatVent::dissipateHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood)
{
    addHeat(-heatDissipationRate());
}

const BigInt& HeatVent::baseHeatDissipationRate() const
{
    return m_commonData->baseHeatDissipationRate();
}

std::string HeatVent::formattedDescription(const std::string& desc) const
{
    return StringFormatter::formatString(desc,
    {
        StringFormatter::Rule{Component::FormatTokens::name, name()},
        StringFormatter::Rule{Component::FormatTokens::price, TextRenderer::formatFancy(price())},
        StringFormatter::Rule{Component::FormatTokens::sellValue, TextRenderer::formatFancy(sellValue())},
        StringFormatter::Rule{Component::FormatTokens::storedHeat, TextRenderer::formatFancy(storedHeat())},
        StringFormatter::Rule{Component::FormatTokens::heatCapacity, TextRenderer::formatFancy(heatCapacity())},
        StringFormatter::Rule{Component::FormatTokens::heatDissipationRate, TextRenderer::formatFancy(heatDissipationRate())}
    });
}
BigInt HeatVent::heatDissipationRate() const
{
    return baseHeatDissipationRate();
}

bool HeatVent::isDepleted() const
{
    return false;
}

std::shared_ptr<Component::CommonData> HeatVent::commonData() const
{
    return m_commonData;
}

float HeatVent::condition() const
{
    return 1.0f - (float)(m_storedHeat * 100 / heatCapacity()) / 100.0f;
}
float HeatVent::conditionBarPercent() const
{
    return 1.0f - condition();
}
bool HeatVent::hasConditionBar() const
{
    return true;
}
ComponentConditionBar HeatVent::conditionBar() const
{
    return ComponentConditionBar(ComponentConditionBar::Type::HeatBar, conditionBarPercent());
}

std::shared_ptr<Component> HeatVent::clone() const
{
    return std::make_shared<HeatVent>(*this);
}
HeatVent::~HeatVent()
{
}
