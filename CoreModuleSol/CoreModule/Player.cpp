#include "Player.h"
#include "Projectile.h"

Player::Player(
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
)
    : customSize(size), 
    vel(velocity), 
    direction(direction), 
    acceleration(acceleration), 

    friction(friction), 
    stoppingFactor(stoppingFactor), 

    shootingCooldown(shootingCooldown), 
    shootingTimer(0.0f),

    powerShotCooldown(powerShotCooldown),
    powerShotTimer(0.0f),

    windowSize(initialWindowSize)
{

    customPosition = position;
    shape.setPosition(customPosition);
    shape.setSize(size);
    shape.setFillColor(sf::Color::Green);

    vel = direction * acceleration;
}

void Player::update(float deltaTime) {
    // Update shooting timer
    shootingTimer += deltaTime;
    powerShotTimer += deltaTime;

    // Reset direction
    direction = sf::Vector2f(0.0f, 0.0f);

    // Input for movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && customPosition.x > 0) {
        direction.x -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && customPosition.x + customSize.x < windowSize.x) {
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

    customPosition += vel * deltaTime;

    // Ensure player stays within window bounds
    if (customPosition.x < 0) {
        customPosition.x = 0;
        vel.x = std::max(vel.x, 0.0f);
    }
    if (customPosition.x + customSize.x > windowSize.x) {
        customPosition.x = windowSize.x - customSize.x;
        vel.x = std::min(vel.x, 0.0f);
    }

    shape.setPosition(customPosition);

    // Handle shooting
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (powerShotTimer >= powerShotCooldown) {
            shootWithPower();
            powerShotTimer = 0.0f;
            shootingTimer = -0.5f; // Apply delay before next regular shot can be triggered
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootingTimer >= shootingCooldown) {
        shoot();
        shootingTimer = 0.0f;
    }
}

void Player::setWindowSize(const sf::Vector2u& size) {
    windowSize = size;
}

float Player::getRandomFloat(float min, float max)
{
    return 0.0f;
}

sf::Vector2f Player::getPosition() {
    return customPosition;
}

sf::Vector2f Player::getSize() {
    return customSize;
}

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

void Player::shoot() {
    sf::Vector2f projectilePosition = getPosition() + sf::Vector2f(customSize.x / 2.0f, 0.0f);
    Projectile::projectiles.emplace_back(std::make_unique<Projectile>(
        projectilePosition, // Start position
        sf::Vector2f(10.0f, 10.0f), // Size
        sf::Vector2f(0.0f, -1.0f), // Direction
        sf::Vector2f(0.0f, 0.0f), // Velocity
        500.0f, // Acceleration
        0.0f, // Friction
        "shooting_projectile", // Type
        sf::Color::Blue, // Color
        windowSize, // Window
        false
    ));
}

void Player::shootWithPower() {
    sf::Vector2f projectilePosition = getPosition() + sf::Vector2f(customSize.x / 2.0f, 0.0f);
    Projectile::projectiles.emplace_back(std::make_unique<Projectile>(
        projectilePosition, // Start position
        sf::Vector2f(20.0f, 20.0f), // Size
        sf::Vector2f(0.0f, -1.0f), // Direction
        sf::Vector2f(0.0f, 0.0f), // Velocity
        500.0f, // Acceleration
        0.0f, // Friction
        "shooting_projectile", // Type
        sf::Color::Cyan, // Color
        windowSize, // Window
        true
    ));
}

Bounds Player::calculateBounds() {
    Bounds bounds{};
    bounds.left = customPosition.x;
    bounds.top = customPosition.y;
    bounds.right = customPosition.x + customSize.x;
    bounds.bottom = customPosition.y + customSize.y;
    return bounds;
}

bool Player::collision(const Projectile& projectile) {
    Bounds playerBounds = calculateBounds();
    Bounds projectileBounds = projectile.calculateBounds();

    return playerBounds.intercepts(projectileBounds);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
