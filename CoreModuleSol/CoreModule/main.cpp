#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Projectile.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

const int MAX_SCORE = 500;
const float GAME_DURATION = 300.0f;

// Function to generate a random float between min and max
float getRandomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int main() {
    // Define initial window size
    sf::Vector2u windowSize(600, 800);

    // Window init
    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Makaveli");

    // Player init
    std::unique_ptr<Player> player = std::make_unique<Player>(
        sf::Vector2f(300, 750), // Initial player position
        sf::Vector2f(50.0f, 50.0f), // Player size
        sf::Vector2f(0.0f, 0.0f), // Initial velocity
        sf::Vector2f(0.0f, 0.0f), // Initial direction
        500.0f, // Acceleration
        0.25f, // Friction
        0.5f, // Stopping factor
        0.5f, // Shooting cooldown
        windowSize // Initial window size passed to Player constructor
    );

    // Game's clock
    sf::Clock clock;

    float spawnTimer = 0;
    float spawnInterval = 1.0f;
    float specialSpawnTimer = 0;
    float specialSpawnInterval = getRandomFloat(5.0f, 15.0f); // Random interval for special projectile

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
        float elapsedTime = clock.getElapsedTime().asSeconds();

        if (!gameOver && !gameWin) {
            // Update player
            player->update(deltaTime);

            spawnTimer += deltaTime;
            specialSpawnTimer += deltaTime;

            if (spawnTimer >= spawnInterval) {
                // Generate a random value on the x axis based on the window size
                float x = static_cast<float>(rand() % windowSize.x);

                int randomDirection = (std::rand() % 2 == 0 ? -1 : 1);
                sf::Vector2f direction = sf::Vector2f(randomDirection, 1);

                // Spawn regular falling projectile
                Projectile::projectiles.emplace_back(std::make_unique<Projectile>(
                    sf::Vector2f(x, 0),
                    sf::Vector2f(30.0f, 30.0f),
                    direction,
                    sf::Vector2f(0.0f, 200.0f),
                    300.0f,
                    "falling",
                    sf::Color::Red,
                    windowSize
                ));

                spawnTimer = 0;
            }

            if (specialSpawnTimer >= specialSpawnInterval) {
                // Generate a random value on the x axis based on the window size
                float x = static_cast<float>(rand() % windowSize.x);

                int randomDirection = (std::rand() % 2 == 0 ? -1 : 1);
                sf::Vector2f direction = sf::Vector2f(randomDirection, 1);

                // Spawn special projectile with random color and speed
                Projectile::projectiles.emplace_back(std::make_unique<Projectile>(
                    sf::Vector2f(x, 0),
                    sf::Vector2f(30.0f, 30.0f),
                    direction,
                    sf::Vector2f(0.0f, getRandomFloat(100.0f, 300.0f)),
                    300.0f,
                    "falling",
                    sf::Color::Yellow,
                    windowSize
                ));

                specialSpawnTimer = 0;
                specialSpawnInterval = getRandomFloat(5.0f, 15.0f); // Reset the special spawn interval
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

            // Check for game win
            if (score >= MAX_SCORE) {
                gameWin = true;
            }

            // Check for game over by timer
            if (elapsedTime >= GAME_DURATION) {
                gameOver = true;
            }
        }

        // Clear the window before drawing
        window.clear();

        // Draw player
        player->draw(window);

        // Draw projectiles
        for (const auto& projectile : Projectile::projectiles) {
            projectile->draw(window);
        }

        // Draw game over screen
        if (gameOver) {
            sf::Font font;
            if (font.loadFromFile("arial.ttf")) { // Make sure you have the font file
                sf::Text text("Game Over", font, 50);
                text.setFillColor(sf::Color::Red);
                text.setPosition(windowSize.x / 2 - text.getLocalBounds().width / 2, windowSize.y / 2 - text.getLocalBounds().height / 2);
                window.draw(text);
            }
        }

        // Draw game win screen
        if (gameWin) {
            sf::Font font;
            if (font.loadFromFile("arial.ttf")) { // Make sure you have the font file
                sf::Text text("You Win!", font, 50);
                text.setFillColor(sf::Color::Green);
                text.setPosition(windowSize.x / 2 - text.getLocalBounds().width / 2, windowSize.y / 2 - text.getLocalBounds().height / 2);
                window.draw(text);
            }
        }

        // Display the score
        sf::Font font;
        if (font.loadFromFile("arial.ttf")) { // Make sure you have the font file
            sf::Text scoreText("Score: " + std::to_string(score), font, 20);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(10, 10);
            window.draw(scoreText);

            // Display the timer
            int timeLeft = static_cast<int>(GAME_DURATION - elapsedTime);
            sf::Text timerText("Time: " + std::to_string(timeLeft), font, 20);
            timerText.setFillColor(sf::Color::White);
            timerText.setPosition(10, 40);
            window.draw(timerText);
        }

        // Display what has been drawn
        window.display();
    }

    return 0;
}
