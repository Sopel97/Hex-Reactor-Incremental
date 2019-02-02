#ifndef HEATEXCHANGER_H
#define HEATEXCHANGER_H

#include "Component.h"
#include "Bignum.h"

class HeatExchanger : public Component
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

    HeatExchanger(std::shared_ptr<CommonData>& commonData);
    HeatExchanger(ConfigurationNode& config);
    HeatExchanger(const HeatExchanger& heatExchanger);

    const BigInt& baseHeatTransferRate() const;

    virtual std::string formattedDescription(const std::string& desc) const;
    BigInt heatTransferRate() const;

    virtual bool isDepleted() const;

    virtual std::shared_ptr<Component::CommonData> commonData() const;

    virtual float condition() const; //this one affects baseValue
    virtual float conditionBarPercent() const; //this one affects rendered condition
    virtual bool hasConditionBar() const;
    virtual ComponentConditionBar conditionBar() const;

    virtual std::shared_ptr<Component> clone() const;
    virtual ~HeatExchanger();
protected:
    std::shared_ptr<CommonData> m_commonData;
};

#endif // HEATEXCHANGER_H
