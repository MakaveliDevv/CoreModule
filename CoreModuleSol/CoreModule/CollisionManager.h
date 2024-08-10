#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Projectile.h"
#include <vector>
#include <memory>
#include <string>

class CollisionManager {
public:
    // This could be a set or a vector depending on your preference.
    std::vector<std::pair<Projectile*, Projectile*>> processedCollisions;

    void checkCollisions(std::vector<Projectile*>& projectiles) {
        for (size_t i = 0; i < projectiles.size(); ++i) {
            for (size_t j = i + 1; j < projectiles.size(); ++j) {
                Projectile* p1 = projectiles[i];
                Projectile* p2 = projectiles[j];

                // Check if this pair has already been processed
                if (processedCollisions.count(std::make_pair(p1, p2)) > 0 ||
                    processedCollisions.count(std::make_pair(p2, p1)) > 0) {
                    continue; // Skip if already processed
                }

                // If p1 is shooting and p2 is falling, or vice versa, check collision
                if ((p1->getType() == "shooting" && p2->getType() == "falling") ||
                    (p2->getType() == "shooting" && p1->getType() == "falling")) {

                    if (p1->calculateBounds().collides(p2->calculateBounds())) {
                        // Award points and mark for removal
                        score += 5;
                        std::cout << "Collision detected: +5 points" << std::endl;
                        p1->markForRemoval();
                        p2->markForRemoval();

                        // Mark this pair as processed
                        processedCollisions.insert(std::make_pair(p1, p2));
                    }
                }
            }
        }
    }
};
#endif