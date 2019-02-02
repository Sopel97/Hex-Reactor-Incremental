#include "NeutronReflector.h"
#include "ComponentConditionBar.h"
#include "StringFormatter.h"
#include "TextRenderer.h"
#include "BinaryStream.h"

/*
    Common data
*/

NeutronReflector::CommonData::CommonData(ConfigurationNode& config) :
    Component::CommonData(config)
{
    m_baseLifespan = config["baseLifespan"].get<int>();
    m_basePowerOutputMultiplier = config["basePowerOutputMultiplier"].get<float>();
}
float NeutronReflector::CommonData::basePowerOutputMultiplier() const
{
    return m_basePowerOutputMultiplier;
}
BigInt NeutronReflector::CommonData::baseLifespan() const
{
    return m_baseLifespan;
}

/*
    Neutron Reflector
*/

NeutronReflector::NeutronReflector(std::shared_ptr<NeutronReflector::CommonData>& commonData) :
    Component(Component::Type::NeutronReflector),
    m_commonData(commonData)
{
    m_lifeLeft = lifespan();

}
NeutronReflector::NeutronReflector(ConfigurationNode& config) :
    Component(Component::Type::NeutronReflector),
    m_commonData(std::make_shared<NeutronReflector::CommonData>(config))
{
    m_lifeLeft = lifespan();
}
NeutronReflector::NeutronReflector(const NeutronReflector& neutronReflector) :
    Component(Component::Type::NeutronReflector),
    m_commonData(neutronReflector.m_commonData),
    m_lifeLeft(neutronReflector.m_lifeLeft)
{

}

void NeutronReflector::tick(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood)
{
    if(isDepleted()) return;
    for(const auto& component : neighbourhood.neighbours())
    {
        if(component->type() == Component::Type::Cell && !component->isDepleted()) --m_lifeLeft;
    }
}

void NeutronReflector::resetToBaseState()
{
    m_lifeLeft = lifespan();
}
void NeutronReflector::updateAfterUpgradesChanged()
{
    m_lifeLeft = std::min(m_lifeLeft, lifespan());
}

float NeutronReflector::basePowerOutputMultiplier() const
{
    return m_commonData->basePowerOutputMultiplier();
}
BigInt NeutronReflector::baseLifespan() const
{
    return m_commonData->baseLifespan();
}
BigInt NeutronReflector::lifePassed() const
{
    return lifespan() - m_lifeLeft;
}
BigInt NeutronReflector::lifeLeft() const
{
    return m_lifeLeft;
}

std::string NeutronReflector::formattedDescription(const std::string& desc) const
{
    return StringFormatter::formatString(desc,
    {
        StringFormatter::Rule{Component::FormatTokens::name, name()},
        StringFormatter::Rule{Component::FormatTokens::price, TextRenderer::formatFancy(price())},
        StringFormatter::Rule{Component::FormatTokens::sellValue, TextRenderer::formatFancy(sellValue())},
        StringFormatter::Rule{Component::FormatTokens::powerOutputMultiplier, TextRenderer::formatFancy(powerOutputMultiplier())},
        StringFormatter::Rule{Component::FormatTokens::lifespan, TextRenderer::formatFancy(lifespan())},
        StringFormatter::Rule{Component::FormatTokens::lifeLeft, TextRenderer::formatFancy(lifeLeft())},
        StringFormatter::Rule{Component::FormatTokens::lifePassed, TextRenderer::formatFancy(sellValue())}
    });
}
float NeutronReflector::powerOutputMultiplier() const
{
    if(isDepleted()) return 1.0f;
    return basePowerOutputMultiplier();
}
BigInt NeutronReflector::lifespan() const
{
    return baseLifespan();
}

bool NeutronReflector::isDepleted() const
{
    return m_lifeLeft <= 0;
}

std::shared_ptr<Component::CommonData> NeutronReflector::commonData() const
{
    return m_commonData;
}

float NeutronReflector::condition() const
{
    if(isDepleted()) return 0.0f;
    return 1.0f - (float)(lifePassed() * 100 / lifespan()) / 100.0f;
}
float NeutronReflector::conditionBarPercent() const
{
    return condition();
}
bool NeutronReflector::hasConditionBar() const
{
    if(isDepleted()) return false;
    return true;
}
ComponentConditionBar NeutronReflector::conditionBar() const
{
    return ComponentConditionBar(ComponentConditionBar::Type::LifeBar, conditionBarPercent());
}

std::shared_ptr<Component> NeutronReflector::clone() const
{
    return std::make_shared<NeutronReflector>(*this);
}
NeutronReflector::~NeutronReflector()
{
}

void NeutronReflector::serialize(BinaryStream& stream) const
{
    Component::serialize(stream);
    stream << m_lifeLeft;
}
void NeutronReflector::deserialize(BinaryStream& stream)
{
    Component::deserialize(stream);
    stream >> m_lifeLeft;
}
