#include "UpgradeBase.h"
#include "Root.h"
#include "BinaryStream.h"

#include <iostream>

UpgradeBase::UpgradeBase(const std::string& name, const Description<UpgradeBase>& description, const std::string& iconComponentName, int minLevel, int maxLevel) :
    m_name(name),
    m_description(description),
    m_iconComponentName(iconComponentName),
    m_minLevel(minLevel),
    m_maxLevel(maxLevel),
    m_selectedLevel(minLevel),
    m_maxUnlockedLevel(minLevel)
{
}

const std::string& UpgradeBase::name() const
{
    return m_name;
}
const Description<UpgradeBase>& UpgradeBase::description() const
{
    return m_description;
}
std::string UpgradeBase::formattedDescription() const
{
    return m_description.buildDescription(*this);
}

int UpgradeBase::minLevel() const
{
    return m_minLevel;
}
int UpgradeBase::maxLevel() const
{
    return m_maxLevel;
}
int UpgradeBase::selectedLevel() const
{
    return m_selectedLevel;
}
int UpgradeBase::maxUnlockedLevel() const
{
    return m_maxUnlockedLevel;
}
std::shared_ptr<Component> UpgradeBase::iconComponent() const
{
    return Root::instance().game().componentFactory().createComponent(m_iconComponentName);
}

const BigInt& UpgradeBase::nextUnlockableLevelCost() const
{
    return m_nextUnlockableLevelCost;
}
bool UpgradeBase::isNextLevelAffordable(const BigInt& money) const
{
    if(isLevelMaxed()) return false;
    return money >= m_nextUnlockableLevelCost;
}
bool UpgradeBase::isLevelMaxed() const
{
    return m_maxUnlockedLevel == m_maxLevel;
}
bool UpgradeBase::isLowestPossibleLevelSelected() const
{
    return m_selectedLevel == m_minLevel;
}
bool UpgradeBase::isHighestPossibleLevelSelected() const
{
    return m_selectedLevel == m_maxUnlockedLevel;
}

void UpgradeBase::updateUpgrade()
{
    updateBonusValues();
    updateCosts();
    Root::instance().game().componentFactory().updateBonusesInComponentCommonDatas(UpgradePool::instance());
    Root::instance().game().componentFactory().resetTemplateComponentsToTheirBaseState();
    Root::instance().game().reactor().reactorGrid().updateComponentsAfterUpgradesChanged();
}
bool UpgradeBase::tryIncreaseSelectedLevel()
{
    if(m_selectedLevel < m_maxUnlockedLevel)
    {
        ++m_selectedLevel;
        updateUpgrade();
        return true;
    }
    return false;
}
bool UpgradeBase::tryDecreaseSelectedLevel()
{
    if(m_selectedLevel > m_minLevel)
    {
        --m_selectedLevel;
        updateUpgrade();
        return true;
    }
    return false;
}
bool UpgradeBase::tryUnlockNextLevel()
{
    auto& playerStats = Root::instance().game().playerStats();
    if(playerStats.money() < m_nextUnlockableLevelCost) return false;
    if(m_maxUnlockedLevel < m_maxLevel)
    {
        ++m_maxUnlockedLevel;
        playerStats.addMoney(-m_nextUnlockableLevelCost);
        updateUpgrade();
        return true;
    }
    return false;
}
void UpgradeBase::serialize(BinaryStream& stream) const
{
    stream << m_selectedLevel;
    stream << m_maxUnlockedLevel;
}
void UpgradeBase::deserialize(BinaryStream& stream)
{
    stream >> m_selectedLevel;
    stream >> m_maxUnlockedLevel;
    updateUpgrade();
}
