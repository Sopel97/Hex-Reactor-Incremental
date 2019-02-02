#include "UpgradePool.h"
#include "Description.h"
#include "BinaryStream.h"

#include <cmath>

UpgradePool::UpgradePool() :
    m_testUpgrade
{
    "Test upgrade",
    Description<UpgradeBase>
    {
        std::vector<Description<UpgradeBase>::DescriptionPart>
        {
            Description<UpgradeBase>::DescriptionPart
            {
                "description"
            }
        }
    },
    "Uranium Cell (Single)",
    0,
    10,
    [](int lvl) -> BigInt {return 100;},
    [](int lvl) -> BigInt {return 200;}
},
m_cellLifespanUpgrade
{
    "Lifespan boost",
    Description<UpgradeBase>{
        std::vector<Description<UpgradeBase>::DescriptionPart>
        {
            Description<UpgradeBase>::DescriptionPart
            {
                "Increases lifespan of all cells by:\n"
                "Level %selected_level% (active): %selected_level_bonus%\n"
            },
            Description<UpgradeBase>::DescriptionPart
            {
                [](const UpgradeBase & arg) -> bool {return !arg.isHighestPossibleLevelSelected();},
                "Level %max_unlocked_level%: %max_unlocked_level_bonus%\n"
            },
            Description<UpgradeBase>::DescriptionPart
            {
                [](const UpgradeBase & arg) -> bool {return !arg.isLevelMaxed();},
                "Level %next_unlockable_level%: %next_unlockable_level_bonus%\n"
            },
            Description<UpgradeBase>::DescriptionPart
            {
                [](const UpgradeBase & arg) -> bool {return !arg.isLevelMaxed();},
                "Unlocking next level costs: $%next_unlockable_level_cost%"
            }
        }
    },
    "Uranium Cell (Single)",
    0,
    4,
    [](int lvl) -> BigInt {return 100ll * std::pow(2ll, lvl);},
    [](int lvl) -> float {return std::pow(1.15f, lvl);}
}
{

}
UpgradePool& UpgradePool::instance()
{
    static UpgradePool upgradePool;
    return upgradePool;
}

Upgrade<BigInt>& UpgradePool::testUpgrade()
{
    return m_testUpgrade;
}
Upgrade<float>& UpgradePool::cellLifespanUpgrade()
{
    return m_cellLifespanUpgrade;
}

float UpgradePool::cellLifespanMultiplier() const
{
    return m_cellLifespanUpgrade.selectedLevelBonusValue();
}

std::vector<UpgradeInterface> UpgradePool::createInterfaceForEveryUpgrade()
{
    std::vector<UpgradeInterface> interfaces;
    interfaces.emplace_back(m_testUpgrade, Vec2I {0, 0}); //positions are not being set here, instead UpgradeShop does the rest
    interfaces.emplace_back(m_cellLifespanUpgrade, Vec2I {0, 0});

    interfaces.emplace_back(m_testUpgrade, Vec2I {0, 0});
    interfaces.emplace_back(m_cellLifespanUpgrade, Vec2I {0, 0});
    interfaces.emplace_back(m_testUpgrade, Vec2I {0, 0});
    interfaces.emplace_back(m_cellLifespanUpgrade, Vec2I {0, 0});

    return interfaces;
}
void UpgradePool::serialize(BinaryStream& stream) const
{
    m_testUpgrade.serialize(stream);
    m_cellLifespanUpgrade.serialize(stream);
}
void UpgradePool::deserialize(BinaryStream& stream)
{
    m_testUpgrade.deserialize(stream);
    m_cellLifespanUpgrade.deserialize(stream);
}
