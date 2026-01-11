#include "assets.h"
#include "ball.h"
#include "ball_colors.h"

// Initialize static data
sf::Texture bouncing_ball::texture;
sf::Texture ballstorm::texture;

bouncing_ball::bouncing_ball(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col, bool plasma_ball) : plasma_ball(plasma_ball) {

    // Load the texture
    if (!texture.loadFromFile(assets::img_ball_path())) {
        throw std::runtime_error("Failed to load the bouncing ball texture.");
    }

    // Set the initial position, velocity, and color of the ball
    // Use (x, y) for the initial position of the ball
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setOrigin(get_centre());
    sprite->setPosition(pos);
    sprite->scale(sca);
    sprite->setColor(col);
    velocity = vel;

    // Set the radius of the ball
    radius = get_bounding_box().size.x / 2.0f;
}

// Get the radius of the ball
float bouncing_ball::get_radius() const noexcept { return radius; }

// Get and set the state of the plasma_ball feature
bool bouncing_ball::get_plasma_ball() const noexcept { return plasma_ball; }
void bouncing_ball::set_plasma_ball(bool on, float factor) noexcept {
    plasma_ball = on;
    sprite->setColor(on ? ball_colors::plasma_ball : ball_colors::bouncing_ball);
    sprite->setScale(on ? factor * sf::Vector2f{ 0.5f, 0.5f } : sf::Vector2f{ 0.5f, 0.5f });
    radius = get_bounding_box().size.x / 2.0f;
}

// Function to detect if the ball hits the any wall
bool bouncing_ball::consumed_wall_hit() noexcept {
    bool r = hit_wall_this_frame;
    hit_wall_this_frame = false;   // consume it
    return r;
}

// Set the components of the velocity vector when the ball hits the paddle
void bouncing_ball::bounce_from_paddle(float dist) noexcept {

    dist = std::clamp(dist, -1.f, 1.f);

    // Current speed (magnitude)
    const float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    // Convert dist to angle
    const float angle_deg = dist * constants::max_angle; // e.g. 45 degrees
    const float angle_rad = angle_deg * 3.14159265f / 180.f;

    // Build velocity from scratch.
    // x: sin(angle), y: cos(angle) but negative because going up
    velocity.x = speed * std::sin(angle_rad);
    velocity.y = -speed * std::cos(angle_rad);
}


// Drawing function
void bouncing_ball::draw(sf::RenderWindow& window) {
    // Ask the window to draw the sprite for us
    window.draw(*sprite);
}

// Update velocities
void bouncing_ball::move_up() noexcept {
    velocity.y = -std::abs(velocity.y);
}

void bouncing_ball::move_down() noexcept {
    velocity.y = std::abs(velocity.y);
}

void bouncing_ball::move_left() noexcept {
    velocity.x = -std::abs(velocity.x);
}

void bouncing_ball::move_right() noexcept {
    velocity.x = std::abs(velocity.x);
}

void bouncing_ball::move_up(float angle) noexcept {
    velocity.y = -std::abs(velocity.y);
    rotate(angle);
}

void bouncing_ball::move_down(float angle) noexcept {
    velocity.y = std::abs(velocity.y);
    rotate(angle);
}

void bouncing_ball::move_left(float angle) noexcept {
    velocity.x = -std::abs(velocity.x);
    rotate(angle);
}

void bouncing_ball::move_right(float angle) noexcept {
    velocity.x = std::abs(velocity.x);
    rotate(angle);
}

// Compute the ball's new position
void bouncing_ball::update() {

    // Respond to user input as this will affect how the ball moves
    //process_player_input();

    // Reset at start of frame
    hit_wall_this_frame = false;

    // Move the position of the ball
    sprite->move(velocity);

    // We check if the ball has moved off the left hand side of the window
    // If so, we change sign of the x-component of the velocity
    // This will make it move at the same speed, but to the right
    // The ball will appear to bounce back into the window
    if ((get_position().x - radius) <= 0.0f) {
        sprite->setPosition({ radius, get_position().y }); // push inside
        velocity.x = std::abs(velocity.x);    // ensure moving right
        hit_wall_this_frame = true;
    }
    // And similarly for the right hand side of the screen
    else if ((get_position().x + radius) >= constants::window_width) {
        sprite->setPosition({ constants::window_width - radius, get_position().y });
        velocity.x = -std::abs(velocity.x); // ensure moving left
        hit_wall_this_frame = true;
    }

    // We can also do this for the top and botoom of the screen
    if ((get_position().y - radius) <= 0.0f) {
        sprite->setPosition({ get_position().x, radius });
        velocity.y = std::abs(velocity.y); // ensure moving down
        hit_wall_this_frame = true;
    }
    else if ((get_position().y + radius) >= constants::window_height) {
        destroy();
    }
}

// Logic when the user speed ups or down the speed
void bouncing_ball::process_player_input() {

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

ballstorm::ballstorm(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col) {

    // Load the texture
    if (!texture.loadFromFile(assets::img_ballstorm_path())) {
        throw std::runtime_error("Failed to load ballstorm texture");
    }

    // Set the initial position, velocity, and color of the ball
    // Use (x, y) for the initial position of the ball
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setOrigin(get_centre());
    sprite->setPosition(pos);
    sprite->setScale(sca);
    sprite->setColor(col);
    velocity = vel;

    // Set the radius of the ball
    radius = get_bounding_box().size.x / 2.f;
}

// Compute the ball's new position
void ballstorm::update() {

    sprite->move(velocity);

    // If it leaves the screen (top), destroy it
    if (get_position().y + radius < 0.f) {
        destroy();
    }
}

// Drawing function
void ballstorm::draw(sf::RenderWindow& w) {
    w.draw(*sprite);
}