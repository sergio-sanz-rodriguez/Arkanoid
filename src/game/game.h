#ifndef GAME_H
#define GAME_H

#include <memory>
#include <typeinfo>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <iostream>
#include <chrono>

#include "constants.h"
#include "background.h"
#include "ball.h"
#include "brick.h"
#include "entity.h"
#include "paddle.h"
#include "bonus.h"

// A class to manage the entities in the game
// It stores the entities in a vector of std::unique_ptr
// The manager can create an entity object and add it to the vector

// Example: create a ball object at position (x, y) on the screen
// manager.create<ball>(x, y);

// When an entity is destroyed, the game will mark it as destroyed, but not delete it.
// Instead, the manager will scan all entities and clean up the destroyed ones

// The manager can also locate all the objects with a given entity type
// These are stored as alias pointers (non-owning) to the allocated objects
// This is useful for performing an operation on e.g. all bricks

// Use aliases to simplify the code (C++11)
using entity_vector = std::vector<std::unique_ptr<entity>>;
using entity_alias_vector = std::vector<entity *>;

class entity_manager {

    // A vector to store all the entities in the game (all brick objects, background, ball, paddle)
    entity_vector all_entities;

    // A map to store the entities grouped by type
    // We can iterate over this to get a vector of all brick objects, then a vector of all ball objects, etc
    // The vector will contain non-owning pointers to the objects
    // Do not delete these pointers!
    // Do not use them after the objects they point to have been destroyed!
    std::map<size_t, entity_alias_vector> grouped_entities; // The hash code retuns a size_t type

public:

    // Function to create an entity object of type T using args as the constructor arguments
    // We use a variadic template to pass any number of arguments
    // We use perfect forwarding to avoid any unnecessary copying of the arguments
    template <typename T, typename... Args>
    T& create(Args&&... args) {

        // Check that the type parameter is derived from the entity base class
        static_assert(std::is_base_of<entity, T>::value,
            R"("T" type parameter in create() must be derived from "entity")");

        // Create a unique_ptr to the entity
        // Forward the arguments to the entity's constructor
        auto ptr{ std::make_unique<T>(std::forward<Args>(args)...) }; //std::forward <float&>(arg1), <float&>(arg2));

        // Make an alias pointer to the allocated memory
        // This will be stored in the entity_type_vector
        auto ptr_alias = ptr.get(); // ptr.get() returns a classical C++ pointer to the object

        // Get the hash code for the entity object's type
        auto hash = typeid(T).hash_code();

        // Insert the alias pointer into the map
        grouped_entities[hash].emplace_back(ptr_alias);

        // Insert the object's pointer into the entities vector
        all_entities.emplace_back(std::move(ptr)); //unique pointers cannot be copied, only moved.

        // Return the new object
        return *ptr_alias;
    }

    // Function to scan all entities and clean up the destroyed ones
    void refresh();

    // Function to destroy all entities
    void clear();

    // Function to retrieve all the objects of a given type
    template <typename T>
    auto& get_all() {
        return grouped_entities[typeid(T).hash_code()];
    }

    // Functions to retrieve the first object
    // This version is used when you have a normal manager, like:
    // entity_manager manager;
    // auto* p = manager.get_first<paddle>();
    template <typename T>
    T* get_first() {
        auto& group = get_all<T>();
        if (group.empty()) return nullptr;
        return dynamic_cast<T*>(group.front());
    }
    // This version is used when the manager is const, like:
    // const entity_manager& manager_ref = manager;
    // auto* p = manager_ref.get_first<paddle>();
    template <typename T>
    const T* get_first() const {
        auto it = grouped_entities.find(typeid(T).hash_code());
        if (it == grouped_entities.end() || it->second.empty()) return nullptr;
        return dynamic_cast<const T*>(it->second.front());
    }


    // Apply a function to all entities of a given type
    template <typename T, typename Func>
    void apply_all(const Func& func) {
        auto& entity_group{ get_all<T>() };

        for (auto ptr : entity_group)
            func(*dynamic_cast<T*>(ptr));
    }

    // Function to update all the entities
    void update();

    // Function to make all the entities draw themselves
    void draw(sf::RenderWindow& window);
};

class game {

    // Enum with allowed values for the game's state
    enum class game_state { game_over, paused, player_wins, start_screen, running };

    // Create the game's window using an object of class RenderWindow
    // The constructor takes an SFML 2D vector with the window dimensions
    // and an std::string with the window title
    // The SFML code is in the sf namespace
    sf::RenderWindow game_window{ sf::VideoMode({constants::window_width, constants::window_height}),
        "Arkanoid by Sergio Sanz" };

    // Instead of embedding every entity in the game class, use an entity_manager
    entity_manager manager;

    // Use SFML text and font classes to communicate with text with the player
    sf::Font verdana;
    sf::Text text_state;
    sf::Text text_fireball;
    sf::Text text_lives;
    sf::Text text_powerup;
    sf::Text text_instructions;

    // Member to store the current state of the game
    game_state state{ game_state::start_screen };

    // Define a vector with all the possible colors for the bricks
    std::vector<sf::Color> vcolor{
        constants::red,
        constants::orange,
        constants::green,
        constants::cyan,
        constants::blue,
        constants::white};

    // How many lives does the player have left?
    int lives{ constants::player_lives };

    // Current position and speed of the ball
    sf::Vector2f current_ball_position{ 
        constants::window_width / 2.0f,
        constants::window_height - constants::paddle_height
    };
    sf::Vector2f current_ball_velocity{
        constants::ball_speed,
        constants::ball_speed
    };

    // Declare some control flags
    bool pause_key_active{ false };

    // Bonus spawn control
    sf::Clock bonus_clock;
    float next_bonus_time = 0.0f;
    std::mt19937 rng{ std::random_device{}() };

    // Timing
    std::uniform_real_distribution<float> bonus_delay_dist{ 5.0f, 15.0f };

    // X position
    std::uniform_real_distribution<float> bonus_x_dist{
        constants::window_width / 16.0f,
        constants::window_width - (constants::window_width / 16.0f)
    };

    // Speed jitter for each bonus type
    std::uniform_real_distribution<float> fireball_jitter{
       constants::bonus_speed_jitter,
       1.0f / constants::bonus_speed_jitter
    };
    std::uniform_real_distribution<float> life_jitter{ 
        constants::bonus_speed_jitter,
        1.0f / constants::bonus_speed_jitter
    };
    std::uniform_real_distribution<float> powerup_jitter{
        constants::bonus_speed_jitter,
        1.0f / constants::bonus_speed_jitter
    };

    // --- Helper functions --- //

    // Handle powerups
    powerups active_powerups;
    void apply_powerups_to_entities();
    void spawn_extra_balls_up_to();
    powerup_type random_powerup();
    std::optional<powerup_type> last_powerup;

    // Globals
    void handle_window_events();
    void handle_global_inputs();
    void update_state_text();

    // Update everything when running
    void update_running_frame();

    // Respawn ball if none
    void ensure_ball_exists();

    // Spawn bonus entities
    void spawn_bonuses();

    // Ball-brick, ball-paddle, bonus-paddle
    void resolve_collisions();

    // Powerup logic + message
    void handle_bonus_pickups();

    // Lives, fireball, powerup msg
    void update_ui_texts(const std::string& powerup_msg);

    // Draw entities + UI
    void draw_frame();

public:

    // Game constructor, initializer
    game();

    // Reinitialize the game
    void reset();

    // Game loop
    void run();
};

#endif // GAME_H
