#include "Projectile.h"

Projectile::Projectile(
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Vector2f& velocity,
	sf::RenderWindow& window
)
	:velocity(velocity), window(window) {
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(sf::Color::Red);
}

void Projectile::update(float deltaTime) {
	sf::Vector2f accel(0.0f, 0.0f);


	
}