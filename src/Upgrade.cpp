#include "Upgrade.h"
#include "TextRenderer.h"
#include "UpgradeInterface.h"
#include "StringFormatter.h"

template <class BonusValueType>
Upgrade<BonusValueType>::Upgrade(const std::string& name, const Description<UpgradeBase>& description, const std::string& iconComponentName, int minLevel, int maxLevel, std::function<BigInt(int)>&& costFunction, std::function<BonusValueType(int)>&& bonusValueFunction) :
    UpgradeBase(name, description, iconComponentName, minLevel, maxLevel),
    m_costFunction(costFunction),
    m_bonusValueFunction(bonusValueFunction)
{
    updateBonusValues();
    updateCosts();
}
template <class BonusValueType>
void Upgrade<BonusValueType>::updateBonusValues()
{
    int nextLevel = m_maxUnlockedLevel + 1;
    if(nextLevel <= m_maxLevel)
    {
        m_nextUnlockableLevelBonusValue = m_bonusValueFunction(nextLevel);
    }
    m_selectedLevelBonusValue = m_bonusValueFunction(m_selectedLevel);
    m_maxUnlockedLevelBonusValue = m_bonusValueFunction(m_maxUnlockedLevel);
}
template <class BonusValueType>
void Upgrade<BonusValueType>::updateCosts()
{
    int nextLevel = m_maxUnlockedLevel + 1;
    if(nextLevel <= m_maxLevel)
    {
        m_nextUnlockableLevelCost = m_costFunction(nextLevel);
    }
}
template <class BonusValueType>
std::string Upgrade<BonusValueType>::formattedDescription() const
{
    std::string nextUnlockableLevelBonus = m_maxUnlockedLevel + 1 > m_maxLevel ? "N/A" : TextRenderer::formatFancy(m_nextUnlockableLevelBonusValue);
    std::string nextUnlockableLevelCost = m_maxUnlockedLevel + 1 > m_maxLevel ? "N/A" : TextRenderer::formatFancy(m_nextUnlockableLevelCost);
    return StringFormatter::formatString(m_description.buildDescription(*this),
    {
        StringFormatter::Rule{UpgradeBase::FormatTokens::maxLevel, std::to_string(m_maxLevel)},
        StringFormatter::Rule{UpgradeBase::FormatTokens::maxUnlockedLevel, std::to_string(m_maxUnlockedLevel)},
        StringFormatter::Rule{UpgradeBase::FormatTokens::minLevel, std::to_string(m_minLevel)},
        StringFormatter::Rule{UpgradeBase::FormatTokens::selectedLevel, std::to_string(m_selectedLevel)},
        StringFormatter::Rule{UpgradeBase::FormatTokens::nextUnlockableLevel, std::to_string(m_maxUnlockedLevel + 1)},
        StringFormatter::Rule{UpgradeBase::FormatTokens::maxUnlockedLevelBonus, TextRenderer::formatFancy(m_maxUnlockedLevelBonusValue)},
        StringFormatter::Rule{UpgradeBase::FormatTokens::selectedLevelBonus, TextRenderer::formatFancy(m_selectedLevelBonusValue)},
        StringFormatter::Rule{UpgradeBase::FormatTokens::nextUnlockableLevelBonus, nextUnlockableLevelBonus},
        StringFormatter::Rule{UpgradeBase::FormatTokens::nextUnlockableLevelCost, nextUnlockableLevelCost}
    });
}
template <class BonusValueType>
const BonusValueType& Upgrade<BonusValueType>::nextUnlockableLevelBonusValue() const
{
    return m_nextUnlockableLevelBonusValue;
}
template <class BonusValueType>
const BonusValueType& Upgrade<BonusValueType>::selectedLevelBonusValue() const
{
    return m_selectedLevelBonusValue;
}
template <class BonusValueType>
const BonusValueType& Upgrade<BonusValueType>::maxUnlockedLevelBonusValue() const
{
    return m_maxUnlockedLevelBonusValue;
}
template <class BonusValueType>
std::string Upgrade<BonusValueType>::nextLevelBonusValueAsString() const
{
    return TextRenderer::formatFancy(m_nextUnlockableLevelBonusValue);
}
template <class BonusValueType>
std::string Upgrade<BonusValueType>::selectedLevelBonusValueAsString() const
{
    return TextRenderer::formatFancy(m_selectedLevelBonusValue);
}
template <class BonusValueType>
std::string Upgrade<BonusValueType>::maxUnlockedLevelBonusValueAsString() const
{
    return TextRenderer::formatFancy(m_maxUnlockedLevelBonusValue);
}

template <class BonusValueType>
const std::function<BigInt(int)>& Upgrade<BonusValueType>::costFunction()
{
    return m_costFunction;
}
template <class BonusValueType>
const std::function<BonusValueType(int)>& Upgrade<BonusValueType>::bonusValueFunction()
{
    return m_bonusValueFunction;
}


template <class BonusValueType>
UpgradeInterface Upgrade<BonusValueType>::createUpgradeInterface(const Vec2I& offset)
{
    return UpgradeInterface(*this, offset);
}
