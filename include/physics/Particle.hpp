#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/System/Vector2.hpp>

struct Particle {
  sf::Vector2f position;
  sf::Vector2f old_position;
  sf::Vector2f acceleration;
};

#endif // !PARTICLE_H
