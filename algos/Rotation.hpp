#pragma once
// -------------------------------------------------------------------------------------
#include "util/Line.hpp"
#include <vector>
// -------------------------------------------------------------------------------------
namespace unv {
// -------------------------------------------------------------------------------------
struct Rotation {
   const std::vector<std::vector<bool>> &grid; // FALSE -> no line of sight

   Rotation(const std::vector<std::vector<bool>> &grid)
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

      sb4 x_dir;
      sb4 y_dir;
      Point<sb4> horizontal_udpate;
      Point<sb4> vertical_udpate;
      Point<float> block_update_1;
      Point<float> block_update_2;

      if (start.x<=goal.x && start.y<=goal.y) {
         // Up right
         x_dir = 1;
         y_dir = 1;
         horizontal_udpate = Point<sb4>(1, 0);
         vertical_udpate = Point<sb4>(0, 1);
         block_update_1 = Point<float>(0.5f, -0.5f);
         block_update_2 = Point<float>(0.5f, 0.5f);
      } else {
         // Down right
         x_dir = 1;
         y_dir = -1;
         horizontal_udpate = Point<sb4>(1, 0);
         vertical_udpate = Point<sb4>(0, -1);
         block_update_1 = Point<float>(0.5f, -0.5f);
         block_update_2 = Point<float>(0.5f, 0.5f);
      }

      Line line = Line::CreateBetweenPoints(start, goal);
      Point<sb4> cur_block = start.RoundSb4();
      Point<sb4> goal_block = goal.RoundSb4();

      while (cur_block.x != goal_block.x && cur_block.y != goal_block.y) {
         // Collision check
         if (!grid[cur_block.x][cur_block.y]) {
            return false;
         }

         // Move to next block
         Line horizontal_border = Line::CreateBetweenPoints(cur_block.ToFloat() + block_update_1, cur_block.ToFloat() + block_update_2);
         if (line.DoLineSegmentsIntersectWithRotation(horizontal_border)) {
            cur_block += horizontal_udpate;
         } else {
            cur_block += vertical_udpate;
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
};
// -------------------------------------------------------------------------------------
}
// -------------------------------------------------------------------------------------
