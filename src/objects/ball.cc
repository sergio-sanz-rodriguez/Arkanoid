#include "ball.h"

// Initialize static data
sf::Texture ball::texture;

ball::ball(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col, bool fireball) : fireball(fireball) {

    // Load the texture
    if (!texture.loadFromFile(constants::ball_path())) {
        throw std::runtime_error("Failed to load the ball texture.");
    }
    //sprite.setTexture(texture);
    sprite = std::make_unique<sf::Sprite>(texture);

    // Set the initial position, velocity, and color of the ball
    // Use (x, y) for the initial position of the ball
    sprite->setOrigin(get_centre());
    sprite->setPosition(pos);
    sprite->scale(sca);
    sprite->setColor(col);
    velocity = vel; //velocity = { vx, vy };

    // Set the radius of the ball
    radius = get_bounding_box().size.x / 2.0f;
}

// Get and set ball radius
float ball::get_radius() const noexcept { return radius; }
void ball::set_radius(float r) noexcept { radius = r; }

// Get and set the state of the fireball feature
bool ball::get_fireball() const noexcept { return fireball; }
void ball::set_fireball(bool on, float factor) noexcept {
    fireball = on;
    sprite->setColor(on ? constants::orange : constants::steel);
    sprite->setScale(on ? factor * sf::Vector2f{ 0.5f, 0.5f } : sf::Vector2f{ 0.5f, 0.5f });
    radius = get_bounding_box().size.x / 2.0f;
}

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

    // We check if the ball has moved off the left hand side of the window
    // If so, we change sign of the x-component of the velocity
    // This will make it move at the same speed, but to the right
    // The ball will appear to bounce back into the window
    if ((get_position().x - radius) <= 0.0f) {
        sprite->setPosition({ radius, get_position().y }); // push inside
        velocity.x = std::abs(velocity.x);    // ensure moving right
    }
    // And similarly for the right hand side of the screen
    else if ((get_position().x + radius) >= constants::window_width) {
        sprite->setPosition({ constants::window_width - radius, get_position().y });
        velocity.x = -std::abs(velocity.x); // ensure moving left
    }

    // We can also do this for the top and botoom of the screen
    if ((get_position().y - radius) <= 0.0f) {
        sprite->setPosition({ get_position().x, radius });
        velocity.y = std::abs(velocity.y); // ensure moving down
    }
    else if ((get_position().y + radius) >= constants::window_height) {
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
