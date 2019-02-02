#include "Capacitor.h"
#include "ComponentConditionBar.h"
#include "StringFormatter.h"
#include "TextRenderer.h"

/*
    Common Data
*/

Capacitor::CommonData::CommonData(ConfigurationNode& config) :
    Component::CommonData(config)
{
    m_baseReactorPowerCapacity = stoll(config["baseReactorPowerCapacity"].get<std::string>());
}

const BigInt& Capacitor::CommonData::baseReactorPowerCapacity() const
{
    return m_baseReactorPowerCapacity;
}

/*
    Capacitor
*/
Capacitor::Capacitor(std::shared_ptr<Capacitor::CommonData>& commonData) :
    Component(Component::Type::Capacitor),
    m_commonData(commonData)
{

}
Capacitor::Capacitor(ConfigurationNode& config) :
    Component(Component::Type::Capacitor),
    m_commonData(std::make_shared<Capacitor::CommonData>(config))
{
}
Capacitor::Capacitor(const Capacitor& capacitor) :
    Component(Component::Type::Capacitor),
    m_commonData(capacitor.m_commonData)
{

}
const BigInt& Capacitor::baseReactorPowerCapacity() const
{
    return m_commonData->baseReactorPowerCapacity();
}

std::string Capacitor::formattedDescription(const std::string& desc) const
{
    return StringFormatter::formatString(desc,
    {
        StringFormatter::Rule{Component::FormatTokens::name, name()},
        StringFormatter::Rule{Component::FormatTokens::price, TextRenderer::formatFancy(price())},
        StringFormatter::Rule{Component::FormatTokens::sellValue, TextRenderer::formatFancy(sellValue())},
        StringFormatter::Rule{Component::FormatTokens::storedHeat, TextRenderer::formatFancy(storedHeat())},
        StringFormatter::Rule{Component::FormatTokens::heatCapacity, TextRenderer::formatFancy(heatCapacity())},
        StringFormatter::Rule{Component::FormatTokens::reactorPowerCapacity, TextRenderer::formatFancy(reactorPowerCapacity())}
    });
}
BigInt Capacitor::reactorPowerCapacity() const
{
    return baseReactorPowerCapacity();
}

bool Capacitor::isDepleted() const
{
    return false;
}

std::shared_ptr<Component::CommonData> Capacitor::commonData() const
{
    return m_commonData;
}

float Capacitor::condition() const
{
    return 1.0f - (float)(m_storedHeat * 100 / heatCapacity()) / 100.0f;
}
float Capacitor::conditionBarPercent() const
{
    return 1.0f - condition();
}
bool Capacitor::hasConditionBar() const
{
    return true;
}
ComponentConditionBar Capacitor::conditionBar() const
{
    return ComponentConditionBar(ComponentConditionBar::Type::HeatBar, conditionBarPercent());
}

std::shared_ptr<Component> Capacitor::clone() const
{
    return std::make_shared<Capacitor>(*this);
}
Capacitor::~Capacitor()
{
}
