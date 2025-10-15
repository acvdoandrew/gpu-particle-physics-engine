#ifndef SOLVER_H
#define SOLVER_H

#include "physics/Particle.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>

class Solver {
private:
  void applyGravity();
  void applyBoundaryConstraints();
  void solveCollision();

  std::vector<Particle> m_particles;
  sf::Vector2f m_gravity = {0.f, 1000.f};
  sf::Vector2u m_windowSize = {800, 600};
  const float m_particleRadius = 5.f;

public:
  Solver();
  void update(float dt);
  void addParticle(sf::Vector2f position);
  const std::vector<Particle>& getParticles() const;
};

#endif
