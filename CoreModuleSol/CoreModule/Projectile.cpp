#include "Projectile.h"
#include <algorithm>

std::vector<std::unique_ptr<Projectile>> Projectile::projectiles;

Projectile::Projectile(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Vector2f& direction,
    const sf::Vector2f& velocity,
    sf::RenderWindow& window,
    float acceleration
)
    : Size(size), direction(direction), window(window), acceleration(acceleration), vel(velocity), outOfBounds(false) {
    shape.setSize(Size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Red);

    // Initialize velocity based on direction and acceleration
    vel = direction * acceleration;

    projectiles.push_back(std::make_unique<Projectile>(*this));
}

void Projectile::update(float deltaTime) {
    // Update position with constant velocity
    shape.move(vel * deltaTime);

    // For increasing velocity when traveling
    //vel += direction * acceleration * deltaTime;

    sf::Vector2f position = shape.getPosition();
    if (position.y < 0 || position.y + shape.getSize().y > window.getSize().y) outOfBounds = true;
   
    // Ensure projectile stays within window x axis bounds
    else if (position.x < 0 || position.x + shape.getSize().x > window.getSize().x) vel.x = -vel.x;
    
    shape.setPosition(position);
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Projectile::getBounds() const {
    return shape.getGlobalBounds();
}

bool Projectile::isOutOfBounds() const {
    return outOfBounds;
}

void Projectile::removeOutOfBounds() {
    projectiles.erase(std::remove_if(
        projectiles.begin(),
        projectiles.end(),
        [](const std::unique_ptr<Projectile>& p) {
            return p->isOutOfBounds();
        }),
        projectiles.end());
}

float Projectile::normalizeDirection(float x) {
    if (x == 0.0f || x == 1.0f) {
        return x;
    }

    float c = x / 2.0f;
    float value = 0.000f;

    while (std::abs(c * c - x) > value) {
        c = (c + x / c) / 2.0f;
    }

    return c;
}