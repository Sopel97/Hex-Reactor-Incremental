#ifndef REACTORPLATING_H
#define REACTORPLATING_H

#include "Component.h"
#include "Bignum.h"

class ReactorPlating : public Component
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

        const BigInt& baseReactorHeatCapacity() const;
    protected:
        BigInt m_baseReactorHeatCapacity;
    };

    ReactorPlating(std::shared_ptr<CommonData>& commonData);
    ReactorPlating(ConfigurationNode& config);
    ReactorPlating(const ReactorPlating& reactorPlating);

    BigInt baseReactorHeatCapacity() const;

    virtual std::string formattedDescription(const std::string& desc) const;
    virtual BigInt reactorHeatCapacity() const;

    virtual bool isDepleted() const;

    virtual std::shared_ptr<Component::CommonData> commonData() const;

    virtual std::shared_ptr<Component> clone() const;
    virtual ~ReactorPlating();
protected:
    std::shared_ptr<CommonData> m_commonData;
};

#endif // REACTORPLATING_H
