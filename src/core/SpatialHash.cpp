#include "core/SpatialHash.hpp"
#include <cstdint>
#include <vector>

constexpr int32_t PRIME_1 = 73289993;
constexpr int32_t PRIME_2 = 12319898;

SpatialHash::SpatialHash(float cell_size)
    : m_cellSize(cell_size) {

      };

void SpatialHash::clear() { m_grid.clear(); }

int32_t SpatialHash::getHashKey(const sf::Vector2f& position) const {
  // Core SpatialHash
  // Divide the position by the cell size to get the integer grid coordinates.
  const int32_t cell_x = static_cast<int32_t>(position.x / m_cellSize);
  const int32_t cell_y = static_cast<int32_t>(position.y / m_cellSize);

  // Combine the 2D grid coordinates into a single 1D hash key
  // Using large prime numbers helps to distribute the keys more evenly.
  return cell_x * PRIME_1 + cell_y * PRIME_2;
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

std::vector<Particle*> SpatialHash::query(Particle* particle) {
  // The list of potential neighbors we will return
  std::vector<Particle*> neighbors;

  // Get the grid cell coordinates of the particle-
  const int32_t cell_x =
      static_cast<int32_t>(particle->position.x / m_cellSize);
  const int32_t cell_y =
      static_cast<int32_t>(particle->position.y / m_cellSize);

  // Loop through the 3x3 neighborhood of cells
  for (int32_t dx = -1; dx <= 1; ++dx) {
    for (int32_t dy = -1; dy <= 1; ++dy) {
      const int32_t neighbor_key =
          (cell_x + dx) * PRIME_1 + (cell_y + dy) * PRIME_2;

      // Check if a cell with this key exists in our grid.
      auto it = m_grid.find(neighbor_key);
      if (it != m_grid.end()) {
        neighbors.insert(neighbors.end(), it->second.begin(), it->second.end());
      }
    }
  }
  return neighbors;
}
