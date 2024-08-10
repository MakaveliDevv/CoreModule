#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Bounds.h"
#include "Projectile.h"
#include <vector>
#include <memory>
#include <iostream>

class Player {
public:
    Player(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const sf::Vector2f& velocity,
        const sf::Vector2f& direction,
        float acceleration,
        float friction,
        float stoppingFactor,
        float shootingCooldown,
        float powerShotCooldown,
        const sf::Vector2u& initialWindowSize 
    );

    void update(float deltaTime);
    void shoot();
    void shootWithPower();
    void draw(sf::RenderWindow& window);
    float normalizeDirection(float x);

    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    Bounds calculateBounds();

    bool collision(const Projectile& projectile);
    void setWindowSize(const sf::Vector2u& size);        

private:
    sf::RectangleShape shape;
    sf::Vector2f vel;
    sf::Vector2f direction;
    sf::Vector2f customPosition;
    sf::Vector2f customSize;
    sf::Vector2u windowSize;

    float acceleration;
    float friction;
    float stoppingFactor;

    float shootingCooldown;
    float shootingTimer;

    float powerShotCooldown;
    float powerShotTimer;
};

#endif // PLAYER_H
