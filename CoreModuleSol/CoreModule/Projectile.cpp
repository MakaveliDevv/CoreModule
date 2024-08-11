#include "Projectile.h"
#include "Player.h" 

std::vector<std::unique_ptr<Projectile>> Projectile::projectiles;

Projectile::Projectile(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Vector2f& direction,
    const sf::Vector2f& velocity,
    float acceleration,
    float friction,
    const std::string& type,
    const sf::Color& color,
    const sf::Vector2u& playerWindowSize,
    bool isPowerShot
)
    : customPosition(position),
    customSize(size),
    direction(direction),
    vel(velocity),
    acceleration(acceleration),
    friction(friction),
    outOfBounds(false),
    type(type),
    color(color),
    windowSize(playerWindowSize),
    markedForRemoval(false),
    powerShot(isPowerShot)
{
    shape.setSize(customSize);
    shape.setPosition(customPosition);
    shape.setFillColor(color);
    vel = direction * acceleration;
    std::cout << "Projectile created: " << type << std::endl;
}


void Projectile::update(float deltaTime, float elapsedTime) {
    
    // Normalize direction
    if (direction.y != 0.0f) {
        float yLength = normalizeDirection(direction.y * direction.y);
        direction / yLength;
    }

    if(direction.x != 0.0f) {
        float xLength = normalizeDirection(direction.x * direction.x);
        direction / xLength;
    }


    if (powerShot || this->type == "destructive_projectile") {
        // Travel with increased speed over time
        vel += direction * acceleration * deltaTime;
    }
    else {
        // Consistent speed over time
        vel += direction * deltaTime;
    }

    // Apply friction
    vel -= vel * friction * deltaTime;

    // Apply the velocity to the custom position
    customPosition += vel * deltaTime;

    shape.setPosition(customPosition);

    // Debug statements
    //std::cout << "Custom Position: (" << customPosition.x << ", " << customPosition.y << ")" << std::endl;
    //std::cout << "Velocity: (" << vel.x << ", " << vel.y << ")" << std::endl;
    std::cout << "Acceleration: " << acceleration << std::endl;

    /*
    if(elapsedTime < 15.0f) {
    }
    if (Player::returnPowerShot()) {
        vel += direction * acceleration * deltaTime;
        customPosition += vel * deltaTime;
        shape.setPosition(customPosition);
    }
    else {
        customPosition += vel * deltaTime;
        shape.setPosition(customPosition);
    }
    /*
    else if (elapsedTime > 15.0f) {
        std::cout << "Timer reached the 15 seconds mark" << std::endl;
        vel += direction * acceleration * deltaTime;
        customPosition += vel * deltaTime;
        shape.setPosition(customPosition);
    }
    */

    if (customPosition.y < 0 || customPosition.y + customSize.y > windowSize.y) {
        outOfBounds = true;
    }
    else if (customPosition.x < 0 || customPosition.x + customSize.x > windowSize.x) {
        vel.x = -vel.x;
    }
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Projectile::getPosition() {
    return customPosition;
}

sf::Vector2f Projectile::getSize() {
    return customSize;
}

Bounds Projectile::calculateBounds() const {
    Bounds bounds{};
    bounds.left = customPosition.x;
    bounds.top = customPosition.y;
    bounds.right = customPosition.x + customSize.x;
    bounds.bottom = customPosition.y + customSize.y;
    return bounds;
}

float Projectile::normalizeDirection(float x) {
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

bool Projectile::isOutOfBounds() {
    return outOfBounds;
}

void Projectile::markForRemoval() {
    markedForRemoval = true;
}

bool Projectile::isMarkedForRemoval() {
    return markedForRemoval;
}

void Projectile::checkCollisionWithPlayer(Player& player, int& score) {
    if (this->type != "shooting_projectile" && player.collision(*this)) {
        const_cast<Projectile*>(this)->markForRemoval();

        if (this->type == "destructive_projectile" && player.collision(*this)) {
            score -= 10;
            std::cout << "Collision between destructive_projectile and player!" << std::endl;
        }
    }
}

void Projectile::checkCollisionWithProjectile(Projectile& other, int& score) {
    if (this == &other || this->isMarkedForRemoval() || other.isMarkedForRemoval()) {
        return; 
    }

    if (this->type == "shooting_projectile" && (other.type == "normal_projectile" 
        || other.type == "special_projectile"
        || other.type == "destructive_projectile"
        )) {
        if (calculateBounds().intercepts(other.calculateBounds())) {
            if (other.type == "normal_projectile") {
                score += 5;
                //std::cout << "Collision detected: +5 points" << std::endl;
            }
            else if (other.type == "special_projectile") {
                score += 10;
                //std::cout << "Collision detected with a special projectile: +10 points" << std::endl;
            }
            else if(other.type == "destructive_projectile") 
            {
                score -= 5;
                //std::cout << "Collision detected with destructive projectile: -5 points" << std::endl;
            }

            other.markForRemoval();
            this->markForRemoval();
        }
    }

    if (this->type != "shooting_projectile" && other.type != "shooting_projectile" && calculateBounds().intercepts(other.calculateBounds())) {
        this->vel.x = -this->vel.x;
        other.vel.x = -other.vel.x;
    }
}

int Projectile::returnScore(int& score) {
    return score;
}

void Projectile::removeOutOfBounds() {
    projectiles.erase(
        std::remove_if(
            projectiles.begin(),
            projectiles.end(),
            [](const std::unique_ptr<Projectile>& p) {
                return p->isOutOfBounds() || p->isMarkedForRemoval();
            }
        ),
        projectiles.end()
    );
}
