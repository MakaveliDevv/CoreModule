#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Bounds.h"

class Player;

class Projectile {
public:
    Projectile(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const sf::Vector2f& direction,
        const sf::Vector2f& velocity,
        float acceleration,
        const std::string& type,
        const sf::Color& color,
        const sf::Vector2u& playerWindowSize
    );

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void checkCollisionWithPlayer(Player& player) const;
    void checkCollisionWithProjectile(Projectile& other) const;

    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    Bounds calculateBounds() const;

    bool isOutOfBounds() const;
    static void removeOutOfBounds();

    static std::vector<std::unique_ptr<Projectile>> projectiles;

private:
    sf::RectangleShape shape;
    sf::Vector2f size;
    sf::Vector2f vel;
    sf::Vector2f direction;
    sf::Color color;
    float acceleration;
    bool outOfBounds;
    bool markedForRemoval;  // New flag for marking projectile for removal

    std::string type;

    sf::Vector2f customPosition;
    sf::Vector2f customSize;
    const sf::Vector2u& windowSize;

    void markForRemoval();  // New method to mark projectile for removal
    bool isMarkedForRemoval() const;  // New method to check if projectile is marked for removal
};

#endif
