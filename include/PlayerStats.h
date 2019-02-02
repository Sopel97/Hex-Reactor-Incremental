#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H

#include "Bignum.h"
#include "Serializable.h"

class BinaryStream;

class PlayerStats : public Serializable
{
public:
    const BigInt& money() const;
    void addMoney(const BigInt& amount);
    PlayerStats();

    virtual void serialize(BinaryStream& stream) const;
    virtual void deserialize(BinaryStream& stream);
protected:
    BigInt m_money;
};

#endif // PLAYERSTATS_H
