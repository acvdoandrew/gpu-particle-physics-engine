#include "core/Solver.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdint>
#include <vector>

Solver::Solver() : m_grid(10.0f) { m_particles.reserve(10000); }

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

  m_collisionChecks = 0;

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
  for (auto& p1 : m_particles) {
    std::vector<Particle*> neighbors;
    m_grid.query(&p1, neighbors);

    for (Particle* p2_ptr : neighbors) {
      auto& p2 = *p2_ptr;

      if (&p1 < &p2) {
        m_collisionChecks++;
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
