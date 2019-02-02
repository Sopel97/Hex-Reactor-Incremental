#ifndef HEATOUTLET_H
#define HEATOUTLET_H

#include "Component.h"
#include "Bignum.h"

class HeatOutlet : public Component
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

    HeatOutlet(std::shared_ptr<CommonData>& commonData);
    HeatOutlet(ConfigurationNode& config);
    HeatOutlet(const HeatOutlet& heatOutlet);

    virtual void exchangeHeat(Reactor& reactor, ReactorGrid::GridRegionAroundComponent& neighbourhood);

    const BigInt& baseHeatTransferRate() const;

    virtual std::string formattedDescription(const std::string& desc) const;
    BigInt heatTransferRate() const;

    virtual std::shared_ptr<Component::CommonData> commonData() const;

    virtual bool isDepleted() const;

    virtual std::shared_ptr<Component> clone() const;
    virtual ~HeatOutlet();
protected:
    std::shared_ptr<CommonData> m_commonData;
};

#endif // HEATOUTLET_H
