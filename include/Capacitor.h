#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "Component.h"
#include "Bignum.h"

class Capacitor : public Component
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

        const BigInt& baseReactorPowerCapacity() const;
    protected:
        BigInt m_baseReactorPowerCapacity;
    };

    Capacitor(std::shared_ptr<CommonData>& commonData);
    Capacitor(ConfigurationNode& config);
    Capacitor(const Capacitor& capacitor);

    virtual bool isDepleted() const;

    const BigInt& baseReactorPowerCapacity() const;

    virtual std::string formattedDescription(const std::string& desc) const;
    virtual BigInt reactorPowerCapacity() const;

    virtual std::shared_ptr<Component::CommonData> commonData() const;
    virtual float condition() const; //this one affects baseValue
    virtual float conditionBarPercent() const; //this one affects rendered condition
    virtual bool hasConditionBar() const;
    virtual ComponentConditionBar conditionBar() const;

    virtual std::shared_ptr<Component> clone() const;
    virtual ~Capacitor();
protected:
    std::shared_ptr<CommonData> m_commonData;
};

#endif // CAPACITOR_H
