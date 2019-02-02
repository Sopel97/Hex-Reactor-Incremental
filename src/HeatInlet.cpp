#include "HeatInlet.h"
#include "Reactor.h"
#include "StringFormatter.h"
#include "TextRenderer.h"

/*
    Common data
*/

HeatInlet::CommonData::CommonData(ConfigurationNode& config) :
    Component::CommonData(config)
{
    m_baseHeatTransferRate = std::stoll(config["baseHeatTransferRate"].get<std::string>());
}
const BigInt& HeatInlet::CommonData::baseHeatTransferRate() const
{
    return m_baseHeatTransferRate;
}

/*
    Heat Inlet
*/

HeatInlet::HeatInlet(std::shared_ptr<HeatInlet::CommonData>& commonData) :
    Component(Component::Type::HeatInlet),
    m_commonData(commonData)
{
}
HeatInlet::HeatInlet(ConfigurationNode& config) :
    Component(Component::Type::HeatInlet),
    m_commonData(std::make_shared<HeatInlet::CommonData>(config))
{
}
HeatInlet::HeatInlet(const HeatInlet& heatInlet) :
    Component(Component::Type::HeatInlet),
    m_commonData(heatInlet.m_commonData)
{

}

void HeatInlet::exchangeHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood) //need to be done more precisely
{
    BigInt heatInNeighbourhood = 0;
    for(const auto& component : neighbourhood.neighbours())
    {
        if(component->canStoreHeat())
        {
            heatInNeighbourhood += component->storedHeat();
        }
    }
    if(heatInNeighbourhood <= 0) return;
    BigInt amountToTransfer = std::min(heatInNeighbourhood, baseHeatTransferRate());
    float percent = (float)amountToTransfer / heatInNeighbourhood;
    for(const auto& component : neighbourhood.neighbours())
    {
        if(component->canStoreHeat())
        {
            BigInt toTransfer = component->storedHeat() * percent;
            component->addHeat(-toTransfer);
            reactor.addHeat(toTransfer);
        }
    }
}

const BigInt& HeatInlet::baseHeatTransferRate() const
{
    return m_commonData->baseHeatTransferRate();
}

std::string HeatInlet::formattedDescription(const std::string& desc) const
{
    return StringFormatter::formatString(desc,
    {
        StringFormatter::Rule{Component::FormatTokens::name, name()},
        StringFormatter::Rule{Component::FormatTokens::price, TextRenderer::formatFancy(price())},
        StringFormatter::Rule{Component::FormatTokens::sellValue, TextRenderer::formatFancy(sellValue())},
        StringFormatter::Rule{Component::FormatTokens::heatTransferRate, TextRenderer::formatFancy(heatTransferRate())}
    });
}
BigInt HeatInlet::heatTransferRate() const
{
    return baseHeatTransferRate();
}

bool HeatInlet::isDepleted() const
{
    return false;
}

std::shared_ptr<Component::CommonData> HeatInlet::commonData() const
{
    return m_commonData;
}

std::shared_ptr<Component> HeatInlet::clone() const
{
    return std::make_shared<HeatInlet>(*this);
}
HeatInlet::~HeatInlet()
{

}
