#include "util/Line.hpp"
#include "util/FastRandom.hpp"
#include "util/Utility.hpp"
#include "algos/Rotation.hpp"
#include "algos/Textbook.hpp"
#include "algos/RayMarching.hpp"
#include "algos/Optimized.hpp"
#include "algos/Stef.hpp"
#include "algos/StefOpt.hpp"
#include <iostream>
#include <set>
#include <chrono>

using namespace std;
using namespace unv;

const ub8 COUNT = 100'000;
const ub4 GRID_SIZE = 100;

Line CreateRandLine(FastRandom &ranny)
{
   Point<float> start = Point<float>(ranny.randf(0, GRID_SIZE - 1), ranny.randf(0, GRID_SIZE - 1));
   Point<float> end = Point<float>(ranny.randf(start.x, GRID_SIZE - 1), ranny.randf(0, start.y));
   return Line::CreateBetweenPoints(start, end);
}

int main(int, char **)
{
   FastRandom ranny;

   // Create grid
   vector<vector<bool>> grid(GRID_SIZE, vector<bool>(GRID_SIZE, true));
   for (ub4 x = 0; x<GRID_SIZE; x++) {
      for (ub4 y = 0; y<GRID_SIZE; y++) {
         if (ranny.rand(0, 100) == 0) {
            grid[x][y] = false;
         }
      }
   }
   Rotation rotation(grid);
   Textbook textbook(grid);
   RayMarching ray_marching(grid);
   Optimized optimized(grid);
   Stef stef(grid);
   StefOpt stef_opt(grid);

   // Create lines
   vector<Line> lines(COUNT);
   for (Line &line : lines) {
      line = CreateRandLine(ranny);
   }

   // Validate that all algos give the same result
   ub4 ray_error_count = 0;
   ub4 rotation_error_count = 0;
   ub4 stef_error_count = 0;
   ub4 stef_opt_error_count = 0;
   ub4 optimized_error_count = 0;
   ub8 total_distance = 0;
   for (ub8 i = 0; i<COUNT; ++i) {
      Line l1 = CreateRandLine(ranny);
      total_distance += l1.GetStartPoint().ManhattanDistance(l1.GetEndPoint());
      bool textbook_sees_it = textbook.HasLineOfSight(l1.GetStartPoint(), l1.GetEndPoint());
      bool rotation_sees_it = rotation.HasLineOfSight(l1.GetStartPoint(), l1.GetEndPoint());
      bool stef_sees_it = stef.HasLineOfSight(l1.GetStartPoint() + Point<float>(0.5f, 0.5f), l1.GetEndPoint() + Point<float>(0.5f, 0.5f));
      bool stef_opt_sees_it = stef_opt.HasLineOfSight(l1.GetStartPoint() + Point<float>(0.5f, 0.5f), l1.GetEndPoint() + Point<float>(0.5f, 0.5f));
      //      bool ray_marching_sees_it = ray_marching.HasLineOfSight(l1);
      bool optimized_sees_it = optimized.HasLineOfSight(l1.GetStartPoint(), l1.GetEndPoint());
      //      if (textbook_sees_it != ray_marching_sees_it) {
      //         ray_error_count++;
      //      }
      if (textbook_sees_it != rotation_sees_it) {
         rotation_error_count++;
      }
      if (textbook_sees_it != optimized_sees_it) {
         optimized_error_count++;
      }
      if (textbook_sees_it != stef_sees_it) {
         stef_error_count++;
      }
      if (textbook_sees_it != stef_opt_sees_it) {
         stef_opt_error_count++;
      }
   }
   cout << "ray_error_count: " << ray_error_count << endl;
   cout << "rotation_error_count: " << rotation_error_count << endl;
   cout << "stef_error_count: " << stef_opt_error_count << endl;
   cout << "stef_opt_error_count: " << stef_opt_error_count << endl;
   cout << "optimized_error_count: " << optimized_error_count << endl;

   // Benchmark textbook algo
   {
      auto begin = chrono::high_resolution_clock::now();
      for (ub8 i = 0; i<COUNT; ++i) {
         DoNotOptimize(textbook.HasLineOfSight(lines[i].GetStartPoint(), lines[i].GetEndPoint()));
      }
      auto end = chrono::high_resolution_clock::now();
      cout << "textbook: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / COUNT << "ns" << endl;
   }

   // Benchmark rotation algo
   {
      auto begin = chrono::high_resolution_clock::now();
      for (ub8 i = 0; i<COUNT; ++i) {
         DoNotOptimize(rotation.HasLineOfSight(lines[i].GetStartPoint(), lines[i].GetEndPoint()));
      }
      auto end = chrono::high_resolution_clock::now();
      cout << "rotation: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / COUNT << "ns" << endl;
   }

   // Benchmark ray_marching algo
   //   {
   //      auto begin = chrono::high_resolution_clock::now();
   //      for (ub8 i = 0; i<COUNT; ++i) {
   //         DoNotOptimize(ray_marching.HasLineOfSight(lines[i]));
   //      }
   //      auto end = chrono::high_resolution_clock::now();
   //      cout << "ray_marching: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / COUNT << "ns" << endl;
   //   }

   // Benchmark optimized algo
   {
      auto begin = chrono::high_resolution_clock::now();
      for (ub8 i = 0; i<COUNT; ++i) {
         DoNotOptimize(optimized.HasLineOfSight(lines[i].GetStartPoint(), lines[i].GetEndPoint()));
      }
      auto end = chrono::high_resolution_clock::now();
      cout << "optimized: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / COUNT << "ns" << endl;
   }

   // Benchmark stef algo
   {
      auto begin = chrono::high_resolution_clock::now();
      for (ub8 i = 0; i<COUNT; ++i) {
         DoNotOptimize(stef.HasLineOfSight(lines[i].GetStartPoint() + Point<float>(0.5f, 0.5f), lines[i].GetEndPoint() + Point<float>(0.5f, 0.5f)));
      }
      auto end = chrono::high_resolution_clock::now();
      cout << "stef: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / COUNT << "ns" << endl;
   }

   // Benchmark stef_opt algo
   {
      auto begin = chrono::high_resolution_clock::now();
      for (ub8 i = 0; i<COUNT; ++i) {
         DoNotOptimize(stef_opt.HasLineOfSight(lines[i].GetStartPoint() + Point<float>(0.5f, 0.5f), lines[i].GetEndPoint() + Point<float>(0.5f, 0.5f)));
      }
      auto end = chrono::high_resolution_clock::now();
      cout << "stef_opt: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / COUNT << "ns" << endl;
   }
}
