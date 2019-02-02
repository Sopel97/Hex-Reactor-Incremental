#ifndef COMPONENT_H
#define COMPONENT_H

#include "../LibS/Geometry.h"
#include <memory>
#include "Bignum.h"
#include "ReactorGrid.h"
#include "Configuration.h"
#include "UpgradePool.h"

class ComponentStaticData;
class ComponentConditionBar;
class Reactor;

using namespace Geo;

class Component : public Serializable
{
public:
    enum class Type
    {
        Cell,
        Capacitor,
        NeutronReflector,
        HeatVent,
        HeatExchanger,
        HeatInlet,
        HeatOutlet,
        CoolingCell,
        ReactorPlating
    };

    struct FormatTokens
    {
        static constexpr const char* ownPowerOutput = "%own_power_out%"; //- how much power a component generates on its own (excluding other component on grid, but including bonuses)
        static constexpr const char* ownHeatOutput = "%own_heat_out%"; //- same but for heat
        static constexpr const char* powerOutput = "%power_out%"; //- power being produced by component including all factors
        static constexpr const char* heatOutput = "%heat_out%"; //- heat being produced by component including all factors
        static constexpr const char* lifespan = "%lifespan%"; //- lifespan of the component
        static constexpr const char* lifeLeft = "%life_left%"; //- life left
        static constexpr const char* lifePassed = "%life_passed%"; //- passed life
        static constexpr const char* storedHeat = "%stored_heat%"; //- amount of stored heat
        static constexpr const char* heatCapacity = "%heat_capacity%"; //- max amount of heat in the component (including all bonuses)
        static constexpr const char* reactorHeatCapacity = "%reactor_heat_capacity%"; //- capacity of heat added to the reactor
        static constexpr const char* reactorPowerCapacity = "%reactor_power_capacity%"; //- capacity of power added to the reactor
        static constexpr const char* powerOutputMultiplier = "%power_out_mul%"; //- power generation multiplier for near components (not in percent, just a value)
        static constexpr const char* heatOutputMultiplier = "%heat_out_mul%"; //- heat generation multiplier for near components (not in percent, just a value)
        static constexpr const char* name = "%name%"; //- full name of the component
        static constexpr const char* price = "%price%"; //- price of a new component
        static constexpr const char* sellValue = "%sell_value%"; //- value of a component (sell value)
        static constexpr const char* heatTransferRate = "%heat_transfer_rate%"; //- heat transfer (between component or reactor) efficincy of the component
        static constexpr const char* heatDissipationRate = "%heat_dissipation_rate%"; //- heat dissipation (nullifying heat) efficincy of the component
    };

    class CommonData
    {
    public:
        CommonData() = default;
        CommonData(ConfigurationNode& config);
        CommonData(const CommonData& commonData) = default;
        CommonData(CommonData&& commonData) = default;

        CommonData& operator=(const CommonData& commonData) = default;
        CommonData& operator=(CommonData && commonData) = default;

        virtual void updateBonuses(UpgradePool& upgradePool);

        const std::string& name() const;
        const std::string& onShopDisplayDescription() const;
        const std::string& onGridDescription() const;
        const std::string& onDepletedDescription() const;
        const Vec2I& spritePosition() const;
        const Vec2I& depletedSpritePosition() const;
        const BigInt& basePrice() const;
        const BigInt& baseHeatCapacity() const;
    private:
        std::string m_name;
        std::string m_onShopDisplayDescription;
        std::string m_onGridDescription;
        std::string m_onDepletedDescription;
        Vec2I m_spritePosition;
        Vec2I m_depletedSpritePosition;
        BigInt m_basePrice;
        BigInt m_baseHeatCapacity; //-1 means that the component heat exists ONLY for this component (i.e. other components are not supposed to see it. It may be used for convinience like cells do)
    };

    Component(Type type);

    virtual void generatePowerAndHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood);
    virtual void exchangeHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood);
    virtual void dissipateHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood);
    virtual void tick(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood); //will be always called last. Usually it will add one to lifePassed or do nothing

    virtual void resetToBaseState(); //this changes values such as m_lifeLeft, it should only be applied to objects in component factory to update their base states
    virtual void updateAfterUpgradesChanged(); //after buying upgrade it may be needed to for example trim lifeleft to the lifespan value

    Type type() const;
    const BigInt& baseValue() const;
    const BigInt& storedHeat() const;

    const std::string& name() const;
    const std::string& onShopDisplayDescription() const;
    const std::string& onGridDescription() const;
    const std::string& onDepletedDescription() const;
    const Vec2I& spritePosition() const;
    const Vec2I& depletedSpritePosition() const;
    const BigInt& basePrice() const;
    const BigInt& baseHeatCapacity() const;

    virtual std::string formattedDescription(const std::string& desc) const = 0;
    virtual std::string formattedOnShopDisplayDescription() const;
    virtual std::string formattedOnGridDescription(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const;
    virtual BigInt sellValue() const;
    BigInt price() const;
    BigInt heatCapacity() const;

    BigInt heatToEmitOnMelting() const;

    virtual bool isMelting() const;
    virtual bool canStoreHeat() const;
    virtual bool acceptsHeat() const;
    virtual bool isDepleted() const = 0;

    void addHeat(const BigInt& amount);
    void setBaseValue(const BigInt& baseValue);

    virtual std::shared_ptr<CommonData> commonData() const = 0;

    virtual float condition() const; //this one affects baseValue
    virtual float conditionBarPercent() const; //this one affects rendered condition
    virtual bool hasConditionBar() const;
    virtual ComponentConditionBar conditionBar() const;


    virtual BigInt reactorPowerCapacity() const; //these two may be resolved diffrently later
    virtual BigInt reactorHeatCapacity() const;

    virtual float powerOutputMultiplier() const; //will be used by reflectors and possibly by cells to inform how much they affect other cells
    virtual float additivePowerOutputMultiplier() const;
    virtual float heatOutputMultiplier() const;
    virtual float additiveHeatOutputMultiplier() const;

    virtual std::shared_ptr<Component> clone() const = 0;
    virtual bool canReplace(const std::shared_ptr<Component>& old) const;
    virtual ~Component();

    virtual void serialize(BinaryStream& stream) const;
    virtual void deserialize(BinaryStream& stream);
protected:
    Type m_type; //type should remain in component class
    BigInt m_baseValue; //baseValue may differ based on time of purchase
    BigInt m_storedHeat;
};

#endif // COMPONENT_H
