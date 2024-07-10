#include "Player.h"
#include "Projectile.h"

Player::Player(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Vector2f& velocity,
    const sf::Vector2f& direction,
    sf::RenderWindow& window,
    float acceleration,
    float friction,
    float stoppingFactor,
    float shootingCooldown
)
    : Size(size), vel(velocity), direction(direction), window(window), acceleration(acceleration), friction(friction), stoppingFactor(stoppingFactor), shootingCooldown(shootingCooldown), shootingTimer(0.0f) {
    shape.setSize(Size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Green);

    vel = direction * acceleration;
}

void Player::movement(float deltaTime) {
    // Update shooting timer
    shootingTimer += deltaTime;

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
        float Xlength = normalizeDirection(direction.x * direction.x);
        direction /= Xlength;
    }

    // Update velocity with acceleration
    vel += direction * acceleration * deltaTime;

    // Apply friction
    if (direction.x == 0.0f) {
        float actualFriction = std::min(friction, 1.0f);
        vel -= vel * actualFriction * deltaTime;
    }

    // Apply stopping factor
    float actualStoppingFactor = std::min(stoppingFactor, 1.0f);
    vel -= vel * actualStoppingFactor * deltaTime;

    // Update position
    shape.move(vel * deltaTime);

    // Ensure player stays within window bounds
    sf::Vector2f position = shape.getPosition();
    if (position.x < 0) {
        position.x = 0;
        vel.x = std::max(vel.x, 0.0f); // Prevent negative velocity
    }
    if (position.x + shape.getSize().x > window.getSize().x) {
        position.x = window.getSize().x - shape.getSize().x;
        vel.x = std::min(vel.x, 0.0f); // Prevent positive velocity
    }

    shape.setPosition(position);

    // Handle shooting
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootingTimer >= shootingCooldown) {
        shoot();
        shootingTimer = 0.0f; 
    }
}

// Method to normalize direction
float Player::normalizeDirection(float x) {
    if (x == 0.0f || x == 1.0f) {
        return x;
    }

    float normalize = x / 2.0f;
    float error = 0.000f;

    while (std::abs(normalize * normalize - x) > error) {
        normalize = (normalize + x / normalize) / 2.0f;
    }

    return normalize;
}

// Method to shoot projectiles
void Player::shoot() {
    sf::Vector2f projectilePosition = shape.getPosition() + sf::Vector2f(Size.x / 2.0f, 0.0f);
    Projectile::projectile.emplace_back(std::make_unique<Projectile>(
        projectilePosition,
        sf::Vector2f(10.0f, 10.0f),
        sf::Vector2f(0.0f, -1.0f),
        sf::Vector2f(0.0f, -300.0f),
        window,
        0.0f,
        sf::Color::Blue
    ));
}


void Projectile::checkCollisions() {
    for (size_t i = 0; i < projectile.size(); ++i) {
        for (size_t j = i + 1; j < projectile.size(); ++j) {
            if (projectile[i]->getBounds().intersects(projectile[j]->getBounds())) {
                projectile[i]->outOfBounds = true;
                projectile[j]->outOfBounds = true;
            }
        }
    }

    // Remove projectiles that are marked as out of bounds
    auto it = projectile.begin();
    while (it != projectile.end()) {
        if ((*it)->isOutOfBounds()) {
            it = projectile.erase(it);
        }
        else {
            ++it;
        }
    }
}

// To draw the player
void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}
