#pragma once
// -------------------------------------------------------------------------------------
#include "util/Line.hpp"
#include "Textbook.hpp"
#include <vector>
// -------------------------------------------------------------------------------------
namespace unv {
// -------------------------------------------------------------------------------------
struct SmartMarch {
   const std::vector<std::vector<bool>> &grid; // FALSE -> no line of sight

   SmartMarch(const std::vector<std::vector<bool>> &grid)
           : grid(grid)
             , textbook(grid) {}

   bool HasLineOfSight(const Point<float> &start, const Point<float> &goal) const
   {
      assert(start.x<=goal.x && start.y>=goal.y);

      Point<float> vec = goal - start;
      float slope = vec.y / vec.x;

      if (!grid[start.x][start.y] || !grid[goal.x][goal.y]) {
         return false;
      }

      if (slope>=-1.0f) {
         float delta_y_float = slope;
         sb8 delta_y = delta_y_float * (sb8(1) << 32);
         sb4 cur_x = std::max((sb4) ceil(start.x), 1);
         sb8 cur_y = (start.y + (cur_x - start.x) * delta_y_float) * (sb8(1) << 32);
         sb4 limit_x = floor(goal.x);

         for (; cur_x<=limit_x; cur_x++) {
            // Check left and right neighbor
            sb4 real_cur_y = (cur_y >> 32);
            assert(real_cur_y<=grid.size());
            if (!grid[cur_x - 1][real_cur_y] || !grid[cur_x][real_cur_y]) {
               return false;
            }

            // Advance
            cur_y += delta_y;
         }
      } else {
         float delta_x_float = vec.x / std::abs(vec.y);
         sb8 delta_x = delta_x_float * (sb8(1) << 32);
         sb4 cur_y = floor(start.y);
         sb8 cur_x = (start.x + (start.y - cur_y) * delta_x_float) * (sb8(1) << 32);
         sb4 limit_y = std::min((sb4) ceil(goal.y), (sb4) grid.size() - 1);

         for (; cur_y>=limit_y; cur_y--) {
            // Check top and bottom neighbor
            sb4 real_cur_x = (cur_x >> 32);
            assert(real_cur_x<=grid.size());
            if (!grid[real_cur_x][cur_y - 1] || !grid[real_cur_x][cur_y]) {
               return false;
            }

            // Advance
            cur_x += delta_x;
         }
      }

      return true;
   }

   Textbook textbook;
};
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------
