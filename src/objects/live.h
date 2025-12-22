#ifndef LIVE_H
#define LIVE_H

#include "constants.h"
#include "entity.h"

// Class to represent the bouncing ball
// Inherits from moving_entity
class live : public moving_entity {

    // Private data members
    static sf::Texture texture;
    float half_width;
    float half_height;

    void process_player_input() override;

public:
    // Interface of the class

    // Constructor
    live(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col);

    // Required overrides
    void move_up() noexcept override;
    void move_down() noexcept override;
    void move_left() noexcept override;
    void move_right() noexcept override;

    // Implement the pure virtual functions
    void update() override;
    void draw(sf::RenderWindow& window) override;

};

#endif // LIVE_H
#pragma once
