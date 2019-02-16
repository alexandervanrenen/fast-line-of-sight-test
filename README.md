## Line of Sight Test for 2D Games

Code for my [blog post][1] on how to optimize line of sight tests.

There are two little benchmarks:

### Line Segment Intersection Benchmark

This one compares different techniques for line segment intersection.

``
g++ line_intersection.cpp util/Line.cpp util/Point.cpp util/FastRandom.cpp -std=c++14 -g0 -O3 -I./ -o line_intersection -flto -march=native && ./line_intersection
./line_intersection
``

### Line of Sight Test

This one compares different techniques for line of sight tests.

``
g++ line_of_sight.cpp util/Line.cpp util/Point.cpp util/FastRandom.cpp -std=c++14 -g0 -O3 -I./ -o line_of_sight -flto -march=native && ./line_of_sight
./line_of_sight
``

[1]:http://anizmow.com/blog/line-of-sight-test/