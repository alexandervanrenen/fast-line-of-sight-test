#pragma once
// -------------------------------------------------------------------------------------
#include "util/Line.hpp"
#include "Textbook.hpp"
#include <vector>
// -------------------------------------------------------------------------------------
namespace unv {
// -------------------------------------------------------------------------------------
struct Stef {
   const std::vector<std::vector<bool>> &grid; // FALSE -> no line of sight

   Stef(const std::vector<std::vector<bool>> &grid)
           : grid(grid)
             , textbook(grid) {}

   bool HasLineOfSight(const Point<float> &start, const Point<float> &goal) const
   {
      assert(start.x<=goal.x && start.y>=goal.y);

      Point<float> vec = goal - start;
      Point<float> delta;
      Point<float> cur;
      float slope = vec.y / vec.x;

      if (!grid[start.x][start.y] || !grid[goal.x][goal.y]) {
         return false;
      }

      if (slope>=-1.0f) {
         delta.x = 1.0f;
         delta.y = slope;
         cur.x = ceil(start.x);
         cur.y = start.y + (cur.x - start.x) * delta.y;

         while (cur.x<goal.x) {
            // Check left and right neighbor
            if (!grid[floor(cur.x - 0.5f)][floor(cur.y)] || !grid[floor(cur.x + 0.5f)][floor(cur.y)]) {
               return false;
            }

            // Advance
            cur += delta;
         }
      } else {
         delta.x = vec.x / std::abs(vec.y);
         delta.y = -1.0f;
         cur.y = floor(start.y);
         cur.x = start.x + (start.y - cur.y) * delta.x;

         while (cur.y>goal.y) {
            // Check top and bottom neighbor
            if (!grid[floor(cur.x)][floor(cur.y - 0.5f)] || !grid[floor(cur.x)][floor(cur.y + 0.5f)]) {
               return false;
            }

            // Advance
            cur += delta;
         }
      }

      return true;
   }

   Textbook textbook;
};
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------
