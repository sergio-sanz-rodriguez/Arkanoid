#include "live.h"

// Initialize static data
sf::Texture live::texture;

live::live(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col) {

    // Load the texture
    if (!texture.loadFromFile(constants::live_path())) {
        throw std::runtime_error("Failed to load the live texture.");
    }
    //sprite.setTexture(texture);
    sprite = std::make_unique<sf::Sprite>(texture);

    // Set the initial position, velocity, and color of the live object
    // Use (x, y) for the initial position of the live object
    sprite->setOrigin(get_centre());
    sprite->setPosition(pos);
    sprite->scale(sca);
    sprite->setColor(col);
    velocity = vel;

    // Set the width and height of the live object divided by 2
    half_width = get_bounding_box().size.x / 2.0f;
    half_height = get_bounding_box().size.y / 2.0f;
}

// Update velocities
void live::move_up() noexcept {
    velocity.y = -std::abs(velocity.y);
}

void live::move_down() noexcept {
    velocity.y = std::abs(velocity.y);
}

void live::move_left() noexcept {
    velocity.x = -std::abs(velocity.x);
}

void live::move_right() noexcept {
    velocity.x = std::abs(velocity.x);
}

// Compute the ball's new position
void live::update() {

    // Move the position of the ball
    sprite->move(velocity);

    if ((get_position().y + half_height) >= constants::window_height) {
        destroy();
    }
}

// Logic when the user speed ups or down the speed
void live::process_player_input() {

    // Up key increases the velocity of the ball, and down key decreases it
    const bool upKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    const bool downKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

    if (!upKey && !downKey) return;

    if (velocity.y <= 0.f) return; // safety

    // new desired speed
    if (upKey)   velocity.y += constants::live_speed_step;
    if (downKey) velocity.y -= constants::live_speed_step;

    velocity.y = std::clamp(velocity.y, constants::live_min_speed, constants::live_max_speed);

}

// Drawing function
void live::draw(sf::RenderWindow& window) {
    // Ask the window to draw the sprite for us
    window.draw(*sprite);
}
