#include "core/SpatialHash.hpp"
#include <iostream>
#include <vector>

int main() {

  SpatialHash grid(100.0f);

  Particle p1, p2, p3, p4;
  p1.position = {50.f, 50.f};  // Should be in cell (0, 0)
  p2.position = {150.f, 50.f}; // Should be in cell (1, 0) -> neighbor of p1
  p3.position = {55.f, 60.f};  // Should be in cell (0, 0) -> same cell as p1
  p4.position = {250.f,
                 250.f}; // Should be in cell (2, 2) -> not a neighbor of p1

  grid.clear();
  grid.insert(&p1);
  grid.insert(&p2);
  grid.insert(&p3);
  grid.insert(&p4);

  std::cout << "Testing Spatial Hash Query...\n";

  return 0;
}
