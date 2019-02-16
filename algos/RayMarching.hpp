#pragma once
// -------------------------------------------------------------------------------------
#include "util/Line.hpp"
#include <vector>
// -------------------------------------------------------------------------------------
namespace unv {
// -------------------------------------------------------------------------------------
struct RayMarching {
   const std::vector<std::vector<bool>> &grid; // FALSE -> no line of sight

   RayMarching(const std::vector<std::vector<bool>> &grid)
           : grid(grid) {}

   bool HasLineOfSight(const Line &line) const
   {
      const ub4 precision = 10; // Defined in samples per block
      Point<float> start = line.GetStartPoint();
      Point<float> cur = line.GetStartPoint();
      Point<float> goal = line.GetEndPoint();
      Point<sb4> goal_block = goal.RoundSb4();
      float distance = cur.Distance(goal);

      // Additative solution
      if (start.x<=goal.x && start.y<=goal.y) {
         // Left up
         Point<float> delta = (goal - cur);
         delta.ScaleToLength(1.0f / precision);
         while (cur.x<goal.x && cur.y<goal.y) {
            Point<sb4> cur_block = cur.RoundSb4();
            if (!grid[cur_block.x][cur_block.y]) {
               return false;
            }
            cur += delta;
         }
      } else if (start.x<=goal.x) {
         // Left down
         Point<float> delta = (goal - cur);
         delta.ScaleToLength(1.0f / precision);
         while (cur.x<goal.x && cur.y>goal.y) {
            Point<sb4> cur_block = cur.RoundSb4();
            if (!grid[cur_block.x][cur_block.y]) {
               return false;
            }
            cur += delta;
         }
      } else if (start.y<=goal.y) {
         // Up left
         Point<float> delta = (goal - cur);
         delta.ScaleToLength(1.0f / precision);
         while (cur.x>goal.x && cur.y<goal.y) {
            Point<sb4> cur_block = cur.RoundSb4();
            if (!grid[cur_block.x][cur_block.y]) {
               return false;
            }
            cur += delta;
         }
      } else {
         // Down left
         Point<float> delta = (goal - cur);
         delta.ScaleToLength(1.0f / precision);
         while (cur.x>goal.x && cur.y>goal.y) {
            Point<sb4> cur_block = cur.RoundSb4();
            if (!grid[cur_block.x][cur_block.y]) {
               return false;
            }
            cur += delta;
         }
      }

      return true;
   }
};
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------
