#pragma once

#include "physics/Particle.hpp"
#include <unordered_map>
#include <vector>

class SpatialHash {
private:
  int getHashKey(const sf::Vector2f& position) const;

  std::unordered_map<int, std::vector<Particle*>> m_grid;
  float m_cellSize;

public:
  SpatialHash(float cell_size);

  void insert(Particle* particle);

  void query(Particle* particle, std::vector<Particle*>& neighbors_out);

  void clear();
};
