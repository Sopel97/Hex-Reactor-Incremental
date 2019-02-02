template <typename IntType>
RandomEngineT<IntType>::RandomEngineT()
{

}
template <typename IntType>
RandomEngineT<IntType>::~RandomEngineT()
{

}
template <typename IntType>
RandomEngineT<IntType>::RandomEngineT(IntType seed, IntType max) :
    m_seed(seed),
    m_max(max)
{

}
template <typename IntType>
IntType RandomEngineT<IntType>::nextInt(IntType rangeFirst, IntType rangeLast) // <rangeFirst, rangeLast>
{
    return nextRaw() % (rangeLast - rangeFirst + IntType(1)) + rangeFirst;
}
template <typename IntType>
float RandomEngineT<IntType>::nextFloat(float rangeFirst, float rangeLast) // <rangeFirst, rangeLast) (if m_max can't fit in int type used for nextRaw() then it's <rangeFirst, rangeLast>
{
    return nextRaw() / float(m_max) * (rangeLast - rangeFirst) + rangeFirst;
}
template <typename IntType>
double RandomEngineT<IntType>::nextDouble(double rangeFirst, double rangeLast) // <rangeFirst, rangeLast) (if m_max can't fit in int type used for nextRaw() then it's <rangeFirst, rangeLast>
{
    return nextRaw() / double(m_max) * (rangeLast - rangeFirst) + rangeFirst;
}

template <typename IntType>
IntType RandomEngineT<IntType>::nextInt() //range defined by IntType size and generator
{
    return nextRaw();
}
template <typename IntType>
float RandomEngineT<IntType>::nextFloat() // <0, 1) (if m_max can't fit in int type used for nextRaw() then it's <0, 1>
{
    return nextRaw() / float(m_max);
}
template <typename IntType>
double RandomEngineT<IntType>::nextDouble() // <0, 1) (if m_max can't fit in int type used for nextRaw() then it's <0, 1>
{
    return nextRaw() / double(m_max);
}
template <typename IntType>
bool RandomEngineT<IntType>::nextBool() //0 or 1
{
    return nextRaw() > (m_max >> 1);
}

template <typename IntType>
IntType RandomEngineT<IntType>::seed() const
{
    return m_seed;
}
template <typename IntType>
IntType RandomEngineT<IntType>::max() const //max+1 or max if max+1 dont fir in IntType
{
    return m_max;
}
