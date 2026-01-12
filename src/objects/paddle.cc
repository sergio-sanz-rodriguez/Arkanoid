#include <iostream>
#include "paddle.h"
#include "assets.h"

// Initialize static data
sf::Texture paddle::texture;
sf::RenderWindow* paddle::window_ = nullptr;

paddle::paddle(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col, paddle_colors paddle_color) {

    // Load the texture
    switch (paddle_color) {
        case paddle_colors::light_gray:
            if (!texture.loadFromFile(assets::img_paddle_lightg_path())) {
                throw std::runtime_error("Failed to load the paddle texture.");
            }
            break;
        case paddle_colors::dark_gray:
            if (!texture.loadFromFile(assets::img_paddle_darkg_path())) {
                throw std::runtime_error("Failed to load the paddle texture.");
            }
            break;
        case paddle_colors::gold:
            if (!texture.loadFromFile(assets::img_paddle_gold_path())) {
                throw std::runtime_error("Failed to load the paddle texture.");
            }
            break;
    }

    //sprite.setTexture(texture);
    sprite = std::make_unique<sf::Sprite>(texture);

    // By default, operations are relative to the sprite's top left corner
    // Make them relative to the sprite's centre    
    sprite->setOrigin(get_centre());

    // Set the initial position, velocity, and color of the paddle
    // Use (x, y) for the initial position of the paddle
    sprite->setPosition(pos);
    sprite->setScale(sca);
    sprite->setColor(col);
    velocity = vel;

    // Set the width of the paddle divided by 2 and the height
    half_width = get_bounding_box().size.x / 2.0f;
    height = get_bounding_box().size.y;

}

// The paddle cannot move up
void paddle::move_up() noexcept {}

// The paddle cannot move down
void paddle::move_down() noexcept {}

// Move to the left
void paddle::move_left() noexcept {
    velocity.x = -constants::paddle_speed;
}

// Move to the right
void paddle::move_right() noexcept {
    velocity.x = constants::paddle_speed;
}

// Compute the paddle's new position
void paddle::update() {

    // Respond to user input as this will affect how the paddle moves
    process_player_input();

    // Move the paddle
    sprite->move(velocity);
}

// Drawing function
void paddle::draw(sf::RenderWindow& window) {
    // Ask the window to draw the sprite for us
    window.draw(*sprite);
}

void paddle::set_window(sf::RenderWindow& w) {
    window_ = &w;
}

// Get the (half) width of the paddle
float paddle::get_half_width() const noexcept { return half_width; }

// Get the height of the paddle
float paddle::get_height() const noexcept { return height; }

// Get and set the scale of the paddle
bool paddle::get_scale() const noexcept { return scaleup; }
void paddle::set_scale(bool on, float factor) noexcept {

    // Set scale
    sprite->setScale(on ? sf::Vector2f{ factor * constants::paddle_scale_width, constants::paddle_scale_height } 
                        : sf::Vector2f{ constants::paddle_scale_width, constants::paddle_scale_height });
    
    // Update the width of the paddle, as it may have changed after scaling
    half_width = get_bounding_box().size.x / 2.0f;
}

// Respond to input from the player
// If the player presses the left arrow key, move to the left (negative velocity)
// If the player presses the right arrow key, move to the right (positive velocity)
// Otherwise, do not move (zero velocity) 
// Do not allow the paddle to move off the screen
void paddle::process_player_input() {

    if (!window_) return;

    // Keyboard input
    static float current_speed = std::clamp(
        constants::paddle_speed,
        constants::paddle_min_speed, constants::paddle_max_speed
    );

    // Keyboard input (priority over mouse)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = ((get_position().x - half_width) >= 0.f) ? -current_speed : 0.f;
        return;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = ((get_position().x + half_width) <= constants::window_width) ? current_speed : 0.f;
        return;
    }

    // Mouse input (ONLY if mouse moved)
    velocity.x = 0.0f; // default

    // Static here means: keep this value between frames(it doesn’t reset each function call).
    static float lastMouseX = 0.0f;
    static bool first = true;

    // Get mouse input (coordinates in the view)
    const sf::Vector2i mousePixel = sf::Mouse::getPosition(*window_);
    const sf::Vector2f mouseBoard = window_->mapPixelToCoords(mousePixel);
    float mouseX = mouseBoard.x;
    
    // If mouse didn't move, don't override keyboard / don't snap paddle
    if (first) {
        lastMouseX = mouseX;
        first = false;
        return;
    }

    const float dx = mouseX - lastMouseX;
    lastMouseX = mouseX;
    if (dx == 0.0f) return;

    // Mouse moved: directly position paddle
    if (mouseX < half_width) mouseX = half_width;
    if (mouseX > constants::window_width - half_width)
        mouseX = constants::window_width - half_width;

    sprite->setPosition({ mouseX, get_position().y });
}
