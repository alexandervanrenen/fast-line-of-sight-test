#pragma once
// -------------------------------------------------------------------------------------
#include <cstdint>
#include <cassert>
// -------------------------------------------------------------------------------------
using ub1 = uint8_t;
using ub2 = uint16_t;
using ub4 = uint32_t;
using ub8 = uint64_t;
// -------------------------------------------------------------------------------------
using sb1 = int8_t;
using sb2 = int16_t;
using sb4 = int32_t;
using sb8 = int64_t;
// -------------------------------------------------------------------------------------
// BENCHMARK_ALWAYS_INLINE macro
// See: https://github.com/google/benchmark
#if defined(__GNUC__)
# define BENCHMARK_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER) && !defined(__clang__)
# define BENCHMARK_ALWAYS_INLINE __forceinline
#else
# define BENCHMARK_ALWAYS_INLINE
#endif
// -------------------------------------------------------------------------------------
// The DoNotOptimize(...) function can be used to prevent a value or
// expression from being optimized away by the compiler. This function is
// intented to add little to no overhead.
// See: http://stackoverflow.com/questions/28287064
// See: https://github.com/google/benchmark
// Note: does not work everywhere (see todos in original post)
#if defined(__GNUC__)
template<class Tp>
inline BENCHMARK_ALWAYS_INLINE void DoNotOptimize(Tp const &value)
{
   asm volatile("" : "+m" (const_cast<Tp &>(value)));
}
#else
template<class Tp>
inline BENCHMARK_ALWAYS_INLINE void DoNotOptimize(Tp const &value)
{
   internal::UseCharPointer(&reinterpret_cast<char const volatile &>(value));
}
#endif
// -------------------------------------------------------------------------------------
