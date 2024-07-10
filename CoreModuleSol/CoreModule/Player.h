#include <SFML/Graphics.hpp>

class Player {
public:
    Player(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const sf::Vector2f& velocity,
        const sf::Vector2f& direction,
        sf::RenderWindow& window,
        float acceleration,
        float friction,
        float stoppingFactor
    );

    void movement(float deltaTime);
    void draw(sf::RenderWindow& window);
    float normalizeDirection(float x);
    float shoot(float shootingPower);
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
    sf::Vector2f Size;
    sf::Vector2f velocity;
    sf::Vector2f direction;
    sf::RenderWindow& window;
    float acceleration;
    float friction;
    float stoppingFactor;
    float shootingPower;
};