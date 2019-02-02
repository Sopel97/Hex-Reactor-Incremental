#ifndef NEUTRONREFLECTOR_H
#define NEUTRONREFLECTOR_H

#include "Component.h"
#include "Bignum.h"

class NeutronReflector : public Component
{
public:

    class CommonData : public Component::CommonData
    {
    public:
        CommonData() = default;
        CommonData(ConfigurationNode& config);
        CommonData(const CommonData& commonData) = default;
        CommonData(CommonData&& commonData) = default;

        CommonData& operator=(const CommonData& commonData) = default;
        CommonData& operator=(CommonData && commonData) = default;

        float basePowerOutputMultiplier() const;
        BigInt baseLifespan() const;

    protected:
        float m_basePowerOutputMultiplier;
        BigInt m_baseLifespan;
    };

    NeutronReflector(std::shared_ptr<CommonData>& commonData);
    NeutronReflector(ConfigurationNode& config);
    NeutronReflector(const NeutronReflector& neutronReflector);

    virtual void tick(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood);

    virtual void resetToBaseState();
    virtual void updateAfterUpgradesChanged();

    float basePowerOutputMultiplier() const;
    BigInt baseLifespan() const;
    BigInt lifePassed() const;
    BigInt lifeLeft() const;

    virtual std::string formattedDescription(const std::string& desc) const;
    virtual float powerOutputMultiplier() const;
    BigInt lifespan() const;

    virtual bool isDepleted() const;

    virtual std::shared_ptr<Component::CommonData> commonData() const;
    virtual float condition() const; //this one affects baseValue
    virtual float conditionBarPercent() const; //this one affects rendered condition
    virtual bool hasConditionBar() const;
    virtual ComponentConditionBar conditionBar() const;

    virtual std::shared_ptr<Component> clone() const;
    virtual ~NeutronReflector();

    virtual void serialize(BinaryStream& stream) const;
    virtual void deserialize(BinaryStream& stream);
protected:
    std::shared_ptr<CommonData> m_commonData;
    BigInt m_lifeLeft;
};

#endif // NEUTRONREFLECTOR_H
