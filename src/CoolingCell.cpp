#include "CoolingCell.h"
#include "ComponentConditionBar.h"
#include "StringFormatter.h"
#include "TextRenderer.h"

/*
    Common data
*/
CoolingCell::CommonData::CommonData(ConfigurationNode& config) :
    Component::CommonData(config)
{
}

/*
    Cooling Cell
*/

CoolingCell::CoolingCell(std::shared_ptr<CoolingCell::CommonData>& commonData) :
    Component(Component::Type::CoolingCell),
    m_commonData(commonData)
{

}
CoolingCell::CoolingCell(ConfigurationNode& config) :
    Component(Component::Type::CoolingCell),
    m_commonData(std::make_shared<CoolingCell::CommonData>(config))
{
}
CoolingCell::CoolingCell(const CoolingCell& coolingCell) :
    Component(Component::Type::CoolingCell),
    m_commonData(coolingCell.m_commonData)
{

}

std::string CoolingCell::formattedDescription(const std::string& desc) const
{
    return StringFormatter::formatString(desc,
    {
        StringFormatter::Rule{Component::FormatTokens::name, name()},
        StringFormatter::Rule{Component::FormatTokens::price, TextRenderer::formatFancy(price())},
        StringFormatter::Rule{Component::FormatTokens::sellValue, TextRenderer::formatFancy(sellValue())},
        StringFormatter::Rule{Component::FormatTokens::storedHeat, TextRenderer::formatFancy(storedHeat())},
        StringFormatter::Rule{Component::FormatTokens::heatCapacity, TextRenderer::formatFancy(heatCapacity())}
    });
}

bool CoolingCell::isDepleted() const
{
    return false;
}

std::shared_ptr<Component::CommonData> CoolingCell::commonData() const
{
    return m_commonData;
}

float CoolingCell::condition() const
{
    return 1.0f - (float)(m_storedHeat * 100 / heatCapacity()) / 100.0f;
}
float CoolingCell::conditionBarPercent() const
{
    return 1.0f - condition();
}
bool CoolingCell::hasConditionBar() const
{
    return true;
}
ComponentConditionBar CoolingCell::conditionBar() const
{
    return ComponentConditionBar(ComponentConditionBar::Type::HeatBar, conditionBarPercent());
}

std::shared_ptr<Component> CoolingCell::clone() const
{
    return std::make_shared<CoolingCell>(*this);
}
CoolingCell::~CoolingCell()
{
}
