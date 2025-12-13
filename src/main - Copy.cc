#include <SFML/Graphics.hpp>
#include <string>
#include <random>
#include "constants.h"
#include "background.h"
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "interactions.h"
#include <iostream>

using namespace std;
using namespace std::literals;

// Class to represent the creature moving around the screen
class creature {
    // Static random number engine and Bernoulli distribution objects
    static mt19937 mt;
    static bernoulli_distribution bd; //50%-50% chance of true or false

    // Define some properties of the creature
    // SFML uses float internally
    float vx{4.0f}; // if positive, it moves to the right, if negative, to the left
    float vy{4.0f}; // if positive, it moves down the screen, if negative, up
    sf::Vector2f velocity;

    // We use the SFML CircleShape
    sf::CircleShape circle;

public:
    // Interface of the class

    // Constructor
    // Arguments are the initial coordinates of the centre of the ball
    // SFML uses the computer graphics convention
    // (0, 0) is the top left corner of the screen
    // x increases to the right
    // y increases downwards
    creature(float x, float y) {
        // Set the initial position and velocity
        // Use (x, y) for the initial position
        velocity = {vx, vy};
        circle.setPosition({ x, y });

        // Set the graphical properties
        circle.setRadius(5.0f);
        circle.setFillColor(sf::Color::Red);
    }

    void draw(sf::RenderWindow& window) {
        // Ask the window to draw the shape for us
        window.draw(circle);
    }

    // Compute the creature's new position
    void update() {
        // Change the direction of movement at random
        vx = bd(mt) ? vx : -vx;
        vy = bd(mt) ? vy : -vy;

        // Move the creature to its new position
        circle.move({vx, vy});
    }
};

// Initialize the static objects, outside any class or function
mt19937 creature::mt;
bernoulli_distribution creature::bd;


// The main function for the program
int main() {

    // Create the background object
    background the_background(0.0f, 0.0f);

    // Create a ball object in the middle of the screen
    ball the_ball(constants::window_width / 2.0, constants::window_height / 2.0);

    // Create a paddle object on the bottom of the screen
    paddle the_paddle(constants::window_width / 2.0, constants::window_height - constants::paddle_height);

    // Create a vector of pointers to bricks on the top of the screen
    std::vector<std::unique_ptr<brick>> bricks;
    for (int i = 0; i < constants::brick_columns; ++i) {
        for (int j = 0; j < constants::brick_rows; ++j) {
            // Calculate the brick's position
            float x = constants::brick_offset + (i + 1) * constants::brick_width;
            float y = (j + 2) * constants::brick_height;

            //Create the brick object directly in the vector, without any copying
            bricks.push_back(std::make_unique<brick>(x, y));
        }
    }

    // Create a creature object in the middle of the screen
    //creature the_creature(constants::window_width/2.0, constants::window_height/2.0);

    // Create the game's window using an object of class RenderWindow
    // The constructor takes an SFML 2D vector with the window dimensions
    // and an std::string with the window title
    // The SFML code is in the sf namespace
    sf::RenderWindow window{sf::VideoMode({constants::window_width, constants::window_height}), "Simple Breakout Game Version 4"s};

    // Set window in paddle to allow mouse interaction
    paddle::set_window(window);

    // Limit the framerate
    // This allows other processes to run and reduces power consumption
    window.setFramerateLimit(60);      // Max rate is 60 frames per second

    // Loop to update the window
    while (window.isOpen()) {

        // Clear the screen
        window.clear(sf::Color::Black);

        // Check for any events since the last loop iteration
        // If the user clicks on "close" or presses "Escape", we close the window program
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            window.close();
        }

        // Calculate the updated graphics and collision handling
        the_background.update();
        the_ball.update();
        the_paddle.update();
        for (auto& b : bricks) {
            b->update(); //Does nothing, but added for consistency with the background
        }
        handle_collision(the_ball, the_paddle);
        for (auto& b : bricks) {
            handle_collision(the_ball, *b);
        }

        // Erase any destroyed bricks from the grid

        // remove_if moves all elements to the back for which the conditional is true
        // It returns an iterator the the first moved element, in other words,
        // it returns an iterator pointing to the first element in the garbage tail
        // We then call erase with this iterator as argument
        // This will erase every element following this iterator
        //bricks.erase(new_end, std::end(bricks));
        bricks.erase(
            std::remove_if(std::begin(bricks), std::end(bricks),
                [](auto const& bptr) {return bptr->is_destroyed(); }),
            std::end(bricks));

        //the_creature.update();

        // Display the updated graphics
        the_background.draw(window);
        the_ball.draw(window);
        the_paddle.draw(window);
        for (auto& b : bricks) {
            b->draw(window);
        }
        //the_creature.draw(window);
        
        window.display();
    }

    return 0;

}