#include "Cell.h"
#include "ReactorGrid.h"
#include "ComponentConditionBar.h"
#include "Reactor.h"
#include "StringFormatter.h"
#include "TextRenderer.h"
#include "BinaryStream.h"

/*
    Common Data
*/

Cell::CommonData::CommonData(ConfigurationNode& config) :
    Component::CommonData(config)
{
    static std::map<std::string, Cell::Variant> cellVariantNames =
    {
        {"Single", Cell::Variant::Single},
        {"Double", Cell::Variant::Double},
        {"Quad", Cell::Variant::Quad},
        {"IsolatedSingle", Cell::Variant::IsolatedSingle},
        {"IsolatedDouble", Cell::Variant::IsolatedDouble},
        {"IsolatedQuad", Cell::Variant::IsolatedQuad}
    };
    m_variant = cellVariantNames[config["cellVariant"].get<std::string>()];
    m_baseLifespan = config["baseLifespan"].get<int>();
    m_basePowerOutput = std::stoll(config["basePowerOutput"].get<std::string>()); //stoll will be replaced when actual bigint will be used
    m_baseHeatOutput = std::stoll(config["baseHeatOutput"].get<std::string>());
}
void Cell::CommonData::updateBonuses(UpgradePool& upgradePool)
{
    m_activeBonuses.lifespanMultiplier = upgradePool.cellLifespanMultiplier();
}
Cell::CommonData::Bonuses& Cell::CommonData::activeBonuses()
{
    return m_activeBonuses;
}
const BigInt& Cell::CommonData::basePowerOutput() const
{
    return m_basePowerOutput;
}
const BigInt& Cell::CommonData::baseHeatOutput() const
{
    return m_baseHeatOutput;
}
Cell::Variant Cell::CommonData::variant() const
{
    return m_variant;
}
BigInt Cell::CommonData::baseLifespan() const
{
    return m_baseLifespan;
}

/*
    Cell
*/

Cell::Cell(std::shared_ptr<Cell::CommonData>& commonData) :
    Component(Component::Type::Cell),
    m_commonData(commonData)
{
    m_lifeLeft = lifespan();
}
Cell::Cell(ConfigurationNode& config) :
    Component(Component::Type::Cell),
    m_commonData(std::make_shared<Cell::CommonData>(config))
{
    m_lifeLeft = lifespan();
}
Cell::Cell(const Cell& cell) :
    Component(Component::Type::Cell),
    m_commonData(cell.m_commonData),
    m_lifeLeft(cell.m_lifeLeft)
{

}
void Cell::generatePowerAndHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood)
{
    if(isDepleted()) return;
    reactor.addPower(powerOutput(neighbourhood));
    reactor.reactorGrid().emitHeat(heatOutput(neighbourhood), neighbourhood);
}
void Cell::tick(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood)
{
    --m_lifeLeft;
}

void Cell::resetToBaseState()
{
    m_lifeLeft = lifespan();
}
void Cell::updateAfterUpgradesChanged()
{
    m_lifeLeft = std::min(m_lifeLeft, lifespan());
}

const BigInt& Cell::basePowerOutput() const
{
    return m_commonData->basePowerOutput();
}
const BigInt& Cell::baseHeatOutput() const
{
    return m_commonData->baseHeatOutput();
}
Cell::Variant Cell::variant() const
{
    return m_commonData->variant();
}
BigInt Cell::baseLifespan() const
{
    return m_commonData->baseLifespan();
}

