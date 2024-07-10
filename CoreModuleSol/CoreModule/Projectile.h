#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Projectile {
public:
    Projectile(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const sf::Vector2f& direction,
        const sf::Vector2f& velocity,
        sf::RenderWindow& window,
        float acceleration,
        const sf::Color& color
    );

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    float normalizeDirection(float x);
    sf::FloatRect getBounds() const;

    bool isOutOfBounds() const;
    static void removeOutOfBounds();
    void checkCollisions();

    static std::vector<std::unique_ptr<Projectile>> projectile;

protected:
    sf::RectangleShape shape;
    sf::Vector2f Size;
    sf::Vector2f vel;
    sf::Vector2f direction;
    sf::RenderWindow& window;
    float acceleration;
    bool outOfBounds;
};

#endif
