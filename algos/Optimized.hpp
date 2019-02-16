#pragma once
// -------------------------------------------------------------------------------------
#include "util/Line.hpp"
#include <vector>
// -------------------------------------------------------------------------------------
namespace unv {
// -------------------------------------------------------------------------------------
struct Optimized {
   const std::vector<std::vector<bool>> &grid; // FALSE -> no line of sight

   Optimized(const std::vector<std::vector<bool>> &grid)
           : grid(grid) {}

   bool HasLineOfSight(const Point<float> &start_in, const Point<float> &goal_in) const
   {
      // Sawp so that we are always moving right
      Point<float> start = start_in;
      Point<float> goal = goal_in;
      if (start_in.x>goal_in.x) {
         start = goal_in;
         goal = start_in;
      }

      Point<sb4> cur_block = start.RoundSb4();
      const Point<sb4> goal_block = goal.RoundSb4();

      sb4 x_dir;
      sb4 y_dir;
      Point<float> offset;
      Point<sb4> update_clockwise;
      Point<sb4> update_counter_clockwise;

      if (start.x<=goal.x && start.y<=goal.y) {
         // Up right
         x_dir = 1;
         y_dir = 1;
         offset = Point<float>(0.5f, 0.5f);
         update_clockwise = Point<sb4>(0, 1);
         update_counter_clockwise = Point<sb4>(1, 0);
      } else {
         // Down right
         x_dir = 1;
         y_dir = -1;
         offset = Point<float>(0.5f, -0.5f);
         update_clockwise = Point<sb4>(1, 0);
         update_counter_clockwise = Point<sb4>(0, -1);
      }

      while (cur_block.x != goal_block.x && cur_block.y != goal_block.y) {
         // Wall
         if (!grid[cur_block.x][cur_block.y]) {
            return false;
         }

         // Move to next block
         ub4 rotation = TripletOrientation(start, goal, cur_block.ToFloat() + offset);
         if (rotation == CLOCKWISE) {
            cur_block += update_clockwise;
         } else {
            cur_block += update_counter_clockwise;
         }
      }

      while (cur_block.x != goal_block.x) {
         if (!grid[cur_block.x][cur_block.y]) {
            return false;
         }
         cur_block.x += x_dir;
      }

      while (cur_block.y != goal_block.y) {
         if (!grid[cur_block.x][cur_block.y]) {
            return false;
         }
         cur_block.y += y_dir;
      }

      return grid[cur_block.x][cur_block.y];
   }

private:
   static const ub4 CLOCKWISE = 1;
   static const ub4 COUNTER_CLOCKWISE = 2; // Or co-linear, but me no care

   static inline ub4 TripletOrientation(const Point<float> &p, const Point<float> &q, const Point<float> &r)
   {
      float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
      return val>0 ? CLOCKWISE : COUNTER_CLOCKWISE;
   }
};
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------
