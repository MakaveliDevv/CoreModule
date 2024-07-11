#include "Projectile.h"
#include "Player.h"
#include <algorithm>
#include <iostream>

std::vector<std::unique_ptr<Projectile>> Projectile::projectiles;

Projectile::Projectile(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Vector2f& direction,
    const sf::Vector2f& velocity,
    sf::RenderWindow& window,
    float acceleration,
    const std::string& type,
    const sf::Color& color
)
    : customPosition(position), customSize(size), direction(direction), window(window), acceleration(acceleration), vel(velocity), outOfBounds(false), type(type), color(color) {
    shape.setSize(customSize);
    shape.setPosition(customPosition);
    shape.setFillColor(color);

    vel = direction * acceleration;

    projectiles.push_back(std::make_unique<Projectile>(*this));
}

void Projectile::update(float deltaTime) {
    customPosition += vel * deltaTime;

    if (customPosition.y < 0 || customPosition.y + customSize.y > window.getSize().y) {
        outOfBounds = true;
    }
    else if (customPosition.x < 0 || customPosition.x + customSize.x > window.getSize().x) {
        vel.x = -vel.x;
    }

    shape.setPosition(customPosition);
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape);
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

void Projectile::removeOutOfBounds() {
    projectiles.erase(std::remove_if(
        projectiles.begin(),
        projectiles.end(),
        [](const std::unique_ptr<Projectile>& p) {
            return p->isOutOfBounds();
        }),
        projectiles.end());
}

void Projectile::checkCollisionWithPlayer(Player& player) {
    if (type == "falling" && player.intersects(*this)) {

        std::cout << "Collision between falling projectile and player!" << std::endl;
    }
}

void Projectile::checkCollisionWithProjectile(Projectile& other) const {
    if (type == "shooting" && other.type == "falling" && calculateBounds().intersects(other.calculateBounds())) {
      

        std::cout << "Collision between shooting projectile and falling projectile!" << std::endl;
    }
}

/*
void Projectile::checkCollisions() {
    for (size_t i = 0; i < projectiles.size(); ++i) {
        for (size_t j = i + 1; j < projectiles.size(); ++j) {
            if (projectiles[i]->calculateBounds().intersects(projectiles[j]->calculateBounds())) {
                projectiles[i]->outOfBounds = true;
                projectiles[j]->outOfBounds = true;
            }
        }
    }

    // Remove projectiles that are marked as out of bounds
    auto it = projectiles.begin();
    while (it != projectiles.end()) {
        if ((*it)->isOutOfBounds()) {
            it = projectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}
*/
