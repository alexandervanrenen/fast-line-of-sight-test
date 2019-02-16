#include "Line.hpp"
#include <vector>
#include <algorithm>
// -------------------------------------------------------------------------------------
using namespace std;
// -------------------------------------------------------------------------------------
namespace unv {
// -------------------------------------------------------------------------------------
namespace {
const ub4 kPrecision = 3; // 3 decimal places are taken into account
const float kEpsilon = 1.0f / (kPrecision + 1);
}
// -------------------------------------------------------------------------------------
Line::Line(const Point<float> &a, const Point<float> &d)
        : a(a)
          , d(d)
{
   assert(d != Point<float>(0, 0));
}
//---------------------------------------------------------------------------
Line Line::CreateBetweenPoints(const Point<float> &start, const Point<float> &end)
{
   return Line(start, end - start);
}
//---------------------------------------------------------------------------
bool Line::IsParallel(const Line &o) const
{
   Point<float> new_d = d;
   Point<float> new_od = o.d;
   new_d.ScaleToLength(1.0f);
   new_od.ScaleToLength(1.0f);

   return new_d == new_od || (new_d * -1.0f) == new_od;
}
//---------------------------------------------------------------------------
bool Line::IsEpsilonEqual(const Line &o) const
{
   if (!IsParallel(o))
      return false;
   if (d.x == 0)
      return a.x - o.a.x<kEpsilon;
   if (d.y == 0)
      return a.y - o.a.y<kEpsilon;

   return (o.a.x - a.x) / d.x - (o.a.y - a.y) / d.y<kEpsilon;
}
// -------------------------------------------------------------------------------------
bool Line::CalculateGenericIntersect(float &alpha, float &beta, const Line &o) const
{
   // case 1: parallel
   alpha = 0;
   beta = 0;
   if (IsEpsilonEqual(o))
      return true;
   if (IsParallel(o))
      return false;

   // case 2: d.x==0
   if (d.x == 0) {
      // we know: d.x==0 => d.y!=0 (otherwise d==(0,0) which is invalid) && o.d.x!=0 (otherwise this and o are parallel)
      beta = (a.x - o.a.x) / o.d.x;
      alpha = (o.a.y - a.y + beta * o.d.y) / d.y;
      return true;
   }

   // case 3: o.d.x==0
   if (o.d.x == 0) {
      // we know: o.d.x==0 => o.d.y!=0 (otherwise o.d==(0,0) which is invalid) && d.x!=0 (otherwise this and o are parallel)
      alpha = (o.a.x - a.x) / d.x;
      beta = -(o.a.y - a.y - alpha * d.y) / o.d.y;
      return true;
   }

   // case 4: not any other case
   // we know: this and o are not parallel (case 1) && d.x!=0 (case 2) && o.d.x!=0 (case 3) => d.y*o.d.y-d.y*o.d.x != 0 (otherwise parallel)
   alpha = ((o.a.x - a.x) * o.d.y - (o.a.y - a.y) * o.d.x) / (d.x * o.d.y - d.y * o.d.x);
   beta = -(o.a.x - a.x - alpha * d.x) / o.d.x;
   return true;
}
// -------------------------------------------------------------------------------------
bool Line::DoLineSegmentsIntersect(const Line &other) const
{
   float alpha, beta;
   bool intersect = CalculateGenericIntersect(alpha, beta, other);
   return intersect && alpha>=0.0f && alpha<=1.0f && beta>=0.0f && beta<=1.0f;
}
namespace {
bool onSegment(const Point<float> &p, const Point<float> &q, const Point<float> &r)
{
   if (q.x<=max(p.x, r.x) && q.x>=min(p.x, r.x) && q.y<=max(p.y, r.y) && q.y>=min(p.y, r.y)) {
      return true;
   }

   return false;
}
ub4 orientation(const Point<float> &p, const Point<float> &q, const Point<float> &r)
{
   float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

   if (val == 0) {
      return 0;  // Co-linear
   }
   return val>0 ? 1 : 2; // Clock or counterclock wise
}
}
// -------------------------------------------------------------------------------------
bool Line::DoLineSegmentsIntersectWithRotation(const Line &other) const
{
   Point<float> p1 = this->a;
   Point<float> q1 = this->a + this->d;
   Point<float> p2 = other.a;
   Point<float> q2 = other.a + other.d;

   // Find the four orientations needed for general and special cases
   ub4 o1 = orientation(p1, q1, p2);
   ub4 o2 = orientation(p1, q1, q2);
   ub4 o3 = orientation(p2, q2, p1);
   ub4 o4 = orientation(p2, q2, q1);

   // General case
   if (o1 != o2 && o3 != o4)
      return true;

   // Special Cases
   // p1, q1 and p2 are colinear and p2 lies on segment p1q1
   if (o1 == 0 && onSegment(p1, p2, q1))
      return true;

   // p1, q1 and q2 are colinear and q2 lies on segment p1q1
   if (o2 == 0 && onSegment(p1, q2, q1))
      return true;

   // p2, q2 and p1 are colinear and p1 lies on segment p2q2
   if (o3 == 0 && onSegment(p2, p1, q2))
      return true;

   // p2, q2 and q1 are colinear and q1 lies on segment p2q2
   if (o4 == 0 && onSegment(p2, q1, q2))
      return true;

   return false; // Doesn't fall in any of the above cases
}
// -------------------------------------------------------------------------------------
bool Line::DoLineSegmentsIntersectInBetween(const Line &other) const
{
   float alpha, beta;
   bool intersect = CalculateGenericIntersect(alpha, beta, other);
   return intersect && alpha>0.0f && alpha<1.0f && beta>0.0f && beta<1.0f;
}
// -------------------------------------------------------------------------------------
const Point<float> Line::Evaluate(float alpha) const
{
   return Point<float>(a.x + d.x * alpha, a.y + d.y * alpha);
}
Line Line::RightAngleLine() const
{
   return Line(a, Point<float>(-d.y, d.x));
}
// -------------------------------------------------------------------------------------
vector<ub4> Line::SortAlongLine(vector<Point<float>> &points) const
{
   // init
   vector<pair<float, ub4> > result;
   Line rectLine = RightAngleLine();

   // calculate a scalar for each point describing its fitness
   for (ub4 i = 0; i<points.size(); i++) {
      rectLine.SetStartPoint(points[i]);
      float alpha, beta;
      CalculateGenericIntersect(alpha, beta, rectLine);
      result.push_back(make_pair(alpha, i));
   }

   // sort by fitness
   sort(result.begin(), result.end(), [](const pair<float, ub4> &lhs, const pair<float, ub4> &rhs) {
      return lhs.first<rhs.first;
   });

   // create permutation
   vector<ub4> permutation(points.size());
   for (ub4 i = 0; i<points.size(); i++)
      permutation[i] = result[i].second;

   return permutation;
}
// -------------------------------------------------------------------------------------
void Line::SetStartPoint(const Point<float> &p)
{
   a = p;
}
// -------------------------------------------------------------------------------------
const Point<float> Line::GetStartPoint() const
{
   return a;
}
// -------------------------------------------------------------------------------------
void Line::SetDirection(const Point<float> &p)
{
   d = p;
}
// -------------------------------------------------------------------------------------
const Point<float> Line::GetDirection() const
{
   return d;
}
// -------------------------------------------------------------------------------------
const Point<float> Line::GetEndPoint() const
{
   return a + d;
}
// -------------------------------------------------------------------------------------
const std::string Line::ToString() const
{
   return "(" + GetStartPoint().ToString() + ", " + GetEndPoint().ToString() + ")";
}
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------