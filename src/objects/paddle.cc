#include <iostream>
#include "paddle.h"

// Initialize static data
sf::Texture paddle::texture;
sf::RenderWindow* paddle::window_ = nullptr;

//paddle::paddle(float x, float y, float vx, float vy = 0.0f) {
paddle::paddle(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col) {

    // Load the texture
    if (!texture.loadFromFile(constants::paddle_path())) {
        throw std::runtime_error("Failed to load the paddle texture.");
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
    velocity = vel; //velocity = { vx, vy };

    // Set the width of the paddle divided by 2
    half_width = get_bounding_box().size.x / 2.0f;

}

// Get paddle speed
//float paddle::get_speed() const noexcept {
//    return std::max(std::abs(velocity.x), std::abs(velocity.y));
//}

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

// Respond to input from the player
// If the player presses the left arrow key, move to the left (negative velocity)
// If the player presses the right arrow key, move to the right (positive velocity)
// Otherwise, do not move (zero velocity) 
// Do not allow the paddle to move off the screen
void paddle::process_player_input() {

    //static float current_speed = constants::paddle_speed;

    // Speed adjust (Up/Down) - do NOT return
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        current_speed += constants::paddle_speed_step;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        current_speed -= constants::paddle_speed_step;

    current_speed = std::clamp(current_speed, constants::paddle_min_speed, constants::paddle_max_speed);
    //std::cout << current_speed << std::endl;
    
    // Keyboard input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = ((get_position().x - half_width) >= 0.f) ? -current_speed : 0.f;
        return;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = ((get_position().x + half_width) <= constants::window_width) ? current_speed : 0.f;
        return;
    }

    // Mouse input (ONLY if mouse moved)
    velocity.x = 0.f; // default

    if (!window_) return;

    // static here means: keep this value between frames(it doesn’t reset each function call).
    static int lastMouseX = 0;
    static bool first = true;

    const int mouseX_i = sf::Mouse::getPosition(*window_).x;

    if (first) {
        lastMouseX = mouseX_i;
        first = false;
        return;
    }

    const int dx = mouseX_i - lastMouseX;
    lastMouseX = mouseX_i;

    // If mouse didn't move, don't override keyboard / don't snap paddle
    if (dx == 0) return;

    // Mouse moved: directly position paddle
    float mouseX = static_cast<float>(mouseX_i);
    if (mouseX < half_width) mouseX = half_width;
    if (mouseX > constants::window_width - half_width)
        mouseX = constants::window_width - half_width;

    sprite->setPosition({ mouseX, get_position().y });
}
