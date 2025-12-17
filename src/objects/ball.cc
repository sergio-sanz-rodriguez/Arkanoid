#include "ball.h"
#include <iostream>

// Initialize static data
sf::Texture ball::texture;

ball::ball(float x, float y, float vx, float vy) {

    // Load the texture
    if (!texture.loadFromFile(constants::ball_path())) {
        throw std::runtime_error("Failed to load the ball texture.");
    }
    //sprite.setTexture(texture);
    sprite = std::make_unique<sf::Sprite>(texture);

    // Set the initial position and velocity of the ball
    // Use (x, y) for the initial position of the ball
    sprite->setOrigin(get_centre());
    sprite->scale({ 0.5f, 0.5f });
    sprite->setPosition({ x, y });

    // Set the radius of the ball
    radius = get_bounding_box().size.x / 2.0f;

    // Set the velocity of the ball
    velocity = { vx, -vy };
}

// Get ball speed
//float ball::get_speed() const noexcept {
//    return std::max(std::abs(velocity.x), std::abs(velocity.y));
//}

// Get ball radius
float ball::get_radius() const noexcept { return radius; }

// Update velocities
void ball::move_up() noexcept {
    velocity.y = -std::abs(velocity.y);
    rotate(constants::rotation_angle);
}

void ball::move_down() noexcept {
    velocity.y = std::abs(velocity.y);
    rotate(constants::rotation_angle);
}

void ball::move_left() noexcept {
    velocity.x = -std::abs(velocity.x);
    rotate(constants::rotation_angle);
}

void ball::move_right() noexcept {
    velocity.x = std::abs(velocity.x);
    rotate(constants::rotation_angle);
}

// Compute the ball's new position
void ball::update() {

    // Respond to user input as this will affect how the ball moves
    process_player_input();

    // Move the position of the ball
    sprite->move(velocity);

    //std::cout << velocity.x << " " << velocity.y << std::endl;
    // We check if the ball has moved off the left hand side of the window
    // If so, we change sign of the x-component of the velocity
    // This will make it move at the same speed, but to the right
    // The ball will appear to bounce back into the window
    if ((x() - radius) <= 0.0f) {
        //velocity.x = -velocity.x;
        sprite->setPosition({ radius, y() }); // push inside
        velocity.x = std::abs(velocity.x);    // ensure moving right
        //rotate(constants::rotation_angle);
    }
    // And similarly for the right hand side of the screen
    else if ((x() + radius) >= constants::window_width) {
        //velocity.x = -velocity.x;
        sprite->setPosition({ constants::window_width - radius, y() });
        velocity.x = -std::abs(velocity.x); // ensure moving left
        //rotate(constants::rotation_angle);
    }

    // We can also do this for the top and botoom of the screen
    if ((y() - radius) <= 0.0f) {
        //velocity.y = -velocity.y;
        sprite->setPosition({ x(), radius });
        velocity.y = std::abs(velocity.y); // ensure moving down
        //rotate(constants::rotation_angle);
    }
    else if ((y() + radius) >= constants::window_height) {
        //velocity.y = -velocity.y;
        //sprite->setPosition({ x(), constants::window_height - radius });
        //velocity.y = -std::abs(velocity.y); // ensure moving up
        destroy();
    }
}

void ball::process_player_input() {

    // Up key increases the velocity of the ball, and down key decreases it
    const bool upKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    const bool downKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

    if (!upKey && !downKey) return;

    // current speed (magnitude)
    //float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    float vx = std::abs(velocity.x);
    float vy = std::abs(velocity.y);
    float speed =std::max(vx, vy);

    if (speed <= 0.f) return; // safety

    // new desired speed
    if (upKey)   speed += constants::ball_speed_step;
    if (downKey) speed -= constants::ball_speed_step;

    speed = std::clamp(speed, constants::ball_min_speed, constants::ball_max_speed);

    velocity.x *= speed / vx;
    velocity.y *= speed / vy;

}

// Drawing function
void ball::draw(sf::RenderWindow& window) {
    // Ask the window to draw the sprite for us
    window.draw(*sprite);
}
