#pragma once
// -------------------------------------------------------------------------------------
#include "Utility.hpp"
#include <cmath>
#include <functional>
#include <string>
// -------------------------------------------------------------------------------------
namespace unv {
// -------------------------------------------------------------------------------------
template<class T>
struct Point {
   T x;
   T y;

   Point()
           : x(0)
             , y(0) {}
   Point(T x, T y)
           : x(x)
             , y(y) {}

   bool operator==(const Point<T> &o) const { return x == o.x && y == o.y; }
   bool operator!=(const Point<T> &o) const { return x != o.x || y != o.y; }

   Point<float> ToFloat() const { return Point<float>(x, y); }
   Point<ub4> RoundUb4() const;
   Point<sb4> RoundSb4() const;

   const std::string ToString() const;

   float Length() const { return Distance(Point(0, 0)); }
   float Distance(const Point<float> &other) const;
   T ManhattanDistance(const Point<T> &other) const { return std::abs(x - other.x) + std::abs(y - other.y); }

   // Create new object and allow to use return value
   friend Point operator+(const Point &lhs, const Point &rhs) { return Point(lhs.x + rhs.x, lhs.y + rhs.y); }
   friend Point operator-(const Point &lhs, const Point &rhs) { return Point(lhs.x - rhs.x, lhs.y - rhs.y); }
   friend Point operator*(const Point &lhs, float rhs) { return Point(lhs.x * rhs, lhs.y * rhs); }
   Point operator-() { return Point(-x, -y); }

   // Modify this object. Return void, as these should not be assigned !
   void operator+=(const Point &o) { *this = Point(x + o.x, y + o.y); }
   void operator-=(const Point &o) { *this = Point(x - o.x, y - o.y); }
   void operator*=(float scale) { *this = Point(x * scale, y * scale); }

   void Rotate90Left();
   void Rotate90Right();

   void MoveTowards(const Point<float> &target, const Point<float> &step);
   const Point<T> &ScaleToLength(float length);

   static ub4 TripletOrientation(const Point<float> &p, const Point<float> &q, const Point<float> &r);
};
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------
namespace std {
// -------------------------------------------------------------------------------------
template<class T>
struct hash<unv::Point<T>> {
   constexpr size_t operator()(const unv::Point<T> &p) const
   {
      return static_cast<size_t>(p.x + p.y * 2017);
   }
};
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------
