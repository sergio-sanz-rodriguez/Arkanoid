#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

// Abstract base class to represent any graphical entity on the screen
class entity {
protected:
    std::unique_ptr<sf::Sprite> sprite;
    // Boolean member tracks the entity's status
    bool destroyed{ false };
public:
    // Pure virtual functions
    // Concrete graphical entities must implement these functions
    // The update member function will compute the new position, appearance, etc of the object
    // The draw member function will cause the updated object to be displayed in the game window
    entity() {}
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    // These are PURE ACCESSORS! they read state, don't change state and should not throw excpetions
    // const noexcept tells the compilter that the operation is safe, simple and side-effect free
    
    // Helper function to get the bounding box of a sprite
    //noexcpet improves optimization opportunities, exceptio-safety guarantees, and makes the class behave better in generic code
    sf::FloatRect get_bounding_box() const noexcept; 

    // Helper function to get the centre of a sprite
    sf::Vector2f get_centre() const noexcept;

    // Helper functions to get the position of the sprite
    float x() const noexcept;
    float y() const noexcept;

    // Helper functions to get the edges of the sprite
    float left() const noexcept;
    float right() const noexcept;
    float top() const noexcept;
    float bottom() const noexcept;

    // Helper functions for the state of the entity
    void destroy() noexcept;
    bool is_destroyed() const noexcept;

    // Virtual destructor
    virtual ~entity() {}
};

// Abstract base class to represent graphical entities which can move around the screen
class moving_entity : public entity {
protected:
    // SFML vector to store the object's velocity
    // This is the distance the ball moves between screen updates
    sf::Vector2f velocity;

    virtual void process_player_input() = 0;

public:
    // Helper functions interited from parent class
    // float x();
    // float y();

    // Helper functions to change the sprite's direction
    virtual void move_up() noexcept = 0;
    virtual void move_down() noexcept = 0;
    virtual void move_left() noexcept = 0;
    virtual void move_right() noexcept = 0;

    // Pure virtual functions inherited from parent class
    // virtual void update() = 0;
    // virtual void draw(sf::RenderWindow& window) = 0;
    // etc
};

#endif // ENTITY_H
