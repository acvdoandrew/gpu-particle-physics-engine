#include <SFML/Graphics.hpp>

int main() {
  // Create main window
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Particle Engine");

  // Game loop will run as long as the window is open
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    window.clear(sf::Color::Black);
    window.display();
  }
}
