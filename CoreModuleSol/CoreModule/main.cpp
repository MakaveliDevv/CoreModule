#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Projectile.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

enum class GameState {
    StartScreen,
    RulesScreen,
    Playing,
    GameOver,
    GameWin
};

const int MAX_SCORE = 500;
const float GAME_DURATION = 300.0f;

int score;
bool gameOver = false;
bool gameWin = false;
float totalElapsedTime = 0;

const float margin = 5.0f;
sf::Vector2f w_Size = sf::Vector2f(1200, 1600);

// Function to generate a random float between min and max
float getRandomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int main() {
    // Define initial window size
    sf::Vector2u windowSize(w_Size.x, w_Size.y);

    // Window init
    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Makaveli");

    // Initialize game state
    GameState gameState = GameState::StartScreen;

    // Font init
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    sf::Vector2f playerSize = sf::Vector2f(50.0f, 50.0f);

    // Player init
    std::unique_ptr<Player> player = std::make_unique<Player>(
        sf::Vector2f(windowSize.x / 2, windowSize.y - playerSize.y), // Initial player position
        sf::Vector2f(playerSize.x, playerSize.y), // Player size
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
    float spawnTimer_2 = 0;
    float spawnInterval_2 = getRandomFloat(5.0f, 15.0f);
    float spawnTimer_3 = 0;
    float spawnInterval_3 = getRandomFloat(3.0f, 8.0f);

    srand(static_cast<unsigned>(time(0)));

    // Menu options
    std::vector<sf::Text> startScreenOptions;
    sf::Text playOption("Play", font, 30);
    sf::Text rulesOption("Rules", font, 30);
    sf::Text quitOption("Quit Game", font, 30);

    playOption.setFillColor(sf::Color::White);
    rulesOption.setFillColor(sf::Color::White);
    quitOption.setFillColor(sf::Color::White);

    startScreenOptions.push_back(playOption);
    startScreenOptions.push_back(rulesOption);
    startScreenOptions.push_back(quitOption);

    for (int i = 0; i < startScreenOptions.size(); ++i) {
        startScreenOptions[i].setPosition(windowSize.x / 2 - startScreenOptions[i].getLocalBounds().width / 2, windowSize.y / 2 - (startScreenOptions.size() / 2 - i) * 40);
    }

    sf::Text rulesText;
    rulesText.setFont(font);
    rulesText.setCharacterSize(30);
    rulesText.setFillColor(sf::Color::White);
    rulesText.setPosition(50, 75);
    rulesText.setString("Rules:\n"
        "1. Use arrow keys to move.\n"
        "\n"
        "2. Use 'Space Key' to shoot projectiles.\n"
        "\n"
        "3. Avoid the 'destructive' projectiles.\n"
        "\n"
        "4. You gain +5 points for every 'normal' \n projectile.\n"
        "\n"
        "5. You gain +10 points for every 'special' \n projectile.\n"
        "\n"
        "6. You lose -5 points for every 'destructive' \n projectile you shoot down.\n"
        "\n"
        "7. You lose -15 points for every 'destructive' \n projectile that hits you.\n"
        "\n"
        "8. The normal projectiles are white, \n the special ones are yellow \n and the destructive ones are red.\n"
        "\n"
        "9. Reach the max amount of points to win the \n game.\n"
        "\n"
        "10. You lose the game if your points are in \n negative amount or if the \n timer ran out.\n"
        "\n"
        "11. Reach the max amount of points before the \n timer runs out to win the game.\n"
        "\n"
        "12. The game gets gradually harder over time.\n");


    int selectedOption = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (gameState == GameState::StartScreen) {
                    if (event.key.code == sf::Keyboard::Up) {
                        selectedOption = (selectedOption - 1 + startScreenOptions.size()) % startScreenOptions.size();
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        selectedOption = (selectedOption + 1) % startScreenOptions.size();
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        if (selectedOption == 0) { // Play
                            gameState = GameState::Playing;
                            // Reset game state variables
                            score = 0;
                            gameOver = false;
                            gameWin = false;
                            totalElapsedTime = 0;
                            Projectile::projectiles.clear();
                        }
                        else if (selectedOption == 1) { // Rules
                            gameState = GameState::RulesScreen;
                        }
                        else if (selectedOption == 2) { // Quit Game
                            window.close();
                        }
                    }
                }
                else if (gameState == GameState::RulesScreen && event.key.code == sf::Keyboard::Escape) {
                    gameState = GameState::StartScreen;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (gameState == GameState::StartScreen) {
                        for (int i = 0; i < startScreenOptions.size(); ++i) {
                            if (startScreenOptions[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                                if (i == 0) { // Play
                                    gameState = GameState::Playing;
                                    // Reset game state variables
                                    score = 0;
                                    gameOver = false;
                                    gameWin = false;
                                    totalElapsedTime = 0;
                                    Projectile::projectiles.clear();
                                }
                                else if (i == 1) { // Rules
                                    gameState = GameState::RulesScreen;
                                }
                                else if (i == 2) { // Quit Game
                                    window.close();
                                }
                            }
                        }
                    }
                    else if (gameState == GameState::RulesScreen) {
                        if (rulesText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            gameState = GameState::StartScreen;
                        }
                    }
                }
            }
        }

        // Assign the time to a variable
        float deltaTime = clock.restart().asSeconds();

        if (gameState == GameState::Playing) {
            if (!gameOver && !gameWin) {
                totalElapsedTime += deltaTime;

                // Update player
                player->update(deltaTime);

                spawnTimer += deltaTime;
                spawnTimer_2 += deltaTime;
                spawnTimer_3 += deltaTime;

                if (spawnTimer >= spawnInterval) {
                    sf::Vector2f projectileSize = sf::Vector2f(30.0f, 30.0f);
                    float max_X = windowSize.x - projectileSize.x - margin;

                    // Generate a random value on the x axis based on the window size
                    float x = margin + static_cast<float>(rand() % static_cast<int>(max_X - margin));

                    int randomDirection = (std::rand() % 2 == 0 ? -1 : 1);
                    sf::Vector2f direction = sf::Vector2f(randomDirection, 1);

                    // Spawn regular falling projectile
                    Projectile::projectiles.emplace_back(std::make_unique<Projectile>(
                        sf::Vector2f(x, 0), // Start position
                        sf::Vector2f(projectileSize), // Size
                        direction, // Direction
                        sf::Vector2f(0.0f, getRandomFloat(50.0f, 150.0f)), // Velocity
                        300.0f, // Acceleration
                        "normal_projectile", // Type
                        sf::Color::White, // Color
                        windowSize // Window
                    ));

                    spawnTimer = 0;
                    spawnInterval = 1.0f;
                }

                if (spawnTimer_2 >= spawnInterval_2) {
                    sf::Vector2f projectileSize = sf::Vector2f(30.0f, 30.0f);
                    float max_X = windowSize.x - projectileSize.x - margin;

                    // Generate a random value on the x axis based on the window size
                    float x = margin + static_cast<float>(rand() % static_cast<int>(max_X - margin));

                    int randomDirection = (std::rand() % 2 == 0 ? -1 : 1);
                    sf::Vector2f direction = sf::Vector2f(randomDirection, 1);

                    // Spawn a special projectile
                    Projectile::projectiles.emplace_back(std::make_unique<Projectile>(
                        sf::Vector2f(x, 0), // Start position
                        sf::Vector2f(projectileSize), // Size
                        direction, // Direction
                        sf::Vector2f(0.0f, getRandomFloat(100.0f, 200.0f)), // Velocity
                        300.0f, // Acceleration
                        "special_projectile", // Type
                        sf::Color::Yellow, // Color 
                        windowSize // Window
                    ));

                    spawnTimer_2 = 0;
                    spawnInterval_2 = getRandomFloat(5.0f, 15.0f);
                }

                if (spawnTimer_3 >= spawnInterval_3) {
                    sf::Vector2f projectileSize = sf::Vector2f(30.0f, 30.0f);
                    float max_X = windowSize.x - projectileSize.x - margin;

                    // Generate a random value on the x axis based on the window size
                    float x = margin + static_cast<float>(rand() % static_cast<int>(max_X - margin));

                    int randomDirection = (std::rand() % 2 == 0 ? -1 : 1);
                    sf::Vector2f direction = sf::Vector2f(randomDirection, 1);

                    // Spawn a destructive projectile
                    Projectile::projectiles.emplace_back(std::make_unique<Projectile>(
                        sf::Vector2f(x, 0), // Start position
                        sf::Vector2f(projectileSize), // Size
                        direction, // Direction
                        sf::Vector2f(0.0f, getRandomFloat(100.0f, 250.0f)), // Velocity
                        300.0f, // Acceleration
                        "destructive_projectile", // Type
                        sf::Color::Red, // Color 
                        windowSize // Window
                    ));

                    spawnTimer_3 = 0;
                    spawnInterval_3 = getRandomFloat(3.0f, 8.0f);
                }

                // Update projectiles
                for (const auto& projectile : Projectile::projectiles) {
                    projectile->update(deltaTime, totalElapsedTime);
                    projectile->checkCollisionWithPlayer(*player);

                    // Check collision with other projectiles
                    for (const auto& other : Projectile::projectiles) {
                        if (projectile != other) {
                            projectile->checkCollisionWithProjectile(*other);
                        }
                    }
                }

                // Update score
                score = Projectile::returnScore();

                // Remove projectiles that are off screen
                Projectile::removeOutOfBounds();

                // Check for game win
                if (score >= MAX_SCORE) {
                    gameWin = true;
                }

                // Check for game over by timer
                if (totalElapsedTime >= GAME_DURATION || Projectile::returnScore() < 0) {
                    gameOver = true;
                }
            }
        }

        // Clear the window before drawing
        window.clear();

        if (gameState == GameState::StartScreen) {
            // Draw start screen
            for (int i = 0; i < startScreenOptions.size(); ++i) {
                if (i == selectedOption) {
                    startScreenOptions[i].setFillColor(sf::Color::Yellow);
                }
                else {
                    startScreenOptions[i].setFillColor(sf::Color::White);
                }
                window.draw(startScreenOptions[i]);
            }
        }
        else if (gameState == GameState::RulesScreen) {
            // Draw rules screen
            window.draw(rulesText);
            sf::Text backText("Press Escape to Return", font, 20);
            backText.setFillColor(sf::Color::White);
            backText.setPosition(windowSize.x / 2 - backText.getLocalBounds().width / 2, windowSize.y - 50);
            window.draw(backText);
        }
        else if (gameState == GameState::Playing) {
            // Draw player
            player->draw(window);

            // Draw projectiles
            for (const auto& projectile : Projectile::projectiles) {
                projectile->draw(window);
            }

            // Draw game over screen
            if (gameOver) {
                sf::Text text("Game Over", font, 50);
                text.setFillColor(sf::Color::Red);
                text.setPosition(windowSize.x / static_cast<float>(2) - text.getLocalBounds().width / 2, windowSize.y / static_cast<float>(2) - text.getLocalBounds().height / 2);
                window.draw(text);
            }

            // Draw game win screen
            if (gameWin) {
                sf::Text text("You Win!", font, 50);
                text.setFillColor(sf::Color::Green);
                text.setPosition(windowSize.x / static_cast<float>(2) - text.getLocalBounds().width / 2, windowSize.y / static_cast<float>(2) - text.getLocalBounds().height / 2);
                window.draw(text);
            }

            // Display the score
            sf::Text scoreText("Score: " + std::to_string(score), font, 20);
            scoreText.setFillColor(sf::Color::Yellow);
            scoreText.setPosition(10, 10);
            window.draw(scoreText);

            // Display the max score to gain
            sf::Text maxScoretext("MaxScore: " + std::to_string(MAX_SCORE), font, 20);
            maxScoretext.setFillColor(sf::Color::Red);
            maxScoretext.setPosition(windowSize.x - 250, 10);
            window.draw(maxScoretext);

            // Display the timer
            int timeLeft = static_cast<int>(GAME_DURATION - totalElapsedTime);
            sf::Text timerText("Time: " + std::to_string(timeLeft), font, 20);
            timerText.setFillColor(sf::Color::Cyan);
            timerText.setPosition(10, 40);
            window.draw(timerText);
        }

        // Display what has been drawn
        window.display();
    }

    return 0;
}
