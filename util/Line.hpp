#pragma once
// -------------------------------------------------------------------------------------
#include "Utility.hpp"
#include "Point.hpp"
#include <string>
#include <vector>
// -------------------------------------------------------------------------------------
namespace unv {
// -------------------------------------------------------------------------------------
class Line {
public:
   // Ctor, the direction of a line may not be (0,0)
   Line() = default;
   Line(const Point<float> &start, const Point<float> &direction);
   static Line CreateBetweenPoints(const Point<float> &start, const Point<float> &end);

   // Compare lines
   bool IsParallel(const Line &other) const;
   bool IsEpsilonEqual(const Line &other) const;

   // Alpha and beta are set to the scalars needed to calculate the intersection point
   // If the lines are equal alpha==beta==0
   bool CalculateGenericIntersect(float &alpha, float &beta, const Line &other) const;
   // Treat both lines as line segments and check for an intersection
   bool DoLineSegmentsIntersect(const Line &other) const;
   bool DoLineSegmentsIntersectWithRotation(const Line &other) const; // Same as above but different algo
   // Treat both lines as line segments and check for an intersection
   // Only intersections that are not on the start or end point are counted
   bool DoLineSegmentsIntersectInBetween(const Line &other) const;

   // Calculates a point on the line
   const Point<float> Evaluate(float alpha) const;

   // Print
   const std::string ToString() const;

   // Get line with right angle
   Line RightAngleLine() const;

   // Sort points as if they where on the line
   std::vector<uint32_t> SortAlongLine(std::vector<Point<float> > &points) const;

   // Setter and getter
   void SetStartPoint(const Point<float> &p);
   const Point<float> GetStartPoint() const;
   void SetDirection(const Point<float> &p);
   const Point<float> GetDirection() const;
   const Point<float> GetEndPoint() const;

private:
   Point<float> a;
   Point<float> d;
};
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------
