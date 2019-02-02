#include "HeatExchanger.h"
#include "ComponentConditionBar.h"
#include "StringFormatter.h"
#include "TextRenderer.h"

/*
    Common data
*/

HeatExchanger::CommonData::CommonData(ConfigurationNode& config) :
    Component::CommonData(config)
{
    m_baseHeatTransferRate = stoll(config["baseHeatTransferRate"].get<std::string>());
}
const BigInt& HeatExchanger::CommonData::baseHeatTransferRate() const
{
    return m_baseHeatTransferRate;
}

/*
    Heat Exchanger
*/

HeatExchanger::HeatExchanger(std::shared_ptr<HeatExchanger::CommonData>& commonData) :
    Component(Component::Type::HeatExchanger),
    m_commonData(commonData)
{

}
HeatExchanger::HeatExchanger(ConfigurationNode& config) :
    Component(Component::Type::HeatExchanger),
    m_commonData(std::make_shared<HeatExchanger::CommonData>(config))
{
}
HeatExchanger::HeatExchanger(const HeatExchanger& heatExchanger) :
    Component(Component::Type::HeatExchanger),
    m_commonData(heatExchanger.m_commonData)
{

}

const BigInt& HeatExchanger::baseHeatTransferRate() const
{
    return m_commonData->baseHeatTransferRate();
}

std::string HeatExchanger::formattedDescription(const std::string& desc) const
{
    return StringFormatter::formatString(desc,
    {
        StringFormatter::Rule{Component::FormatTokens::name, name()},
        StringFormatter::Rule{Component::FormatTokens::price, TextRenderer::formatFancy(price())},
        StringFormatter::Rule{Component::FormatTokens::sellValue, TextRenderer::formatFancy(sellValue())},
        StringFormatter::Rule{Component::FormatTokens::storedHeat, TextRenderer::formatFancy(storedHeat())},
        StringFormatter::Rule{Component::FormatTokens::heatCapacity, TextRenderer::formatFancy(heatCapacity())},
        StringFormatter::Rule{Component::FormatTokens::heatTransferRate, TextRenderer::formatFancy(heatTransferRate())}
    });
}
BigInt HeatExchanger::heatTransferRate() const
{
    return baseHeatTransferRate();
}

bool HeatExchanger::isDepleted() const
{
    return false;
}

std::shared_ptr<Component::CommonData> HeatExchanger::commonData() const
{
    return m_commonData;
}

float HeatExchanger::condition() const
{
    return 1.0f - (float)(m_storedHeat * 100 / heatCapacity()) / 100.0f;
}
float HeatExchanger::conditionBarPercent() const
{
    return 1.0f - condition();
}
bool HeatExchanger::hasConditionBar() const
{
    return true;
}
ComponentConditionBar HeatExchanger::conditionBar() const
{
    return ComponentConditionBar(ComponentConditionBar::Type::HeatBar, conditionBarPercent());
}

std::shared_ptr<Component> HeatExchanger::clone() const
{
    return std::make_shared<HeatExchanger>(*this);
}
HeatExchanger::~HeatExchanger()
{

}
