#include "physics/Particle.hpp"
#include <SFML/Graphics.hpp>

int main() {
  // Create main window
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Particle Engine");
  window.setFramerateLimit(60); // Limit framerate for consistency

  // Create the particle's data model
  Particle particle;
  particle.acceleration = {0.f, 1000.f};

  // Create circle shape
  sf::CircleShape particleShape(10.f);
  particleShape.setFillColor(sf::Color::White);

  // Clock
  sf::Clock clock;

  // Game loop will run as long as the window is open
  while (window.isOpen()) {

    // Get the time elapsed since the last frame
    // restart() returns the elapsed time and resets the clock for the next
    // frame
    float dt = clock.restart().asSeconds();

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    // Euler integration formula
    particle.velocity += particle.acceleration * dt;
    particle.position += particle.velocity * dt;

    // Now we update the particle position using the particle's physics state
    particleShape.setPosition(particle.position);

    window.clear(sf::Color::Black);
    window.draw(particleShape);
    window.display();
  }

  return 0;
}
