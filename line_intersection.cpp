#include "util/Line.hpp"
#include "util/FastRandom.hpp"
#include "util/Utility.hpp"
#include <iostream>
#include <set>
#include <chrono>
#include <vector>

using namespace std;
using namespace unv;

Point<float> CreateRandPoint(FastRandom &ranny)
{
   return Point<float>(ranny.randf(-10.0f, 10.0f), ranny.randf(-10.0f, 10.0f));
}

int main(int, char **)
{
   FastRandom ranny;
   const ub8 COUNT = 10'000'000;

   // Validate that both give the same result
   for (ub8 i = 0; i<COUNT; ++i) {
      Line l1 = Line::CreateBetweenPoints(CreateRandPoint(ranny), CreateRandPoint(ranny));
      Line l2 = Line::CreateBetweenPoints(CreateRandPoint(ranny), CreateRandPoint(ranny));
      if (l1.DoLineSegmentsIntersect(l2) != l1.DoLineSegmentsIntersectWithRotation(l2)) {
         cout << "i: " << i << endl;
         cout << "l1: " << l1.ToString() << endl;
         cout << "l2: " << l2.ToString() << endl;
         cout << "normal: " << (l1.DoLineSegmentsIntersect(l2) ? "yes" : "no") << endl;
         cout << "rotation: " << (l1.DoLineSegmentsIntersectWithRotation(l2) ? "yes" : "no") << endl;
      }
   }

   // Check performance for rotating version
   {
      auto begin = chrono::high_resolution_clock::now();
      for (ub8 i = 0; i<COUNT; ++i) {
         Line l1 = Line::CreateBetweenPoints(CreateRandPoint(ranny), CreateRandPoint(ranny));
         Line l2 = Line::CreateBetweenPoints(CreateRandPoint(ranny), CreateRandPoint(ranny));
         DoNotOptimize(l1.DoLineSegmentsIntersectWithRotation(l2));
      }
      auto end = chrono::high_resolution_clock::now();
      cout << "rotation: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / COUNT << "ns" << endl;
   }

   // Check performance for textbook version
   {
      auto begin = chrono::high_resolution_clock::now();
      for (ub8 i = 0; i<COUNT; ++i) {
         Line l1 = Line::CreateBetweenPoints(CreateRandPoint(ranny), CreateRandPoint(ranny));
         Line l2 = Line::CreateBetweenPoints(CreateRandPoint(ranny), CreateRandPoint(ranny));
         DoNotOptimize(l1.DoLineSegmentsIntersect(l2));
      }
      auto end = chrono::high_resolution_clock::now();
      cout << "normal: " <<chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / COUNT << "ns" << endl;
   }
}
