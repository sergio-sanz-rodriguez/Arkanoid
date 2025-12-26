#ifndef BONUS_H
#define BONUS_H

#include "constants.h"
#include "entity.h"

// Define the two types of bonuses for the dame
enum class bonus_type {
    life,
    powerup
};

// Class to represent a bonus object
// Inherits from moving_entity
class bonus : public moving_entity {

private:

    // Private data members
    static sf::Texture life_texture;
    static sf::Texture powerup_texture;
    bonus_type type;
    float half_width;
    float half_height;

    void process_player_input() override;

public:

    // Constructor
    bonus(bonus_type type, sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col);

    // Required overrides
    void move_up() noexcept override;
    void move_down() noexcept override;
    void move_left() noexcept override;
    void move_right() noexcept override;

    // Implement the pure virtual functions
    void update() override;
    void draw(sf::RenderWindow& window) override;

    bonus_type get_type() const;

};

#endif // BONUS_H
#pragma once