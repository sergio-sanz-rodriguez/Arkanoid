#include "brick.h"

// Initialize static data
sf::Texture brick::texture;

brick::brick(float x, float y) { //}: entity() {

    // Load the texture
    if (!texture.loadFromFile(constants::brick_path())) {
        throw std::runtime_error("Failed to load brick01.png.");
    }
    // Set sprite
    sprite = std::make_unique<sf::Sprite>(texture);

    // By default, operations are relative to the sprite's top left corner
    // Make them relative to the sprite's centre    
    sprite->setOrigin(get_centre());

    // Set the initial position and velocity of the brick
    // Use (x, y) for the initial position of the brick
    sprite->setPosition({ x, y });

}

// Compute the brick's new position
void brick::update() {
  // Nothing to do here
}

void brick::draw(sf::RenderWindow& window) {
  // Ask the window to draw the sprite for us
  window.draw(*sprite);
}
