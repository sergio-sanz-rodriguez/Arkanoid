#include "bonus.h"

// Initialize static data
sf::Texture bonus::texture;

bonus::bonus(const std::string& path, sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col) {

    // Load the texture
    if (!texture.loadFromFile(path)) {
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
void bonus::move_up() noexcept {
    velocity.y = -std::abs(velocity.y);
}

void bonus::move_down() noexcept {
    velocity.y = std::abs(velocity.y);
}

void bonus::move_left() noexcept {
    velocity.x = -std::abs(velocity.x);
}

void bonus::move_right() noexcept {
    velocity.x = std::abs(velocity.x);
}

// Compute the ball's new position
void bonus::update() {

    // Move the position of the ball
    sprite->move(velocity);

    if ((get_position().y + half_height) >= constants::window_height) {
        destroy();
    }
}

// Logic when the user speed ups or down the speed
void bonus::process_player_input() {

    // Up key increases the velocity of the ball, and down key decreases it
    const bool upKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    const bool downKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

    if (!upKey && !downKey) return;

    if (velocity.y <= 0.f) return; // safety

    // new desired speed
    if (upKey)   velocity.y += constants::bonus_speed_step;
    if (downKey) velocity.y -= constants::bonus_speed_step;

    velocity.y = std::clamp(velocity.y, constants::bonus_min_speed, constants::bonus_max_speed);

}

// Drawing function
void bonus::draw(sf::RenderWindow& window) {
    // Ask the window to draw the sprite for us
    window.draw(*sprite);
}
