#pragma once
// -------------------------------------------------------------------------------------
#include "Utility.hpp"
#include "Point.hpp"
#include <cstdint>
#include <limits>
// -------------------------------------------------------------------------------------
namespace unv {
// -------------------------------------------------------------------------------------
// Based on: https://en.wikipedia.org/wiki/Xorshift
class FastRandom {
public:
   explicit FastRandom(ub8 seed = 2305843008139952128ull); // The 8th perfect number found 1772 by Euler with <3
   ub4 rand();
   sb4 srand();
   ub4 rand(ub4 min, ub4 max);
   sb4 srand(sb4 min, sb4 max);
   float randf(float min, float max);
   float ScaleFactor(); // returns float between 0 and 1
   bool Bool();

   ub8 seed;

   static constexpr ub8 min() { return 0; }
   static constexpr ub8 max() { return std::numeric_limits<ub8>::max(); }
};
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------
