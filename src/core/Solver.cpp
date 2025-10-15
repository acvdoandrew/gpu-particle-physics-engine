#include "core/Solver.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdint>
#include <vector>

Solver::Solver() : m_grid(2.f * m_particleRadius) { m_particles.reserve(2000); }

const std::vector<Particle>& Solver::getParticles() const {
  return m_particles;
}

void Solver::addParticle(sf::Vector2f position) {
  Particle p;
  p.position = position;
  p.old_position = position;
  p.acceleration = m_gravity;
  m_particles.push_back(p);
}

void Solver::update(float dt) {
  if (dt > 0.05f) {
    dt = 0.05f;
  }

  const float sub_dt = dt / static_cast<float>(m_sub_steps);

  for (uint32_t i = m_sub_steps; i > 0; --i) {

    updatePositions(sub_dt);

    m_grid.clear();
    for (auto& p : m_particles) {
      m_grid.insert(&p);
    }

    solveCollision();
    applyBoundaryConstraints();
  }
}

void Solver::updatePositions(float dt) {
  for (auto& p : m_particles) {
    p.acceleration = m_gravity;

    // Perform Verlet integration
    const sf::Vector2f previous_position = p.position;
    p.position = 2.f * p.position - p.old_position + p.acceleration * dt * dt;
    p.old_position = previous_position;
  }
}

void Solver::applyBoundaryConstraints() {
  for (auto& p : m_particles) {
    const float damping = 0.8f;
    if (p.position.y > m_windowSize.y - m_particleRadius) {
      p.position.y = m_windowSize.y - m_particleRadius;
      p.old_position.y =
          p.position.y + (p.position.y - p.old_position.y) * damping;
    }
    if (p.position.x > m_windowSize.x - m_particleRadius) {
      p.position.x = m_windowSize.x - m_particleRadius;
      p.old_position.x =
          p.position.x + (p.position.x - p.old_position.x) * damping;
    }
    if (p.position.x < m_particleRadius) {
      p.position.x = m_particleRadius;
      p.old_position.x =
          p.position.x + (p.position.x - p.old_position.x) * damping;
    }
  }
}

void Solver::solveCollision() {
  // We create one vector ONCE outside the Loop
  std::vector<Particle*> neighbors;

  neighbors.reserve(100);
  // Loop through every particle once
  for (auto& p1 : m_particles) {

    m_grid.query(&p1, neighbors);

    // Check against only the neighbors
    for (Particle* p2_ptr : neighbors) {
      auto& p2 = *p2_ptr;

      // This is the trick to process each pair only once.
      // We use the memory address of the particles as a unique ID.
      // We only solve the collision if p1's address is less than p2's.
      // This guarantees that the pair (p1, p2) is solved, but (p2, p1) is
      // skipped.
      if (&p1 < &p2) {
        const sf::Vector2f v = p1.position - p2.position;
        const float dist2 = v.x * v.x + v.y * v.y;
        const float min_dist = 2.f * m_particleRadius;

        if (dist2 < min_dist * min_dist) {
          const float dist = std::sqrt(dist2);
          if (dist > 0.0001f) {
            const sf::Vector2f collision_axis = v / dist;
            const float overlap = 0.5f * (min_dist - dist);
            p1.position += collision_axis * overlap;
            p2.position -= collision_axis * overlap;
          }
        }
      }
    }
  }
}
