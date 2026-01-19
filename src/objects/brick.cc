#include "assets.h"
#include "brick.h"
#include "brick_config.h"

// Initialize static data
sf::Texture brick::texture;

void brick::set_strength(int value) noexcept { strength = value; }
int  brick::get_strength() const noexcept { return strength; }
void brick::set_indestructible(bool value) noexcept { indestructible = value; }
bool brick::is_indestructible() const noexcept { return indestructible; }
void brick::weaken() noexcept { --strength;  }
bool brick::is_too_weak() const noexcept { return strength <= 0;  }
bool brick::is_solid() const noexcept { return is_indestructible() || strength > 0; }

//brick::brick(float x, float y, sf::Color c) : color(c) {
brick::brick(sf::Vector2f pos, sf::Vector2f sca, sf::Color col) {

    // Load the texture
    if (!texture.loadFromFile(assets::img_brick_path())) {
        throw std::runtime_error("Failed to load the brick texture.");
    }
    // Set sprite
    sprite = std::make_unique<sf::Sprite>(texture);

    // By default, operations are relative to the sprite's top left corner
    // Make them relative to the sprite's centre    
    sprite->setOrigin(get_centre());

    // Set the initial position, scale, and color of the brick
    // Use (x, y) for the initial position of the brick
    sprite->setPosition(pos);
    sprite->scale(sca);
    sprite->setColor(col);
}


// Compute the brick's new position
void brick::update() {
    // Change the transparence of the brick based on its weakness
    sf::Color color = sprite->getColor();
    if (strength == 1) {
        color.a = brick_config::brick_alpha_hit3;
    }
    else if (strength == 2) {
        color.a = brick_config::brick_alpha_hit2;
    }
    else if (strength == 3) {
        color.a = brick_config::brick_alpha_hit1;
    }
    sprite->setColor(color);
}

void brick::draw(sf::RenderWindow& window) {
  // Ask the window to draw the sprite for us
  window.draw(*sprite);
}
