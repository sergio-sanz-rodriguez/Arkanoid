#ifndef BALL_H
#define BALL_H

#include "constants.h"
#include "entity.h"
#include "ball_config.h"
#include "brick.h"

// Class to represent the bouncing ball
// Inherits from moving_entity
class bouncing_ball : public moving_entity {

private:

    // Private data members
    static sf::Texture texture;
    float radius;
    bool hit_wall_this_frame{ false };
    bool launched{ false };
    ball_type type{ ball_type::regular };
    const brick* ignore_brick_ = nullptr;
    void process_player_input() override;

 public:
     // Interface of the class

     // Constructor
     // Arguments are the initial coordinates of the centre of the ball
     // SFML uses the computer graphics convention
     // (0, 0) is the top left corner of the screen
     // pos.x: increases to the right
     // pos.y: increases downwards
     // vel: current speed of the ball
     // sca: current scale of the ball
     // col: current color of the ball
     bouncing_ball(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col, ball_type type = ball_type::regular);

     // Get the radius of the ball
     float get_radius() const noexcept;

     // Define base color
     sf::Color base_color{ ball_color_maps::bouncing_ball };

     // Get and set the state of the plasma_ball feature
     //bool get_plasma_ball() const noexcept;
     //void set_plasma_ball(bool on, float factor) noexcept;
     ball_type get_ball_type() const noexcept;
     void set_ball_type(ball_type new_type, float factor) noexcept;

     // Detecting the hit with the wall
     bool consumed_wall_hit() noexcept;

     // Launch the ball when the user presses the key "Space".
     void launch();
     void launch_keep_velocity();
     void stick_to_paddle(sf::Vector2f paddle_pos);
     bool is_launched() const noexcept;
     void reset_for_serve();

     //Logic to pass through the brick when plasma ball or antimatter ball is enabled
     void set_ignore_brick(const brick* b) noexcept { ignore_brick_ = b; }
     const brick* ignore_brick() const noexcept { return ignore_brick_; }
     void clear_ignore_brick() noexcept { ignore_brick_ = nullptr; }

     // Color mapping
     static sf::Color to_sf_color(ball_colors color);

     // Set the components of the velocity vector when the ball hits the paddle
     void bounce_from_paddle(float dist) noexcept;

     // Required overrides
     void move_up() noexcept override;
     void move_down() noexcept override;
     void move_left() noexcept override;
     void move_right() noexcept override;
     void move_up(float angle) noexcept;
     void move_down(float angle) noexcept;
     void move_left(float angle) noexcept;
     void move_right(float angle) noexcept;

     // Implement the pure virtual functions
     void update() override;
     void draw(sf::RenderWindow& window) override;

};

// Class to represent the burst_ball (a powerup)
class ballstorm : public moving_entity {

private:

    static sf::Texture texture;
    float radius = 0.f;

    void process_player_input() override {} // no input

public:
    ballstorm(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col);

    void update() override;
    void draw(sf::RenderWindow& w) override;

    void move_up() noexcept override {}
    void move_down() noexcept override {}
    void move_left() noexcept override {}
    void move_right() noexcept override {}

    float get_radius() const noexcept { return radius; }
};


#endif // BALL_H
#pragma once