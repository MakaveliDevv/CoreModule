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

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    float customSqr(float x);
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
};