#include "brick.h"

// Initialize static data
sf::Texture brick::texture;

// Use different colors, depending on the strength of the brick in RGBA format


void brick::set_strength(int s) noexcept { strength = s; }
void brick::weaken() noexcept { --strength;  }
bool brick::is_too_weak() const noexcept { return strength <= 0;  }

brick::brick(float x, float y, sf::Color c) : color(c) {

    // Load the texture
    if (!texture.loadFromFile(constants::brick_path())) {
        throw std::runtime_error("Failed to load the brick texture.");
    }
    // Set sprite
    sprite = std::make_unique<sf::Sprite>(texture);

    // By default, operations are relative to the sprite's top left corner
    // Make them relative to the sprite's centre    
    sprite->setOrigin(get_centre());

    // Set the initial position and velocity of the brick
    // Use (x, y) for the initial position of the brick
    sprite->setPosition({ x, y });

    // Set color for the sprite
    sprite->setColor(c);
}


// Compute the brick's new position
void brick::update() {
    // Change the transparence of the brick based on its weakness
    if (strength == 1) {
        color.a = constants::brick_alpha_hit1;
    }
    else if (strength == 2) {
        color.a = constants::brick_alpha_hit2;
    }
    else if (strength == 3) {
        color.a = constants::brick_alpha_hit3;
    }
    sprite->setColor(color);
}

void brick::draw(sf::RenderWindow& window) {
  // Ask the window to draw the sprite for us
  window.draw(*sprite);
}
