#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include <vector>
#include <memory>

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
        float stoppingFactor,
        float shootingCooldown
    );

    void movement(float deltaTime);
    void draw(sf::RenderWindow& window);
    float normalizeDirection(float x);
    void shoot();
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
    sf::Vector2f Size;
    sf::Vector2f vel;
    sf::Vector2f direction;
    sf::RenderWindow& window;
    float acceleration;
    float friction;
    float stoppingFactor;
    float shootingCooldown;
    float shootingTimer;
};

#endif
