#include "Player.h"

Player::Player(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& velocity)
    : velocity(velocity) {
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Green);
}

void Player::update(float deltaTime) {
    sf::Vector2f accel(0.0f, 0.0f);

    // Handle input for acceleration
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && shape.getPosition().x > 0) {
        accel.x -= acceleration;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && shape.getPosition().x + shape.getSize().x < 1200) {
        accel.x += acceleration;
    }

    // Update velocity with acceleration
    velocity += accel * deltaTime;

    // Apply friction
    velocity *= pow(friction, deltaTime);

    // Update position with velocity
    shape.move(velocity * deltaTime);

    // Ensure player stays within window bounds
    sf::Vector2f position = shape.getPosition();
    if (position.x < 0) position.x = 0;
    if (position.x + shape.getSize().x > 1200) position.x = 1200 - shape.getSize().x;
    shape.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}
