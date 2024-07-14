#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string> 
#include "Player.h"
#include "Bounds.h"

class Projectile {
public:
    Projectile(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const sf::Vector2f& direction,
        const sf::Vector2f& velocity,
        sf::RenderWindow& window,
        float acceleration,
        const std::string& type,
        const sf::Color& color
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
    //static void checkCollisions();

    static std::vector<std::unique_ptr<Projectile>> projectiles;

private:
    sf::RectangleShape shape;
    sf::Vector2f size;
    sf::Vector2f vel;
    sf::Vector2f direction;
    sf::RenderWindow& window;
    sf::Color color;
    float acceleration;
    bool outOfBounds;

    std::string type;

    sf::Vector2f customPosition;
    sf::Vector2f customSize;
};

#endif 
