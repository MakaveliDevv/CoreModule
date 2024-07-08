#include <SFML/Graphics.hpp>

class Player {
public:
    Player(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& velocity);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    const float acceleration = 500.0f; // Reasonable value for acceleration
    const float friction = 0.2f; // Adjust friction for better control
};
