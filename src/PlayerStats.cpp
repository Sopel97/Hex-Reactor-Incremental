#include "PlayerStats.h"
#include "BinaryStream.h"

PlayerStats::PlayerStats() :
    m_money(32333)
{
}

const BigInt& PlayerStats::money() const
{
    return m_money;
}
void PlayerStats::addMoney(const BigInt& amount)
{
    m_money += amount;
}

void PlayerStats::serialize(BinaryStream& stream) const
{
    stream << m_money;
}
void PlayerStats::deserialize(BinaryStream& stream)
{
    stream >> m_money;
}
