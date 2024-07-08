#include <SFML/Graphics.hpp>

class Player {
public:
    Player(
        const sf::Vector2f& position, 
        const sf::Vector2f& size, 
        const sf::Vector2f& velocity,
        const sf::Vector2f& direction,
        sf::RenderWindow& window
    );

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    float customSqr(float x);
    sf::FloatRect getBounds() const;

   // void setDirection(const sf::Vector2f& dir);
   //void setStoppingFactor(float factor);

private:
    sf::RectangleShape shape;
    sf::Vector2f velocity{ .0f, .0f };
    sf::Vector2f direction{ .0f, .0f};
    sf::RenderWindow& window;
    float acceleration = 500.0f;
    float friction = .25f;
    float stoppingFactor = .5f;
    bool hitWindowBounds = false;
};
