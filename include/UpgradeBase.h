#ifndef UPGRADEBASE_H
#define UPGRADEBASE_H

#include <string>
#include <memory>

#include "Bignum.h"
#include "Description.h"
#include "Serializable.h"

class Component;

class UpgradeBase : public Serializable
{
public:
    struct FormatTokens
    {
        static constexpr const char* minLevel = "%min_level%";
        static constexpr const char* selectedLevel = "%selected_level%";
        static constexpr const char* maxUnlockedLevel = "%max_unlocked_level%";
        static constexpr const char* maxLevel = "%max_level%";
        static constexpr const char* nextUnlockableLevel = "%next_unlockable_level%";
        static constexpr const char* selectedLevelBonus = "%selected_level_bonus%";
        static constexpr const char* maxUnlockedLevelBonus = "%max_unlocked_level_bonus%";
        static constexpr const char* nextUnlockableLevelBonus = "%next_unlockable_level_bonus%";
        static constexpr const char* nextUnlockableLevelCost = "%next_unlockable_level_cost%";
    };

    UpgradeBase(const std::string& name, const Description<UpgradeBase>& description, const std::string& iconComponentName, int minLevel, int maxLevel);

    virtual void updateUpgrade();
    virtual void updateBonusValues() = 0;
    virtual void updateCosts() = 0;

    const std::string& name() const;
    const Description<UpgradeBase>& description() const;
    virtual std::string formattedDescription() const;

    int minLevel() const;
    int maxLevel() const;
    int selectedLevel() const;
    int maxUnlockedLevel() const;

    std::shared_ptr<Component> iconComponent() const;

    bool tryIncreaseSelectedLevel();
    bool tryDecreaseSelectedLevel();
    bool tryUnlockNextLevel();

    bool isNextLevelAffordable(const BigInt& money) const;
    bool isLevelMaxed() const;
    bool isLowestPossibleLevelSelected() const;
    bool isHighestPossibleLevelSelected() const;

    virtual std::string nextLevelBonusValueAsString() const = 0;
    virtual std::string selectedLevelBonusValueAsString() const = 0;
    virtual std::string maxUnlockedLevelBonusValueAsString() const = 0;

    const BigInt& nextUnlockableLevelCost() const;

    virtual void serialize(BinaryStream& stream) const;
    virtual void deserialize(BinaryStream& stream);
protected:
    std::string m_name;
    Description<UpgradeBase> m_description;
    std::string m_iconComponentName;
    int m_minLevel;
    int m_maxLevel;
    int m_selectedLevel;
    int m_maxUnlockedLevel;

    BigInt m_nextUnlockableLevelCost;
};

#endif // UPGRADEBASE_H
