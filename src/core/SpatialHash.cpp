#include "core/SpatialHash.hpp"
#include <cmath>
#include <cstdint>
#include <vector>

constexpr int32_t PRIME_1 = 73856093;
constexpr int32_t PRIME_2 = 19349663;

SpatialHash::SpatialHash(float cell_size) : m_cellSize(cell_size) {}

void SpatialHash::clear() { m_grid.clear(); }

// This function is the single source of truth for our hashing algorithm.
int32_t SpatialHash::getHashKey(const sf::Vector2f& position) const {
  const int32_t cell_x =
      static_cast<int32_t>(std::floor(position.x / m_cellSize));
  const int32_t cell_y =
      static_cast<int32_t>(std::floor(position.y / m_cellSize));
  return (cell_x * PRIME_1) ^ (cell_y * PRIME_2);
}

void SpatialHash::insert(Particle* particle) {
  const int32_t key = getHashKey(particle->position);
  m_grid[key].push_back(particle);
}

void SpatialHash::query(Particle* particle,
                        std::vector<Particle*>& neighbors_out) {
  neighbors_out.clear();

  // Get the base cell coordinates of the particle.
  const int32_t cell_x =
      static_cast<int32_t>(std::floor(particle->position.x / m_cellSize));
  const int32_t cell_y =
      static_cast<int32_t>(std::floor(particle->position.y / m_cellSize));

  // Loop through the 3x3 neighborhood
  for (int32_t dx = -1; dx <= 1; ++dx) {
    for (int32_t dy = -1; dy <= 1; ++dy) {

      // Calculate the neighbor's key using the EXACT SAME HASHING ALGORITHM
      // as getHashKey().
      const int32_t neighbor_key =
          ((cell_x + dx) * PRIME_1) ^ ((cell_y + dy) * PRIME_2);

      auto it = m_grid.find(neighbor_key);
      if (it != m_grid.end()) {
        // If the key exists, insert the particles from that cell into our
        // results.
        neighbors_out.insert(neighbors_out.end(), it->second.begin(),
                             it->second.end());
      }
    }
  }
}
