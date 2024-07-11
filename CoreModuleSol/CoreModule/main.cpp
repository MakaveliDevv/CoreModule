#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Projectile.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

// Function to get window size
sf::Vector2u getWindowSize(const sf::RenderWindow& window) {
    return window.getSize();
}

int main() {
    // Window init
    sf::RenderWindow window(sf::VideoMode(600, 800), "Makaveli");

    // Player init
    std::unique_ptr<Player> player = std::make_unique<Player>(
        sf::Vector2f(300, 750),
        sf::Vector2f(50.0f, 50.0f),
        sf::Vector2f(0.0f, 0.0f),
        sf::Vector2f(0.0f, 0.0f),
        window,
        500.0f,
        0.25f,
        0.5f,
        0.5f
    );

    // Game's clock
    sf::Clock clock;

    float spawnTimer = 0;
    float spawnInterval = 1.0f; 
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
                float x = static_cast<float>(rand() % getWindowSize(window).x);

                // Initial direction is downwards, acceleration is set
                Projectile::projectiles.emplace_back(std::make_unique<Projectile>(
                    sf::Vector2f(x, 0),
                    sf::Vector2f(30.0f, 30.0f),
                    sf::Vector2f(1.0f, 1.0f),
                    sf::Vector2f(0.0f, 200.0f),
                    window,
                    projectileAcceleration,
                    "falling",
                    sf::Color::Red
                ));

                spawnTimer = 0;
            }

            // Update projectiles
            for (const auto& projectile : Projectile::projectiles) {
                projectile->update(deltaTime);
                projectile->checkCollisionWithPlayer(*player);

                // Check collision with other projectiles
                for (const auto& other : Projectile::projectiles) {
                    if (projectile != other) {
                        projectile->checkCollisionWithProjectile(*other);
                    }
                }
            }


            // Remove projectiles that are off screen
            Projectile::removeOutOfBounds();
        }

        // Clear the window before drawing
        window.clear();

        // Draw player
        player->draw(window);

        // Draw projectiles
        for (const auto& projectile : Projectile::projectiles) {
            projectile->draw(window);
        }

        // Display what has been drawn
        window.display();
    }

    return 0;
}
