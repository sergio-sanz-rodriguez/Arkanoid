#ifndef BALL_H
#define BALL_H

#include "constants.h"
#include "entity.h"

// Class to represent the bouncing ball
// Inherits from moving_entity
class ball : public moving_entity {

    // Private data members
    static sf::Texture texture;
    float radius;
    bool fireball{ false };

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
     ball(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f sca, sf::Color col, bool fireball = false);

     // Get and set the state of the fireball feature
     bool get_fireball() const noexcept;
     void set_fireball(bool on, float factor) noexcept;

     // Required overrides
     void move_up() noexcept override;
     void move_down() noexcept override;
     void move_left() noexcept override;
     void move_right() noexcept override;

     // Implement the pure virtual functions
     void update() override;
     void draw(sf::RenderWindow& window) override;

};

#endif // BALL_H
