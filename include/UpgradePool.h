#ifndef UPGRADEPOOL_H
#define UPGRADEPOOL_H

#include "Bignum.h"
#include "Upgrade.h"
#include "Serializable.h"

class UpgradePool : public Serializable
{
public:
    static UpgradePool& instance();

    Upgrade<BigInt>& testUpgrade();
    Upgrade<float>& cellLifespanUpgrade();

    float cellLifespanMultiplier() const;
    std::vector<UpgradeInterface> createInterfaceForEveryUpgrade();

    virtual void serialize(BinaryStream& stream) const;
    virtual void deserialize(BinaryStream& stream);
protected:
    UpgradePool();

    Upgrade<BigInt> m_testUpgrade;
    Upgrade<float> m_cellLifespanUpgrade;
};

#endif // UPGRADEPOOL_H
