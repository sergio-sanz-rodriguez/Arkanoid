#ifndef PADDLE_H
#define PADDLE_H

#include "constants.h"
#include "entity.h"

// Class to represent the bouncing ball
// Inherits from moving_entity
class paddle : public moving_entity {
    
    // Private data members
    static sf::Texture texture;
    float half_width;
    static sf::RenderWindow* window_;
    float current_speed = constants::paddle_speed;

    // Respond to input from the player
    void process_player_input() override;

 public:
     // Interface of the class

      // Constructor      
      paddle(float x, float y, float vx, float vy);

      // Get paddle speed
      //float get_speed() const noexcept;

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
