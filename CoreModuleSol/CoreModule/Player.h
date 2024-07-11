#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Bounds.h"
#include <vector>
#include <memory>

class Projectile;

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

    void update(float deltaTime);
    void shoot();
    void draw(sf::RenderWindow& window);
    float normalizeDirection(float x);

    // Custom methods for bounds
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    Bounds calculateBounds() const;

    bool intersects(const Projectile& projectile) const;

private:
    sf::RectangleShape shape;
    sf::Vector2f size;
    sf::Vector2f vel;
    sf::Vector2f direction;
    sf::RenderWindow& window;
    float acceleration;
    float friction;
    float stoppingFactor;
    float shootingCooldown;
    float shootingTimer;

    sf::Vector2f customPosition;
    sf::Vector2f customSize;
    sf::Vector2u windowSize;
};

#endif
