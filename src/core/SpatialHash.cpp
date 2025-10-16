#include "core/SpatialHash.hpp"
#include <cmath>
#include <cstdint>
#include <vector>

constexpr int32_t PRIME_1 = 73289993;
constexpr int32_t PRIME_2 = 12319898;

SpatialHash::SpatialHash(float cell_size)
    : m_cellSize(cell_size) {

      };

void SpatialHash::clear() { m_grid.clear(); }

int32_t SpatialHash::getHashKey(const sf::Vector2f& position) const {
  // Use std::floor to correctly handle negative positions.
  // e.g., -50.f / 100.f becomes -0.5f, which floors to -1, the correct cell.
  const int32_t cell_x =
      static_cast<int32_t>(std::floor(position.x / m_cellSize));
  const int32_t cell_y =
      static_cast<int32_t>(std::floor(position.y / m_cellSize));

  // A more robust hashing function that avoids collisions with negative
  // coordinates. The bitwise XOR (^) is better at combining hash values than
  // simple addition.
  return (cell_x * 73856093) ^ (cell_y * 19349663);
}

void SpatialHash::insert(Particle* particle) {
  // Get the hash key for the particle's current position
  const int32_t key = getHashKey(particle->position);

  // Use the key to access the int map
  // If the key doesnt exist, std::unordered_map automatically creates a new
  // empty std::vector for us at that key.
  // Then, add the particle's pointer to that vector
  m_grid[key].push_back(particle);
}

void SpatialHash::query(Particle* particle,
                        std::vector<Particle*>& neighbors_out) {

  // Clear the vector that was passed in
  neighbors_out.clear();
  // Get the grid cell coordinates of the particle-
  const int32_t cell_x =
      static_cast<int32_t>(particle->position.x / m_cellSize);
  const int32_t cell_y =
      static_cast<int32_t>(particle->position.y / m_cellSize);

  for (int32_t dx = -1; dx <= 1; ++dx) {
    for (int32_t dy = -1; dy <= 1; ++dy) {
      const int32_t neighbor_key =
          (cell_x + dx) * PRIME_1 + (cell_y + dy) * PRIME_2;
      auto it = m_grid.find(neighbor_key);
      if (it != m_grid.end()) {
        // Insert the contents of the grid cell's vector into our output vector
        neighbors_out.insert(neighbors_out.end(), it->second.begin(),
                             it->second.end());
      }
    }
  }
}
