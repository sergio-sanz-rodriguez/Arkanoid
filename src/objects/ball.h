#ifndef BALL_H
#define BALL_H

#include "constants.h"
#include "entity.h"

// Class to represent the bouncing ball
// Inherits from moving_entity
class bouncing_ball : public moving_entity {

private:

    // Private data members
    static sf::Texture texture;
    float radius;
    bool fireball{ false };
    bool hit_wall_this_frame = false;

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
     bouncing_ball(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col, bool fireball = false);

     // Get the radius of the ball
     float get_radius() const noexcept;

     // Get and set the state of the fireball feature
     bool get_fireball() const noexcept;
     void set_fireball(bool on, float factor) noexcept;

     // Detecting the hit with the wall
     bool consumed_wall_hit() noexcept;

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
class burst_ball : public moving_entity {

private:

    static sf::Texture texture;
    float radius = 0.f;

    void process_player_input() override {} // no input

public:
    burst_ball(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col);

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