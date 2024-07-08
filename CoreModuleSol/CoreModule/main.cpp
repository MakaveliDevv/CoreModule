#include <SFML/Graphics.hpp>
#include "Player.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

int main() 
{
	// Window init
	sf::RenderWindow window(sf::VideoMode(1200, 1200), "Makaveli");

	// Player init
	std::unique_ptr<Player> player = std::make_unique<Player>(sf::Vector2f(600, 1150), sf::Vector2f(50, 50), sf::Vector2f(0.0f, 0.0f), window);

	// Game's clock
	sf::Clock clock;
	float spawnTimer = 0;
	float spawnInterval = 1.0f;

	srand(static_cast<unsigned>(time(0)));

	// Gameplay stuff such as scores and stuff
	int score = 0;
	bool gameOver = false;
	bool gameWin = false;

	while(window.isOpen()) 
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed) 
			{
				window.close();
			}
		}

		// Assign the time to a variable
		float deltaTime = clock.restart().asSeconds();

		if(!gameOver && !gameWin) 
		{
			player->update(deltaTime);

			// Spawn projectiles
			// 
			// Update projectiles and check collisions

			// Collision detected with the player

			// Check if projectile is off screen

			// Remove projectiles that are off screen
		}

		// Clear the window before drawing
		window.clear();

		// Draw player
		player->draw(window);

		// Draw projectiles

		// Display what has been drawn
		window.display();


	}
}