std::string Cell::formattedDescription(const std::string& desc) const
{
    return StringFormatter::formatString(desc,
    {
        StringFormatter::Rule{Component::FormatTokens::name, name()},
        StringFormatter::Rule{Component::FormatTokens::price, TextRenderer::formatFancy(price())},
        StringFormatter::Rule{Component::FormatTokens::sellValue, TextRenderer::formatFancy(sellValue())},
        StringFormatter::Rule{Component::FormatTokens::ownPowerOutput, TextRenderer::formatFancy(ownPowerOutput())},
        StringFormatter::Rule{Component::FormatTokens::ownHeatOutput, TextRenderer::formatFancy(ownHeatOutput())},
        StringFormatter::Rule{Component::FormatTokens::lifespan, TextRenderer::formatFancy(lifespan())},
        StringFormatter::Rule{Component::FormatTokens::lifeLeft, TextRenderer::formatFancy(lifeLeft())},
        StringFormatter::Rule{Component::FormatTokens::lifePassed, TextRenderer::formatFancy(sellValue())}
    });
}
std::string Cell::formattedOnGridDescription(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const
{
    return StringFormatter::formatString(formattedDescription(onGridDescription()),
    {
        StringFormatter::Rule{Component::FormatTokens::powerOutput, TextRenderer::formatFancy(powerOutput(neighbourhood))},
        StringFormatter::Rule{Component::FormatTokens::heatOutput, TextRenderer::formatFancy(heatOutput(neighbourhood))}
    });
}
BigInt Cell::lifespan() const
{
    return baseLifespan() * m_commonData->activeBonuses().lifespanMultiplier;
}
BigInt Cell::lifePassed() const
{
    return lifespan() - m_lifeLeft;
}
BigInt Cell::lifeLeft() const
{
    return m_lifeLeft;
}

bool Cell::isDepleted() const
{
    return m_lifeLeft <= 0;
}

float Cell::powerMultiplier(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const //first additive, then multiplicative
{
    float multiplier = 0.0f;
    for(auto& component : neighbourhood.neighbours())
    {
        multiplier += component->additivePowerOutputMultiplier();
    }
    multiplier *= effectiveAdditivePowerOutputMultiplier();
    multiplier += 1.0f;
    for(auto& component : neighbourhood.neighbours())
    {
        multiplier *= component->powerOutputMultiplier();
    }

    return multiplier;
}
float Cell::heatMultiplier(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const //first additive, then multiplicative
{
    float multiplier = 0.0f;
    for(auto& component : neighbourhood.neighbours())
    {
        multiplier += component->additiveHeatOutputMultiplier();
    }
    multiplier *= effectiveAdditiveHeatOutputMultiplier();
    multiplier += 1.0f;
    multiplier *= multiplier; //only additive bonus is squared

    for(auto& component : neighbourhood.neighbours())
    {
        multiplier *= component->heatOutputMultiplier();
    }

    return multiplier;
}

BigInt Cell::powerOutput(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const
{
    return powerMultiplier(neighbourhood) * basePowerOutput(); //this will problematic with power as bignum
}
BigInt Cell::heatOutput(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const
{
    return heatMultiplier(neighbourhood) * baseHeatOutput(); //this will problematic with power as bignum
}
BigInt Cell::ownPowerOutput() const //does not count any multipliers from components on grid
{
    return basePowerOutput();
}
BigInt Cell::ownHeatOutput() const
{
    return baseHeatOutput();
}

float Cell::powerOutputMultiplier() const
{
    return 1.0f;
}
float Cell::additivePowerOutputMultiplier() const
{
    float ret = 0.0f;
    if(isDepleted()) return ret;
    switch(variant())
    {
        case Variant::Single: ret = 1.0f; break;
        case Variant::Double: ret = 2.0f; break;
        case Variant::Quad: ret = 4.0f; break;
        case Variant::IsolatedSingle: ret = 1.0f; break;
        case Variant::IsolatedDouble: ret = 1.0f; break;
        case Variant::IsolatedQuad: ret = 1.0f; break;
        default: ret = 0.0f; break;
    }
    return ret;
}
float Cell::heatOutputMultiplier() const
{
    return 1.0f;
}
float Cell::additiveHeatOutputMultiplier() const
{
    return additivePowerOutputMultiplier();
}
float Cell::effectiveAdditivePowerOutputMultiplier() const
{
    float ret = 1.0f;
    if(isDepleted()) return ret;
    switch(variant())
    {
        case Variant::Single: ret = 1.0f; break;
        case Variant::Double: ret = 0.5f; break;
        case Variant::Quad: ret = 0.25f; break;
        case Variant::IsolatedSingle: ret = 0.0f; break;
        case Variant::IsolatedDouble: ret = 0.0f; break;
        case Variant::IsolatedQuad: ret = 0.0f; break;
        default: ret = 1.0f; break;
    }
    return ret;
}
float Cell::effectiveAdditiveHeatOutputMultiplier() const
{
    return effectiveAdditivePowerOutputMultiplier();
}

std::shared_ptr<Component::CommonData> Cell::commonData() const
{
    return m_commonData;
}

float Cell::condition() const
{
    return 1.0f - (float)(lifePassed() * 100 / lifespan()) / 100.0f;
}
float Cell::conditionBarPercent() const
{
    return condition();
}
bool Cell::hasConditionBar() const
{
    if(isDepleted()) return false;
    return true;
}
ComponentConditionBar Cell::conditionBar() const
{
    return ComponentConditionBar(ComponentConditionBar::Type::LifeBar, conditionBarPercent());
}

std::shared_ptr<Component> Cell::clone() const
{
    return std::make_shared<Cell>(*this);
}
Cell::~Cell()
{
}
void Cell::serialize(BinaryStream& stream) const
{
    Component::serialize(stream);
    stream << m_lifeLeft;
}
void Cell::deserialize(BinaryStream& stream)
{
    Component::deserialize(stream);
    stream >> m_lifeLeft;
}

