#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Projectile.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

int main() {
    // Window init
    sf::RenderWindow window(sf::VideoMode(600, 800), "Makaveli");

    // Player init
    std::unique_ptr<Player> player = std::make_unique<Player>(
        sf::Vector2f(300, 750),   // Position
        sf::Vector2f(50.0f, 50.0f),  // Size
        sf::Vector2f(0.0f, 0.0f),    // Velocity
        sf::Vector2f(0.0f, 0.0f),    // Direction
        window,                     // RenderWindow
        500.0f,                     // Acceleration
        0.25f,                      // Friction
        0.5f                        // Stopping Factor
    );

    std::vector<std::unique_ptr<Projectile>> projectiles;

    // Game's clock
    sf::Clock clock;

    float spawnTimer = 0;
    float spawnInterval = 1.0f; // spawn a projectile every second
    float projectileAcceleration = 200.0f;

    srand(static_cast<unsigned>(time(0)));

    int score = 0;
    bool gameOver = false;
    bool gameWin = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Assign the time to a variable
        float deltaTime = clock.restart().asSeconds();

        if (!gameOver && !gameWin) {
            // Update player
            player->update(deltaTime);

            spawnTimer += deltaTime;
            if (spawnTimer >= spawnInterval) {
                // Generate a random value on the x axis based on the window size
                float x = static_cast<float>(rand() % window.getSize().x);

                // Initial velocity is zero, direction is downwards, acceleration is set
                projectiles.emplace_back(std::make_unique<Projectile>(
                    sf::Vector2f(x, 0),
                    sf::Vector2f(30.0f, 30.0f),
                    sf::Vector2f(0.0f, 0.0f), // Initial velocity is zero
                    sf::Vector2f(0.0f, 1.0f), // Move downwards
                    window,
                    projectileAcceleration
                ));

                spawnTimer = 0;
            }

            for (auto it = projectiles.begin(); it != projectiles.end();) {
                (*it)->update(deltaTime);
                if ((*it)->getBounds().top > window.getSize().y) {
                    it = projectiles.erase(it); // Remove projectile if it goes out of bounds
                }
                else {
                    ++it;
                }
            }
        }

        // Clear the window before drawing
        window.clear();

        // Draw player
        player->draw(window);

        // Draw projectiles
        for (const auto& projectile : projectiles) {
            projectile->draw(window);
        }

        // Display what has been drawn
        window.display();
    }

    return 0;
}
