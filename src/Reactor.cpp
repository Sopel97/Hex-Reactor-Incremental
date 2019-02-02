#include "Reactor.h"
#include "Component.h"
#include "Game.h"
#include "BinaryStream.h"

Reactor::Reactor(Game& game, int gridWidth, int gridHeight) :
    m_game(game),
    m_reactorGrid(game, *this, gridWidth, gridHeight),
    m_storedHeat(0),
    m_baseHeatCapacity(10000),
    m_heatCapacity(m_baseHeatCapacity),
    m_storedPower(0),
    m_basePowerCapacity(10000),
    m_powerCapacity(m_basePowerCapacity)
{
}
void Reactor::update(double dt)
{
    m_reactorGrid.update(dt);
}
void Reactor::tick()
{
    std::vector<ReactorGrid::GridRegionAroundComponent> components;
    std::vector<Vec2I> possibleHexCoords = m_reactorGrid.possibleHexCoords();
    components.reserve(possibleHexCoords.size());
    for(auto& coords : possibleHexCoords)
    {
        auto& component = m_reactorGrid.componentAt(coords);
        if(component == nullptr) continue;
        components.emplace_back(m_reactorGrid, coords);
    }
    std::sort(components.begin(), components.end(), [](const ReactorGrid::GridRegionAroundComponent & lhs, const ReactorGrid::GridRegionAroundComponent & rhs) {return (unsigned)(lhs.component()->type()) < (unsigned)(rhs.component()->type());});

    recalculateReactorPowerCapacity(components);
    if(!reactorMeltdown())
    {
        generatePowerAndHeatFromComponents(components);
        exchangeHeatBetweenComponents(components);
        dissipateHeat(components);
    }
    recalculateReactorHeatCapacity(components);
    checkForMeltingOfComponents(components);

    for(auto& region : components)
    {
        region.component()->tick(*this, region);
    }
}

void Reactor::recalculateReactorPowerCapacity(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents)
{
    m_powerCapacity = m_basePowerCapacity;
    for(auto& region : sortedComponents)
    {
        m_powerCapacity += region.component()->reactorPowerCapacity();
    }
    addPower(0);
}
void Reactor::generatePowerAndHeatFromComponents(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents)
{
    for(auto& region : sortedComponents)
    {
        region.component()->generatePowerAndHeat(*this, region);
    }
}
void Reactor::exchangeHeatBetweenComponents(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents)
{
    for(auto& region : sortedComponents)
    {
        region.component()->exchangeHeat(*this, region);
    }
}
void Reactor::dissipateHeat(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents)
{
    for(auto& region : sortedComponents)
    {
        region.component()->dissipateHeat(*this, region);
    }
}
void Reactor::recalculateReactorHeatCapacity(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents)
{
    m_heatCapacity = m_baseHeatCapacity;
    for(auto& region : sortedComponents)
    {
        m_heatCapacity += region.component()->reactorHeatCapacity();
    }
    addHeat(0);
}
void Reactor::checkForMeltingOfComponents(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents)
{
    for(auto& region : sortedComponents)
    {
        if(region.component()->isMelting())
        {
            m_storedHeat += region.component()->heatToEmitOnMelting();
            m_reactorGrid.meltComponent(region.center());
        }
    }
    if(reactorMeltdown())
    {
        m_reactorGrid.meltAllComponents();
    }

}
void Reactor::sellPower()
{
    m_game.playerStats().addMoney(m_storedPower);
    m_storedPower = 0;
}
bool Reactor::reactorMeltdown() const
{
    return m_storedHeat > m_heatCapacity;
}
void Reactor::draw()
{
    m_reactorGrid.draw();
}
ReactorGrid& Reactor::reactorGrid()
{
    return m_reactorGrid;
}
const ReactorGrid& Reactor::reactorGrid() const
{
    return m_reactorGrid;
}

float Reactor::heatPercentage() const
{
    return (m_storedHeat * 100ll / m_heatCapacity) / 100.0f;
}
float Reactor::powerPercentage() const
{
    return (m_storedPower * 100ll / m_powerCapacity) / 100.0f;
}
void Reactor::addHeat(BigInt heat)
{
    m_storedHeat += heat;
    if(m_storedHeat < 0) m_storedHeat = 0;
}
void Reactor::addPower(BigInt power)
{
    m_storedPower += power;
    if(m_storedPower < 0) m_storedPower = 0;
    if(m_storedPower > m_powerCapacity) m_storedPower = m_powerCapacity;
}

const BigInt& Reactor::storedHeat() const
{
    return m_storedHeat;
}
const BigInt& Reactor::storedPower() const
{
    return m_storedPower;
}
const BigInt& Reactor::heatCapacity() const
{
    return m_heatCapacity;
}
const BigInt& Reactor::powerCapacity() const
{
    return m_powerCapacity;
}
const BigInt& Reactor::baseHeatCapacity() const
{
    return m_baseHeatCapacity;
}
const BigInt& Reactor::basePowerCapacity() const
{
    return m_basePowerCapacity;
}
void Reactor::onWindowResized(sf::Event& windowResizedEvent)
{
    m_reactorGrid.onWindowResized(windowResizedEvent);
}

void Reactor::serialize(BinaryStream& stream) const
{
    stream << m_storedHeat;
    stream << m_storedPower;
    stream << m_heatCapacity;
    stream << m_powerCapacity;

    m_reactorGrid.serialize(stream);
}
void Reactor::deserialize(BinaryStream& stream)
{
    stream >> m_storedHeat;
    stream >> m_storedPower;
    stream >> m_heatCapacity;
    stream >> m_powerCapacity;

    m_reactorGrid.deserialize(stream);
}
