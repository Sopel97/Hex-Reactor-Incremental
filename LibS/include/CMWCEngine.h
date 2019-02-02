#ifndef CMWCENGINE_H_INCLUDED
#define CMWCENGINE_H_INCLUDED

template <int I>
class CMWCEngineT : public RandomEngineT<uint32_t> //with lag of 4096 //find better version
{
public:
    typedef uint32_t GeneratedValuesType;
    CMWCEngineT(uint32_t seed = 0xb5f3c6a7);
    ~CMWCEngineT();
    virtual uint32_t nextRaw(); /* generates 16 bit number */

    //uint32_t nextInt(uint32_t rangeFirst, uint32_t rangeLast); /* generates int in range <rangeFirst, rangeLast> */
    //float nextFloat(float rangeFirst, float rangeLast); /* generates float in range <rangeFirst, rangeLast) */
    //double nextDouble(double rangeFirst, double rangeLast); /* generates double in range <rangeFirst, rangeLast) */

    //uint32_t nextInt(); /* generates raw 16 bit value */
    //float nextFloat(); /* generates float in range <0,1) */
    //double nextDouble(); /* generates double in range <0,1) */
    //bool nextBool(); /* generates bool */
protected:
    static const int PHI = 0x9e3779b9;
    uint32_t Q[4096];
    uint32_t c;
};
typedef CMWCEngineT<> CMWCEngine;

#include "../src/CMWCEngine.cpp"
#endif // CMWCENGINE_H_INCLUDED
