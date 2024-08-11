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
        float friction,
        const std::string& type,
        const sf::Color& color,
        const sf::Vector2u& playerWindowSize,
        bool isPowerShot = false
    );

    void update(float deltaTime, float elapsedTime, int& score);
    void draw(sf::RenderWindow& window);
    void checkCollisionWithPlayer(Player& player, int& score);
    void checkCollisionWithProjectile(Projectile& other, int& score);

    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    Bounds calculateBounds() const;

    float normalizeDirection(float y);
    bool isOutOfBounds();

    static void removeOutOfBounds();
    static int returnScore(int& score);
    static float accelerationIncrementPercentage;

    static float destructiveProjectileSpawnRate;
    static void increaseDestructiveProjectileSpawnRate(float percentage);
    static void returnType();

    static std::vector<std::unique_ptr<Projectile>> projectiles;

private:
    sf::RectangleShape shape;
    sf::Vector2f size;
    sf::Vector2f vel;
    sf::Vector2f direction;
    sf::Color color;
    float acceleration;
    float friction;
    bool outOfBounds;
    bool markedForRemoval;


    std::string type;

    sf::Vector2f customPosition;
    sf::Vector2f customSize;
    const sf::Vector2u& windowSize;

    void markForRemoval();  
    bool isMarkedForRemoval(); 
    bool powerShot;
    float dynamicAcceleration; 

};

#endif
