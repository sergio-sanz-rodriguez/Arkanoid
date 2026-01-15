#ifndef BONUS_H
#define BONUS_H

#include "constants.h"
#include "entity.h"

// Define the two types of bonuses for the dame
enum class bonus_type {
    plasma_ball,
    life,
    powerup
};

enum class powerup_type {
    ball_faster,
    ball_slower,
    ballstorm,
    plasma_ball,
    antimatter_ball,
    multiball,
    paddle_wider,
    paddle_narrower,
    reset_powerups
};

struct powerups {

    bool ball_faster = false;
    bool ball_slower = false;
    bool ballstorm = false;
    bool plasma_ball = false;
    bool antimatter_ball = false;
    bool multiball = false;
    bool paddle_wider = false;
    bool paddle_narrower = false;
    bool reset_powerups = true;

    void reset() { *this = {}; }

    void apply(powerup_type bonus) {
        switch (bonus) {
            case powerup_type::plasma_ball:     plasma_ball = true; break;
            case powerup_type::antimatter_ball: antimatter_ball = true; break;
            case powerup_type::multiball:       multiball = true; break;
            case powerup_type::ballstorm:       ballstorm = true; break;
            case powerup_type::ball_faster:     ball_faster = true; ball_slower = false; break;
            case powerup_type::ball_slower:     ball_slower = true; ball_faster = false; break;
            case powerup_type::paddle_wider:    paddle_wider = true; paddle_narrower = false; break;
            case powerup_type::paddle_narrower: paddle_narrower = true; paddle_wider = false; break;
            case powerup_type::reset_powerups:  reset(); break;
            default: break;
        }
    }
};

inline constexpr std::array<powerup_type, 1> powerup_candidates = {
    //powerup_type::multiball,
    powerup_type::ballstorm,
    //powerup_type::ball_faster,
    //powerup_type::ball_slower,
    //powerup_type::paddle_wider,
    //powerup_type::paddle_narrower,
    //powerup_type::reset_powerups
};

// Class to represent a bonus object
// Inherits from moving_entity
class bonus : public moving_entity {

private:

    // Private data members
    static sf::Texture plasma_ball_texture;
    static sf::Texture life_texture;
    static sf::Texture powerup_texture;
    float half_width;
    float half_height;
    bonus_type type;

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

    // Helper function to get the type of texture
    bonus_type get_type() const;

    // Helper function to get half width of a bonus type
    static float half_width_for(bonus_type type);

};

#endif // BONUS_H
#pragma once