#include <cstdlib>
using namespace std;

template<typename T>
inline T TRnd(const T& x)
{
	return static_cast<T>(rand() % (x)) + 1;
}
template<typename T>
inline T TRund(const T& x)
{
	return static_cast<T>(rand() % (x));
}
template<typename T>
inline T TMathMin(const T& x, const T& y)
{
	return x > y ? y : x;
}
template<typename T>
inline T TMathMax(const T& x, const T& y)
{
	return x > y ? x : y;
}
