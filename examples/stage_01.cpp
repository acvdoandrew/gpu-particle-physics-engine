#include "physics/Particle.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

int main() {

  // Window properties as constants
  const sf::Vector2u windowSize({800, 600});
  const float particleRadius = 5.f;
  const uint32_t particleCount = 500;

  // Create main window
  sf::RenderWindow window(sf::VideoMode({windowSize.x, windowSize.y}),
                          "Particle Engine");
  window.setFramerateLimit(60); // Limit framerate for consistency

  // Vector to hold all our particles
  std::vector<Particle> particles(particleCount);
  for (auto &p : particles) {
    float x = static_cast<float>(rand() % windowSize.x);
    float y = static_cast<float>(rand() % (windowSize.y / 2));
    p.position = {x, y};
    p.old_position = p.position;
    p.acceleration = {0.f, 1000.f};
  }

  // Create circle shape
  std::vector<sf::CircleShape> particleShapes(particleCount);
  for (auto &s : particleShapes) {
    s.setRadius(particleRadius);
    s.setFillColor(sf::Color::White);
    s.setOrigin({particleRadius, particleRadius});
  }

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

    // Loop through all particles for physics and collision
    for (auto &particle : particles) {
      // Save current position
      const sf::Vector2f previous_position = particle.position;

      // Perform Verlet Integration
      particle.position = 2.f * particle.position - particle.old_position +
                          particle.acceleration * dt * dt;

      // Update old position
      particle.old_position = previous_position;

      // Collision logic remains the same
      const float damping = 0.8f;
      if (particle.position.y > windowSize.y - particleRadius) {
        particle.position.y = windowSize.y - particleRadius;
        // For Verlet, we need to update old_position on collision as well
        particle.old_position.y =
            particle.position.y +
            (particle.position.y - particle.old_position.y) * damping;
      }
      if (particle.position.x > windowSize.x - particleRadius) {
        particle.position.x = windowSize.x - particleRadius;
        particle.old_position.x =
            particle.position.x +
            (particle.position.x - particle.old_position.x) * damping;
      }
      if (particle.position.x < particleRadius) {
        particle.position.x = particleRadius;
        particle.old_position.x =
            particle.position.x +
            (particle.position.x - particle.old_position.x) * damping;
      }
    }

    window.clear(sf::Color::Black);

    // ---  Loop through all particles to draw them ---
    for (size_t i = 0; i < particles.size(); ++i) {
      particleShapes[i].setPosition(particles[i].position);
      window.draw(particleShapes[i]);
    }

    window.display();
  }

  return 0;
}
