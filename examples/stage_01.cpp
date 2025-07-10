#include "physics/Particle.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

int main() {

  // Window properties as constants
  const sf::Vector2u windowSize({800, 600});
  const float particleRadius = 5.f;
  const uint32_t maxParticles = 500;

  // Create main window
  sf::RenderWindow window(sf::VideoMode({windowSize.x, windowSize.y}),
                          "Particle Engine");
  window.setFramerateLimit(60); // Limit framerate for consistency

  // Vector to hold all our particles
  std::vector<Particle> particles;
  particles.reserve(maxParticles);

  // Spawning Logic variables
  const float spawnInterval = 0.01f;
  float spawnTimer = 0.f;

  // FPS counter setup
  sf::Font font;
  // Load the font from the file in our directory
  if (!font.openFromFile("font.ttf")) {
    return -1;
  }
  sf::Text fpsText(font);
  fpsText.setCharacterSize(16);
  fpsText.setFillColor(sf::Color::White);
  fpsText.setPosition({5.f, 5.f});
  float frameTime = 0.f;

  // Clock
  sf::Clock clock;

  // Game loop will run as long as the window is open
  while (window.isOpen()) {

    // Get the time elapsed since the last frame
    // restart() returns the elapsed time and resets the clock for the next
    // frame
    float dt = clock.restart().asSeconds();
    frameTime = dt;
    if (dt > 0.1f) {
      dt = 0.1f;
    }

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    // Time-based spawn logic
    spawnTimer += dt;
    if (spawnTimer > spawnInterval && particles.size() < maxParticles) {
      spawnTimer = 0.f;
      Particle p;
      p.position = {windowSize.x / 2.f, 100.f};
      p.old_position = p.position;
      p.acceleration = {0.f, 1000.f};
      particles.push_back(p);
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

    // Update fps counter
    int fps = static_cast<int>(1.f / frameTime);
    fpsText.setString("FPS: " + std::to_string(fps));

    window.clear(sf::Color::Black);

    // This is not super efficient, but simple for now
    sf::CircleShape particleShape(particleRadius);
    particleShape.setFillColor(sf::Color::White);
    particleShape.setOrigin({particleRadius, particleRadius});
    for (const auto &p : particles) {
      particleShape.setPosition(p.position);
      window.draw(particleShape);
    }

    // --- NEW: Draw the FPS counter ---
    window.draw(fpsText);

    window.display();
  }

  return 0;
}
