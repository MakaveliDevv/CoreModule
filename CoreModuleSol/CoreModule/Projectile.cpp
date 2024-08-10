#include "Projectile.h"
#include "Player.h" 

//int score = 0;
int Projectile::score = 0;
std::vector<std::unique_ptr<Projectile>> Projectile::projectiles;

Projectile::Projectile(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Vector2f& direction,
    const sf::Vector2f& velocity,
    float acceleration,
    const std::string& type,
    const sf::Color& color,
    const sf::Vector2u& playerWindowSize
)
    : customPosition(position),
    customSize(size),
    direction(direction),
    vel(velocity),
    acceleration(acceleration),
    outOfBounds(false),
    type(type),
    color(color),
    windowSize(playerWindowSize),
    markedForRemoval(false)
{
    shape.setSize(customSize);
    shape.setPosition(customPosition);
    shape.setFillColor(color);
    vel = direction * acceleration;
    //std::cout << "Projectile created: " << type << std::endl;
}


void Projectile::update(float deltaTime, float elapsedTime) {
    if(elapsedTime < 15.0f) {
        customPosition += vel * deltaTime;
        shape.setPosition(customPosition);     
    }
    else if (elapsedTime > 15.0f) {
        std::cout << "Timer reached the 200.0f seconds mark" << std::endl;
        vel += direction * acceleration * deltaTime;
        customPosition += vel * deltaTime;
        shape.setPosition(customPosition);
    }

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

sf::Vector2f Projectile::getPosition() const {
    return customPosition;
}

sf::Vector2f Projectile::getSize() const {
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

bool Projectile::isOutOfBounds() const {
    return outOfBounds;
}

void Projectile::markForRemoval() {
    markedForRemoval = true;
}

bool Projectile::isMarkedForRemoval() const {
    return markedForRemoval;
}

void Projectile::checkCollisionWithPlayer(Player& player) const {
    if (this->type != "shooting_projectile" && player.collision(*this)) {
        const_cast<Projectile*>(this)->markForRemoval();
        //std::cout << "Collision between projectile and player!" << std::endl;

        if (this->type == "destructive_projectile" && player.collision(*this)) {
            score -= 10;
            std::cout << "Collision between destructive_projectile and player!" << std::endl;
        }
    }
}

void Projectile::checkCollisionWithProjectile(Projectile& other) {
    if (this == &other || this->isMarkedForRemoval() || other.isMarkedForRemoval()) {
        return; // Skip if it's the same projectile or if either is marked for removal
    }

    if (this->type == "shooting_projectile" && (other.type == "normal_projectile" 
        || other.type == "special_projectile"
        || other.type == "destructive_projectile"
        )) {
        if (calculateBounds().intercepts(other.calculateBounds())) {
            if (other.type == "normal_projectile") {
                score += 5;
                std::cout << "Collision detected: +5 points" << std::endl;
            }
            else if (other.type == "special_projectile") {
                score += 10;
                std::cout << "Collision detected with a special projectile: +10 points" << std::endl;
            }
            else if(other.type == "destructive_projectile") 
            {
                score -= 5;
                std::cout << "Collision detected with destructive projectile: -5 points" << std::endl;
            }
            // Mark both projectiles for removal
            other.markForRemoval();
            this->markForRemoval();
        }
    }

    if (this->type != "shooting_projectile" && other.type != "shooting_projectile" && calculateBounds().intercepts(other.calculateBounds())) {
        this->vel.x = -this->vel.x;
        other.vel.x = -other.vel.x;
    }
}

int Projectile::returnScore() {
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
