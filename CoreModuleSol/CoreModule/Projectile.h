#include <SFML/Graphics.hpp>

class Projectile {
public:
	Projectile(
		const sf::Vector2f& position,
		const sf::Vector2f& size,
		const sf::Vector2f& velocity,
		sf::RenderWindow& window
	);
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	sf::FloatRect getBounds() const;

private:
	sf::RectangleShape shape;
	sf::Vector2f velocity;
	sf::RenderWindow& window;
	const float acceleration = 250.0f;
	const float frictoin = .2f;

};