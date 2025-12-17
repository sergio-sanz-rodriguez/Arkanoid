#include "entity.h"

// Helper function to get the bounding box of a sprite
sf::FloatRect entity::get_bounding_box() const noexcept {
    return sprite->getGlobalBounds();
}

// Helper function to get the centre of a sprite
sf::Vector2f entity::get_centre() const noexcept {
    auto box = get_bounding_box();
    return box.getCenter();  
}

// Helper functions to get the position of the sprite
float entity::x() const noexcept {
    return sprite->getPosition().x;
}
float entity::y() const noexcept {
    return sprite->getPosition().y;
}

// Helper functions to get the edges of the sprite
float entity::left() const noexcept {
    auto box = get_bounding_box();
    return x() - box.size.x / 2.0f;
}

float entity::right() const noexcept {
    auto box = get_bounding_box();
    return x() + box.size.x / 2.0f;
}

float entity::top() const noexcept {
    auto box = get_bounding_box();
    return y() - box.size.y / 2.0f;
}

float entity::bottom() const noexcept {
    auto box = get_bounding_box();
    return y() + box.size.y / 2.0f;
}

// Helper functions for the state of the entity
void entity::destroy() noexcept { destroyed = true; }
bool entity::is_destroyed() const noexcept { return destroyed; }

// Helper function to get the velocity of the moving entity
sf::Vector2f moving_entity::get_velocity() const noexcept { return velocity;  }

// Helper function to rotate the velocity vector
void moving_entity::rotate(float degrees, bool random) noexcept {
    if (random) {
        // Create random number generator and uniform distribution
        static thread_local std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> dist(-degrees, degrees);
        degrees = dist(rng);
    }
    const double a = degrees * (constants::pi / 180.0); // degrees -> radians
    const double c = std::cos(a);
    const double s = std::sin(a);
    const double x = velocity.x;
    const double y = velocity.y;
    velocity.x = static_cast<float>(x * c - y * s);
    velocity.y = static_cast<float>(x * s + y * c);
}