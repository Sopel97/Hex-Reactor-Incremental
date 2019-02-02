#ifndef XORSHIFT64ENGINE_H_INCLUDED
#define XORSHIFT64ENGINE_H_INCLUDED

template <int I>
class Xorshift64EngineT : public RandomEngineT<uint64_t>
{
public:
    typedef uint64_t GeneratedValuesType;
    Xorshift64EngineT(uint64_t seed = 0xb5f3c6a7);
    ~Xorshift64EngineT();
    virtual uint64_t nextRaw();
protected:
    uint64_t x;
};
typedef Xorshift64EngineT<> Xorshift64Engine;

#include "../src/Xorshift64Engine.cpp"

#endif // XORSHIFT64ENGINE_H_INCLUDED
