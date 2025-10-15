#include "physics/Particle.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <vector>

int main() {

  // Window properties as constants
  const sf::Vector2u windowSize({1920, 1080});
  const float particleRadius = 5.f;
  const uint32_t maxParticles = 2000;

  // Create main window
  sf::RenderWindow window(sf::VideoMode({windowSize.x, windowSize.y}),
                          "Particle Engine");
  window.setFramerateLimit(60); // Limit framerate for consistency

  // Vector to hold all our particles
  std::vector<Particle> particles;
  particles.reserve(maxParticles);

  // FPS counter setup
  sf::Font font;
  // Load the font from the file in our directory
  if (!font.openFromFile("assets/font.ttf")) {
    return -1;
  }
  sf::Text fpsText(font);
  fpsText.setCharacterSize(16);
  fpsText.setFillColor(sf::Color::White);
  fpsText.setPosition({5.f, 5.f});
  float frameTime = 0.f;

  // Particle Counter setup
  sf::Text particleCountText(font);
  particleCountText.setCharacterSize(16);
  particleCountText.setFillColor(sf::Color::White);
  particleCountText.setPosition(
      {5.f, 25.f}); // Position it below the FPS counter

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
    if (particles.size() < maxParticles) {
      Particle p;
      // Add a small random offset to the spawn position
      const float jitter_x = (static_cast<float>(rand()) / RAND_MAX) * 2.f -
                             1.f; // float from -1 to 1;
      p.position = {(windowSize.x / 2.f) + jitter_x, 100.f};
      p.old_position = p.position;
      p.acceleration = {0.f, 1000.f};
      particles.push_back(p);
    }

    // Loop through all particles for physics and collision
    for (auto& p : particles) {
      // Apply Gravity (Verlet Integration)
      const sf::Vector2f previous_position = p.position;
      p.position = 2.f * p.position - p.old_position + p.acceleration * dt * dt;
      p.old_position = previous_position;
    }

    // Collision Loop
    const int num_sub_steps = 4;
    for (int k = 0; k < num_sub_steps; ++k) {

      // Boundary constrains
      for (auto& particle : particles) {
        const float damping = 0.8f;
        if (particle.position.y > windowSize.y - particleRadius) {
          particle.position.y = windowSize.y - particleRadius;
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

      // Particle - Particle Collision
      for (size_t i = 0; i < particles.size(); ++i) {
        auto& p1 = particles[i];
        for (size_t j = i + 1; j < particles.size(); ++j) {
          auto& p2 = particles[j];
          const sf::Vector2f v = p1.position - p2.position;
          const float dist2 = v.x * v.x + v.y * v.y;
          const float min_dist = 2.f * particleRadius;

          // Check for overlap
          if (dist2 < min_dist * min_dist) {
            const float dist = std::sqrt(dist2);

            if (dist > 0.0001f) {
              const sf::Vector2f collision_axis = v / dist;
              const float overlap = 0.5f * (min_dist - dist);

              // Move particles apart
              p1.position += collision_axis * overlap;
              p2.position -= collision_axis * overlap;
            }
          }
        }
      }
    }

    // Update fps counter
    int fps = static_cast<int>(1.f / frameTime);
    fpsText.setString("FPS: " + std::to_string(fps));

    // Update Particle Counter Text
    particleCountText.setString("Particles: " +
                                std::to_string(particles.size()));

    window.clear(sf::Color::Black);

    // This is not super efficient, but simple for now
    sf::CircleShape particleShape(particleRadius);
    particleShape.setFillColor(sf::Color::White);
    particleShape.setOrigin({particleRadius, particleRadius});
    for (const auto& p : particles) {
      particleShape.setPosition(p.position);
      window.draw(particleShape);
    }

    // --- NEW: Draw the FPS counter ---
    window.draw(fpsText);
    window.draw(particleCountText);

    window.display();
  }

  return 0;
}
