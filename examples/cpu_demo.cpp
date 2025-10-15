#include "core/Solver.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <string>
#include <vector>

// =================================================================
// DIAGNOSTIC FLAGS - CHANGE THESE TO RUN EXPERIMENTS
// =================================================================
const bool PHYSICS_ENABLED = true;
const bool RENDERING_ENABLED = true;
// =================================================================

int main() {

  // Window properties as constants
  const sf::Vector2u windowSize({800, 600});
  const uint32_t maxParticles = 5000; // Let's test with a high number

  // Create main window
  sf::RenderWindow window(sf::VideoMode({windowSize.x, windowSize.y}),
                          "Particle Engine");
  window.setFramerateLimit(144); // Uncap framerate to see true performance

  Solver solver;

  // Font and text setup
  sf::Font font;
  if (!font.openFromFile("assets/font.ttf")) {
    return -1;
  }
  sf::Text fpsText(font);
  fpsText.setCharacterSize(16);
  fpsText.setFillColor(sf::Color::White);
  fpsText.setPosition({5.f, 5.f});

  sf::Text particleCountText(font);
  particleCountText.setCharacterSize(16);
  particleCountText.setFillColor(sf::Color::White);
  particleCountText.setPosition({5.f, 25.f});

  sf::Clock clock;
  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();
    if (dt > 0.1f) {
      dt = 0.1f;
    }

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    // Spawning logic (always runs)
    if (solver.getParticles().size() < maxParticles) {
      const float jitter_x =
          (static_cast<float>(rand()) / RAND_MAX) * 2.f - 1.f;
      solver.addParticle({(windowSize.x / 2.f) + jitter_x, 100.f});
    }

    // --- Run Physics ONLY if enabled ---
    if (PHYSICS_ENABLED) {
      solver.update(dt);
    }

    // Update UI Text (always runs)
    const float frameTime = dt;
    int fps = (frameTime > 0.f) ? static_cast<int>(1.f / frameTime) : 0;
    fpsText.setString("FPS: " + std::to_string(fps));
    particleCountText.setString("Particles: " +
                                std::to_string(solver.getParticles().size()));

    // Render Step
    window.clear(sf::Color::Black);

    // --- Render Particles ONLY if enabled ---
    if (RENDERING_ENABLED) {
      const auto& particles = solver.getParticles();
      sf::CircleShape particleShape(5.f);
      particleShape.setFillColor(sf::Color::White);
      particleShape.setOrigin({5.f, 5.f});
      for (const auto& p : particles) {
        particleShape.setPosition(p.position);
        window.draw(particleShape);
      }
    }

    window.draw(fpsText);
    window.draw(particleCountText);
    window.display();
  }
  return 0;
}
