#include "HeatOutlet.h"
#include "Reactor.h"
#include "StringFormatter.h"
#include "TextRenderer.h"

/*
    Heat Outlet
*/

HeatOutlet::CommonData::CommonData(ConfigurationNode& config) :
    Component::CommonData(config)
{
    m_baseHeatTransferRate = std::stoll(config["baseHeatTransferRate"].get<std::string>());
}

const BigInt& HeatOutlet::CommonData::baseHeatTransferRate() const
{
    return m_baseHeatTransferRate;
}

/*
    Heat Outlet
*/

HeatOutlet::HeatOutlet(std::shared_ptr<HeatOutlet::CommonData>& commonData) :
    Component(Component::Type::HeatOutlet),
    m_commonData(commonData)
{

}
HeatOutlet::HeatOutlet(ConfigurationNode& config) :
    Component(Component::Type::HeatOutlet),
    m_commonData(std::make_shared<HeatOutlet::CommonData>(config))
{
}
HeatOutlet::HeatOutlet(const HeatOutlet& heatOutlet) :
    Component(Component::Type::HeatOutlet),
    m_commonData(heatOutlet.m_commonData)
{

}

void HeatOutlet::exchangeHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood) //need to be done more precisely
{
    BigInt amountToTransfer = std::min(reactor.storedHeat(), baseHeatTransferRate());
    reactor.addHeat(-amountToTransfer);
    reactor.reactorGrid().emitHeat(amountToTransfer, neighbourhood);
}

const BigInt& HeatOutlet::baseHeatTransferRate() const
{
    return m_commonData->baseHeatTransferRate();
}

std::string HeatOutlet::formattedDescription(const std::string& desc) const
{
    return StringFormatter::formatString(desc,
    {
        StringFormatter::Rule{Component::FormatTokens::name, name()},
        StringFormatter::Rule{Component::FormatTokens::price, TextRenderer::formatFancy(price())},
        StringFormatter::Rule{Component::FormatTokens::sellValue, TextRenderer::formatFancy(sellValue())},
        StringFormatter::Rule{Component::FormatTokens::heatTransferRate, TextRenderer::formatFancy(heatTransferRate())}
    });
}
BigInt HeatOutlet::heatTransferRate() const
{
    return baseHeatTransferRate();
}

bool HeatOutlet::isDepleted() const
{
    return false;
}

std::shared_ptr<Component::CommonData> HeatOutlet::commonData() const
{
    return m_commonData;
}

std::shared_ptr<Component> HeatOutlet::clone() const
{
    return std::make_shared<HeatOutlet>(*this);
}
HeatOutlet::~HeatOutlet()
{

}
