#pragma once
#include <SFML/System/Vector2.hpp>

struct Particle {
  sf::Vector2f position{400.f, 300.f};
  sf::Vector2f velocity{0.f, 0.f};
  sf::Vector2f acceleration{0.f, 0.f};
};
