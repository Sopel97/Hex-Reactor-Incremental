#ifndef UPGRADE_H
#define UPGRADE_H

#include <functional>
#include <string>

#include "../LibS/Geometry.h"

#include "Bignum.h"
#include "UpgradeBase.h"

class UpgradeInterface;

using namespace Geo;

template <class BonusValueType>
class Upgrade : public UpgradeBase
{
public:
    Upgrade(const std::string& name, const Description<UpgradeBase>& description, const std::string& iconComponentName, int minLevel, int maxLevel, std::function<BigInt(int)>&& costFunction, std::function<BonusValueType(int)>&& bonusValueFunction);

    virtual void updateBonusValues();
    virtual void updateCosts();
    virtual std::string formattedDescription() const;

    const BonusValueType& nextUnlockableLevelBonusValue() const;
    const BonusValueType& selectedLevelBonusValue() const;
    const BonusValueType& maxUnlockedLevelBonusValue() const;

    virtual std::string nextLevelBonusValueAsString() const;
    virtual std::string selectedLevelBonusValueAsString() const;
    virtual std::string maxUnlockedLevelBonusValueAsString() const;

    const std::function<BigInt(int)>& costFunction();
    const std::function<BonusValueType(int)>& bonusValueFunction();

    UpgradeInterface createUpgradeInterface(const Vec2I& offset);
protected:
    BonusValueType m_nextUnlockableLevelBonusValue;
    BonusValueType m_selectedLevelBonusValue;
    BonusValueType m_maxUnlockedLevelBonusValue;

    std::function<BigInt(int)> m_costFunction;
    std::function<BonusValueType(int)> m_bonusValueFunction;
};

#include "../src/Upgrade.cpp"

#endif // UPGRADE_H
