#include "ReactorPlating.h"
#include "StringFormatter.h"
#include "TextRenderer.h"

/*
    Common data
*/

ReactorPlating::CommonData::CommonData(ConfigurationNode& config) :
    Component::CommonData(config)
{
    m_baseReactorHeatCapacity = std::stoll(config["baseReactorHeatCapacity"].get<std::string>());
}
const BigInt& ReactorPlating::CommonData::baseReactorHeatCapacity() const
{
    return m_baseReactorHeatCapacity;
}

/*
    Reactor Plating
*/

ReactorPlating::ReactorPlating(std::shared_ptr<ReactorPlating::CommonData>& commonData) :
    Component(Component::Type::ReactorPlating),
    m_commonData(commonData)
{

}
ReactorPlating::ReactorPlating(ConfigurationNode& config) :
    Component(Component::Type::ReactorPlating),
    m_commonData(std::make_shared<ReactorPlating::CommonData>(config))
{
}
ReactorPlating::ReactorPlating(const ReactorPlating& reactorPlating) :
    Component(Component::Type::ReactorPlating),
    m_commonData(reactorPlating.m_commonData)
{

}

BigInt ReactorPlating::baseReactorHeatCapacity() const
{
    return m_commonData->baseReactorHeatCapacity();
}

std::string ReactorPlating::formattedDescription(const std::string& desc) const
{
    return StringFormatter::formatString(desc,
    {
        StringFormatter::Rule{Component::FormatTokens::name, name()},
        StringFormatter::Rule{Component::FormatTokens::price, TextRenderer::formatFancy(price())},
        StringFormatter::Rule{Component::FormatTokens::sellValue, TextRenderer::formatFancy(sellValue())},
        StringFormatter::Rule{Component::FormatTokens::reactorHeatCapacity, TextRenderer::formatFancy(reactorHeatCapacity())}
    });
}
BigInt ReactorPlating::reactorHeatCapacity() const
{
    return baseReactorHeatCapacity();
}

bool ReactorPlating::isDepleted() const
{
    return false;
}

std::shared_ptr<Component::CommonData> ReactorPlating::commonData() const
{
    return m_commonData;
}

std::shared_ptr<Component> ReactorPlating::clone() const
{
    return std::make_shared<ReactorPlating>(*this);
}
ReactorPlating::~ReactorPlating()
{

}
