#ifndef CELL_H
#define CELL_H

#include "Component.h"
#include "Bignum.h"

class Cell : public Component
{
public:
    enum class Variant
    {
        Single,
        Double,
        Quad,
        IsolatedSingle,
        IsolatedDouble,
        IsolatedQuad
    };

    class CommonData : public Component::CommonData
    {
    public:
        struct Bonuses
        {
            float lifespanMultiplier = 1.0f;
        };
        CommonData() = default;
        CommonData(ConfigurationNode& config);
        CommonData(const CommonData& commonData) = default;
        CommonData(CommonData&& commonData) = default;

        CommonData& operator=(const CommonData& commonData) = default;
        CommonData& operator=(CommonData && commonData) = default;

        virtual void updateBonuses(UpgradePool& upgradePool);

        const BigInt& basePowerOutput() const;
        const BigInt& baseHeatOutput() const;
        Cell::Variant variant() const;
        BigInt baseLifespan() const;

        Bonuses& activeBonuses();

    protected:
        Bonuses m_activeBonuses;

        Cell::Variant m_variant;
        BigInt m_baseLifespan;
        BigInt m_basePowerOutput; //base is always a base of single of the same type. So it is automatically multiplied regarding double/quad later
        BigInt m_baseHeatOutput;


    };

    Cell(std::shared_ptr<CommonData>& commonData);
    Cell(ConfigurationNode& config);
    Cell(const Cell& cell);

    virtual void generatePowerAndHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood);
    virtual void tick(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood);

    virtual void resetToBaseState();
    virtual void updateAfterUpgradesChanged();

    const std::string& onDepletedDescription() const;
    const BigInt& basePowerOutput() const;
    const BigInt& baseHeatOutput() const;
    Variant variant() const;
    BigInt baseLifespan() const;
    BigInt lifePassed() const;
    BigInt lifeLeft() const;

    virtual std::string formattedDescription(const std::string& desc) const;
    virtual std::string formattedOnGridDescription(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const;
    BigInt lifespan() const;

    BigInt powerOutput(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const;
    BigInt heatOutput(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const;
    BigInt ownPowerOutput() const; //does not count any multipliers from components on grid
    BigInt ownHeatOutput() const;

    virtual bool isDepleted() const;

    virtual std::shared_ptr<Component::CommonData> commonData() const;
    virtual float condition() const; //this one affects baseValue
    virtual float conditionBarPercent() const; //this one affects rendered condition
    virtual bool hasConditionBar() const;
    virtual ComponentConditionBar conditionBar() const;

    float powerMultiplier(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const;
    float heatMultiplier(const ReactorGrid::GridRegionAroundComponent& neighbourhood) const;
    virtual float powerOutputMultiplier() const; //will be applied after all other multipliers
    virtual float additivePowerOutputMultiplier() const; //will be applied first
    virtual float heatOutputMultiplier() const;  //will be applied after all other multipliers
    virtual float additiveHeatOutputMultiplier() const; //will be applied first
    //resulting heat generation multiplier is ALWAYS squared
    float effectiveAdditivePowerOutputMultiplier() const; //additive generation multiplier is multiplied by this
    float effectiveAdditiveHeatOutputMultiplier() const;

    virtual std::shared_ptr<Component> clone() const;
    virtual ~Cell();

    virtual void serialize(BinaryStream& stream) const;
    virtual void deserialize(BinaryStream& stream);
protected:
    std::shared_ptr<CommonData> m_commonData;
    BigInt m_lifeLeft;
};

#endif // CELL_H
