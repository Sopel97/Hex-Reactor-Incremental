#ifndef HEATINLET_H
#define HEATINLET_H

#include "Component.h"
#include "Bignum.h"

class HeatInlet : public Component
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

        const BigInt& baseHeatTransferRate() const;
    protected:
        BigInt m_baseHeatTransferRate;
    };

    HeatInlet(std::shared_ptr<CommonData>& commonData);
    HeatInlet(ConfigurationNode& config);
    HeatInlet(const HeatInlet& heatInlet);

    virtual void exchangeHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood);

    const BigInt& baseHeatTransferRate() const;

    virtual std::string formattedDescription(const std::string& desc) const;
    BigInt heatTransferRate() const;

    virtual std::shared_ptr<Component::CommonData> commonData() const;

    virtual bool isDepleted() const;

    virtual std::shared_ptr<Component> clone() const;
    virtual ~HeatInlet();
protected:
    std::shared_ptr<CommonData> m_commonData;
};

#endif // HEATINLET_H
