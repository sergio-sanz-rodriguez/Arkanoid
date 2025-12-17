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

    void process_player_input() override;

 public:
     // Interface of the class

      // Constructor
      // Arguments are the initial coordinates of the centre of the ball
      // SFML uses the computer graphics convention
      // (0, 0) is the top left corner of the screen
      // x: increases to the right
      // y: increases downwards
      // speed: the current ball speed
      ball(float x, float y, float vx = constants::ball_speed, float vy = constants::ball_speed);

      // Get ball speed
      //float get_speed() const noexcept;

      // Get ball radius
      float get_radius() const noexcept;

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
