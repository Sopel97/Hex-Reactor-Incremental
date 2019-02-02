#ifndef RandomEngineT_H_INCLUDED
#define RandomEngineT_H_INCLUDED

template <typename IntType>
class RandomEngineT
{
public:
    typedef IntType GeneratedValuesType;
    RandomEngineT();
    RandomEngineT(IntType seed, IntType max);
    ~RandomEngineT();
    virtual IntType nextRaw() = 0;

    virtual IntType nextInt(IntType rangeFirst, IntType rangeLast);
    virtual float nextFloat(float rangeFirst, float rangeLast);
    virtual double nextDouble(double rangeFirst, double rangeLast);

    virtual IntType nextInt();
    virtual float nextFloat();
    virtual double nextDouble();
    virtual bool nextBool();

    virtual IntType seed() const;
    virtual IntType max() const;
protected:
    IntType m_seed;
    IntType m_max;
};
typedef RandomEngineT<> RandomEngine;
typedef RandomEngineT<uint64_t> RandomEngine64;

#include "../src/RandomEngine.cpp"

#endif // RandomEngineT_H_INCLUDED
