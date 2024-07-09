#include "Player.h"

Player::Player(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Vector2f& velocity,
    const sf::Vector2f& direction,
    sf::RenderWindow& window,
    float acceleration,
    float friction,
    float stoppingFactor
)
    : Size(size), velocity(velocity), direction(direction), window(window), acceleration(acceleration), friction(friction), stoppingFactor(stoppingFactor) {
    shape.setSize(Size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Green);
}

void Player::update(float deltaTime) {
    // Reset direction
    direction = sf::Vector2f(0.0f, 0.0f);

    // Input for movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && shape.getPosition().x > 0) {
        direction.x -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && shape.getPosition().x + shape.getSize().x < window.getSize().x) {
        direction.x += 1.0f;
    }

    // Normalize direction
    if (direction.x != 0.0f) {
        float Xlength = customSqr(direction.x * direction.x);
        direction /= Xlength;
    }

    // Update velocity with acceleration
    velocity += direction * acceleration * deltaTime;

    // Apply friction
    if (direction.x == 0.0f) {
        float actualFriction = std::min(friction, 1.0f);
        velocity -= velocity * actualFriction * deltaTime;
    }

    // Apply stopping factor
    float actualStoppingFactor = std::min(stoppingFactor, 1.0f);
    velocity -= velocity * actualStoppingFactor * deltaTime;

    // Update position
    shape.move(velocity * deltaTime);

    // Ensure player stays within window bounds
    sf::Vector2f position = shape.getPosition();
    if (position.x < 0) {
        position.x = 0;
        velocity.x = std::max(velocity.x, 0.0f); // Prevent negative velocity
    }
    if (position.x + shape.getSize().x > window.getSize().x) {
        position.x = window.getSize().x - shape.getSize().x;
        velocity.x = std::min(velocity.x, 0.0f); // Prevent positive velocity
    }

    shape.setPosition(position);
}

// Method to normalize direction
float Player::customSqr(float x) {
    if (x == 0.0f || x == 1.0f) {
        return x;
    }

    float guess = x / 2.0f;
    float error = 0.000f;

    while (std::abs(guess * guess - x) > error) {
        guess = (guess + x / guess) / 2.0f;
    }

    return guess;
}

// To draw the player
void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}