#ifndef BONUS_H
#define BONUS_H

#include "constants.h"
#include "entity.h"

// Class to represent a bonus object
// Inherits from moving_entity
class bonus : public moving_entity {

    // Private data members
    static sf::Texture texture;
    float half_width;
    float half_height;

    void process_player_input() override;

public:
    // Interface of the class

    // Constructor
    bonus(const std::string& path, sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col);

    // Required overrides
    void move_up() noexcept override;
    void move_down() noexcept override;
    void move_left() noexcept override;
    void move_right() noexcept override;

    // Implement the pure virtual functions
    void update() override;
    void draw(sf::RenderWindow& window) override;

    enum class type { life, powerup };
    virtual type get_type() const = 0;

};

// Define bonus life subclass
class bonus_life : public bonus {
public:

    bonus_life(const std::string& path, sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col)
        : bonus(path, pos, vel, sca, col) {}

    type get_type() const override { return type::life; }
};

// -Define bonus powerup subclass
class bonus_powerup : public bonus {
public:

    bonus_powerup(const std::string& path, sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col)
        : bonus(path, pos, vel, sca, col) {}

    type get_type() const override { return type::powerup; }
};


#endif // BONUS_H
#pragma once