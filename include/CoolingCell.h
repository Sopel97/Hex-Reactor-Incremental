#ifndef COOLINGCELL_H
#define COOLINGCELL_H

#include "Component.h"
#include "Bignum.h"

class CoolingCell : public Component
{
public:

    class CommonData : public Component::CommonData
    {
    public:
        CommonData() = default;
        CommonData(ConfigurationNode& config);
        CommonData(const std::string& name, const std::string& onShopDisplayDescription, const std::string& onGridDescription, const Vec2I& spritePos, const Vec2I& depletedSpritePos, const BigInt& basePrice);
        CommonData(const CommonData& commonData) = default;
        CommonData(CommonData&& commonData) = default;

        CommonData& operator=(const CommonData& commonData) = default;
        CommonData& operator=(CommonData && commonData) = default;
    };

    CoolingCell(std::shared_ptr<CommonData>& commonData);
    CoolingCell(ConfigurationNode& config);
    CoolingCell(const CoolingCell& coolingCell);

    virtual std::string formattedDescription(const std::string& desc) const;

    virtual bool isDepleted() const;

    virtual std::shared_ptr<Component::CommonData> commonData() const;

    virtual float condition() const; //this one affects baseValue
    virtual float conditionBarPercent() const; //this one affects rendered condition
    virtual bool hasConditionBar() const;
    virtual ComponentConditionBar conditionBar() const;

    virtual std::shared_ptr<Component> clone() const;
    virtual ~CoolingCell();
protected:
    std::shared_ptr<CommonData> m_commonData;
};

#endif // COOLINGCELL_H
