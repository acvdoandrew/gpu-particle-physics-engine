#pragma once
#include <SFML/System/Vector2.hpp>

struct Particle {
  sf::Vector2f position;
  sf::Vector2f old_position;
  sf::Vector2f acceleration;
};
