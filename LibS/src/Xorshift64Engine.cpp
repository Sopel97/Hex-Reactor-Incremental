template <int I>
Xorshift64EngineT<I>::Xorshift64EngineT(uint64_t seed) :
    RandomEngineT<uint64_t>(seed, 0xffffffffffffffffULL)
{
    x = seed;
}
template <int I>
Xorshift64EngineT<I>::~Xorshift64EngineT()
{

}
template <int I>
uint64_t Xorshift64EngineT<I>::nextRaw()
{
	x ^= x >> 12; // a
	x ^= x << 25; // b
	x ^= x >> 27; // c
	return x * 2685821657736338717ULL;
}
