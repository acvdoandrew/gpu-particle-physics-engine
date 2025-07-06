#include "physics/Particle.hpp"
#include <SFML/Graphics.hpp>

int main() {

  // Window properties as constants
  const sf::Vector2u windowSize({800, 600});
  const float particleRadius = 10.f;

  // Create main window
  sf::RenderWindow window(sf::VideoMode({windowSize.x, windowSize.y}),
                          "Particle Engine");
  window.setFramerateLimit(60); // Limit framerate for consistency

  // Create the particle's data model
  Particle particle;
  particle.position = {windowSize.x / 2.f, windowSize.y / 2.f};
  particle.velocity = {0.f, 0.f};
  particle.acceleration = {0.f, 1000.f};

  // Create circle shape
  sf::CircleShape particleShape(particleRadius);
  particleShape.setFillColor(sf::Color::White);
  particleShape.setOrigin({particleRadius, particleRadius});

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

    // Collision Detection and Response
    const float damping = 0.8f;

    // Floor Collision
    if (particle.position.y > windowSize.y - particleRadius) {
      particle.position.y = windowSize.y - particleRadius;
      particle.velocity.y *= -damping;
    }

    // Right wall Collision
    if (particle.position.x > windowSize.x - particleRadius) {
      particle.position.x = windowSize.x - particleRadius;
      particle.velocity.x *= -damping;
    }

    // Left wall Collision
    if (particle.position.x < particleRadius) {
      particle.position.x = particleRadius;
      particle.velocity.x *= -damping;
    }

    // Now we update the particle position using the particle's physics state
    particleShape.setPosition(particle.position);

    window.clear(sf::Color::Black);
    window.draw(particleShape);
    window.display();
  }

  return 0;
}
