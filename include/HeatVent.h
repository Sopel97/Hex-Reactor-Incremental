#ifndef HEATVENT_H
#define HEATVENT_H

#include "Component.h"
#include "Bignum.h"

class HeatVent : public Component
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

        const BigInt& baseHeatDissipationRate() const;
    protected:
        BigInt m_baseHeatDissipationRate;
    };

    HeatVent(std::shared_ptr<CommonData>& commonData);
    HeatVent(ConfigurationNode& config);
    HeatVent(const HeatVent& heatVent);

    virtual void dissipateHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood);

    const BigInt& baseHeatDissipationRate() const;

    virtual std::string formattedDescription(const std::string& desc) const;
    BigInt heatDissipationRate() const;

    virtual bool isDepleted() const;

    virtual std::shared_ptr<Component::CommonData> commonData() const;

    virtual float condition() const; //this one affects baseValue
    virtual float conditionBarPercent() const; //this one affects rendered condition
    virtual bool hasConditionBar() const;
    virtual ComponentConditionBar conditionBar() const;

    virtual std::shared_ptr<Component> clone() const;
    virtual ~HeatVent();
protected:
    std::shared_ptr<CommonData> m_commonData;
};
#endif // HEATVENT_H
