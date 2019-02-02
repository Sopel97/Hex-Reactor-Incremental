#ifndef REACTOR_H
#define REACTOR_H

#include "ReactorGrid.h"
#include "Bignum.h"
#include "Serializable.h"

class Game;

class Reactor : public Serializable
{
public:
    Reactor(Game& game, int gridWidth, int gridHeight);

    ReactorGrid& reactorGrid();
    const ReactorGrid& reactorGrid() const;

    void update(double dt);
    void draw();

    void tick(); //calls all 7 phases, each phase calls respective component tick (for all components on grid) which take Reactor& as a parameter as well as position on ReactorGrid
    void recalculateReactorPowerCapacity(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents); //1st phase of tick [tick of components: none (reactor handles all components)]
    void generatePowerAndHeatFromComponents(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents); //2nd phase [tick of components: cells]
    void exchangeHeatBetweenComponents(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents); //3rd phase [tick of components: outlet/inlet pomps (sequenced), heat exchangers, cells]
    void dissipateHeat(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents); //4th phase [tick of components: heat vents, reactor itself]
    void recalculateReactorHeatCapacity(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents); //5th phase [tick of components: none (reactor handles all components)]
    void checkForMeltingOfComponents(std::vector<ReactorGrid::GridRegionAroundComponent>& sortedComponents); //6th phase [tick of components: none (reactor handles all components)]. Will also handle reactor meltdown

    float heatPercentage() const;
    float powerPercentage() const;
    void addHeat(BigInt heat);
    void addPower(BigInt power);

    const BigInt& storedHeat() const;
    const BigInt& storedPower() const;
    const BigInt& baseHeatCapacity() const;
    const BigInt& basePowerCapacity() const;
    const BigInt& heatCapacity() const;
    const BigInt& powerCapacity() const;


    void sellPower();

    bool reactorMeltdown() const;

    void onWindowResized(sf::Event& windowResizedEvent);

    virtual void serialize(BinaryStream& stream) const;
    virtual void deserialize(BinaryStream& stream);

private:
    Game& m_game;
    ReactorGrid m_reactorGrid;
    BigInt m_storedHeat, m_baseHeatCapacity, m_heatCapacity;
    BigInt m_storedPower, m_basePowerCapacity, m_powerCapacity;
};

#endif // REACTOR_H
