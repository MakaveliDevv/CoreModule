#include "Projectile.h"

Projectile::Projectile(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Vector2f& velocity,
    const sf::Vector2f& direction,
    sf::RenderWindow& window,
    float acceleration
)
    : Size(size), velocity(velocity), direction(direction), window(window), acceleration(acceleration) {
    shape.setSize(Size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Red);
}

void Projectile::update(float deltaTime) {
    // Update velocity with acceleration
    velocity += direction * acceleration * deltaTime;

    // Update position
    shape.move(velocity * deltaTime);

    // Ensure projectile stays within window bounds
    sf::Vector2f position = shape.getPosition();
    if (position.x < 0 || position.x + shape.getSize().x > window.getSize().x ||
        position.y < 0 || position.y + shape.getSize().y > window.getSize().y) {
        // Mark projectile as out of bounds (you may want to handle this)
    }

    shape.setPosition(position);
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Projectile::getBounds() const {
    return shape.getGlobalBounds();
}
