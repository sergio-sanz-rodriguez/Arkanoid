#ifndef PADDLE_H
#define PADDLE_H

#include "constants.h"
#include "entity.h"
#include "paddle_colors.h"

// Class to represent the bouncing ball
// Inherits from moving_entity
class paddle : public moving_entity {

private:

    // Private data members
    static sf::Texture texture;
    static sf::RenderWindow* window_;
    bool scaleup{ false };
    float half_width;
    float height;

    // Respond to input from the player
    void process_player_input() override;

 public:

     // Interface of the class

     // Constructor
     paddle(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col, paddle_colors paddle_color = paddle_colors::light_gray);

     // Get the (half) width and height of the paddle
     float get_half_width() const noexcept;
     float get_height() const noexcept;

     // Get and set the the scale of the paddle
     bool get_scale() const noexcept;
     void set_scale(bool on, float factor) noexcept;

     // Required overrides
     void move_up() noexcept override;
     void move_down() noexcept override;
     void move_left() noexcept override;
     void move_right() noexcept override;

     // Implement the pure virtual functions
     void update() override;
     void draw(sf::RenderWindow& window) override;

     // Set window, needed for enabling mouse interaction
     static void set_window(sf::RenderWindow& w);

};

#endif // PADDLE_H
#pragma once