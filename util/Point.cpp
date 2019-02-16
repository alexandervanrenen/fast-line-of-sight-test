#include "Point.hpp"
#include <cmath>
#include <cstdlib>
#include <sstream>
// -------------------------------------------------------------------------------------
using namespace std;
// -------------------------------------------------------------------------------------
namespace unv {
// -------------------------------------------------------------------------------------
template<class T>
Point<sb4> Point<T>::RoundSb4() const
{
   return Point<sb4>(lround(x), lround(y));
}
// -------------------------------------------------------------------------------------
template<class T>
float Point<T>::Distance(const Point<float> &other) const
{
   return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}
// -------------------------------------------------------------------------------------
template<class T>
void Point<T>::Rotate90Left()
{
   swap(x, y);
   x = -x;
}
// -------------------------------------------------------------------------------------
template<class T>
void Point<T>::Rotate90Right()
{
   swap(x, y);
   y = -y;
}
// -------------------------------------------------------------------------------------
template<class T>
const string Point<T>::ToString() const
{
   ostringstream os;
   os << '(' << x << ',' << y << ')';
   return os.str();
}
// -------------------------------------------------------------------------------------
template<class T>
void Point<T>::MoveTowards(const Point<float> &target, const Point<float> &step)
{
   if (abs(x - target.x)<=abs(step.x)) {
      x = target.x;
   } else {
      x += step.x;
   }

   if (abs(y - target.y)<=abs(step.y)) {
      y = target.y;
   } else {
      y += step.y;
   }
}
// -------------------------------------------------------------------------------------
template<class T>
const Point<T> &Point<T>::ScaleToLength(float intended)
{
   float current = Length();
   if (current>0.0f) {
      float scale_factor = intended / current;
      *this *= scale_factor;
   }
   return *this;
}
// -------------------------------------------------------------------------------------
// Code from: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
template<class T>
ub4 Point<T>::TripletOrientation(const Point<float> &p, const Point<float> &q, const Point<float> &r)
{
   float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

   if (val == 0) {
      return 0;  // Co-linear
   }
   return val>0 ? 1 : 2; // Clock or counterclock wise
}
// -------------------------------------------------------------------------------------
template float Point<float>::Distance(const Point<float> &) const;
template void Point<float>::Rotate90Left();
template void Point<sb4>::Rotate90Left();
template void Point<float>::Rotate90Right();
template void Point<sb4>::Rotate90Right();
template Point<sb4> Point<float>::RoundSb4() const;
template const string Point<float>::ToString() const;
template const string Point<ub4>::ToString() const;
template const string Point<sb4>::ToString() const;
template const Point<float> &Point<float>::ScaleToLength(float);
template ub4 Point<float>::TripletOrientation(const Point<float> &p, const Point<float> &q, const Point<float> &r);
template void Point<float>::MoveTowards(const Point<float> &, const Point<float> &);
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------