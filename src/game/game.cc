#include "game.h"
#include "interactions.h"

// Function to scan all entities and clean up the destroyed ones
void entity_manager::refresh() {
    // We must clean up the alias pointers first, to avoid dangling pointers
    // We simply remove them from their vector
    for (auto& [type, alias_vector] : grouped_entities) {
        // remove_if takes an iterator range and a predicate
        // All the elements for which the predicate is true are moved to the back
        // It returns an iterator to the first moved element
        // erase takes an iterator range and deletes all the elements in the range
        alias_vector.erase(remove_if(begin(alias_vector), end(alias_vector),
            [](const auto& p) { return p->is_destroyed(); }
        ),
            end(alias_vector));
    }

    // Now we can safely destroy the objects, now that there are no aliases to them
    all_entities.erase(remove_if(begin(all_entities), end(all_entities),
        [](const auto& p) { return p->is_destroyed(); }
    ),
        end(all_entities));
}

// Function to destroy all entities
void entity_manager::clear() {
    // Again, we must clean up the alias pointers first
    grouped_entities.clear();
    all_entities.clear();
}

// Function to update all the entities
void entity_manager::update() {
    for (auto& e : all_entities)
        e->update();
}

// Function to update make all the entities draw themselves
void entity_manager::draw(sf::RenderWindow& window) {
    for (auto& e : all_entities)
        e->draw(window);
}

game::game() : 
    text_state(verdana), 
    text_lives(verdana), 
    text_instructions(verdana) {

    // Limit the framerate
    game_window.setFramerateLimit(60);      // Max rate is 60 frames per second

    // Set window in paddle to allow mouse interaction
    paddle::set_window(game_window);

    // Load a font from file
    if (!verdana.openFromFile(constants::font_verdana)) {
        std::cerr << "Failed to load font!" << std::endl;
        // Handle font loading failure (could exit, use default font, etc.)
    }

    // Configure our text objects
    text_state.setFont(verdana);
    text_state.setPosition({ constants::window_width / 2.0f - 125.0f, constants::window_height / 2.0f - 100.0f });
    text_state.setCharacterSize(35);
    text_state.setFillColor(sf::Color::White);
    text_state.setString("Paused");

    text_lives.setFont(verdana);
    //text_lives.setPosition({ constants::window_width / 2.0f - 90.0f, constants::window_height / 2.0f - 50.0f });
    text_lives.setPosition({ constants::window_width - 75.0f, constants::window_height - 27.0f });
    text_lives.setCharacterSize(12);
    text_lives.setFillColor(sf::Color::White);
    text_lives.setString("Lives: " + std::to_string(lives));

    text_instructions.setFont(verdana);
    text_instructions.setPosition({ constants::window_width / 16.0f, constants::window_height / 4 });
    text_instructions.setCharacterSize(20);
    text_instructions.setFillColor(sf::Color::White);
    text_instructions.setString(
        "Welcome to Arkanoid!\n\n"
        "Instructions:\n\n"
        "- Left arrow / Move mouse left, to move paddle left\n"
        "- Right Arrow / Move mouse right to move paddle right\n"
        "- Up arrow to increase ball speed\n"
        "- Down arrow to decrease ball speed\n"
        "- P button to pause and resume the game\n"
        "- R button to reset the game\n\n"
        "Press any key to start."
    );

}

// (Re)initialize the game
void game::reset() {

    // Reset the number of lives
    lives = constants::player_lives;

    // Remove power-ups
    fireball_enabled = false;

    // Destroy all the entities and re-create them
    manager.clear();

    // Reset the entities and their positions
    // Background picture
    manager.create<background>(0.0f, 0.0f);

    // Ball object
    manager.create<ball>(
        sf::Vector2f{ constants::window_width / 2.0f, constants::window_height - constants::paddle_height },
        sf::Vector2f{ constants::ball_speed, -constants::ball_speed },
        sf::Vector2f{ 0.5f, 0.5f},
        constants::steel
    );

    // Paddle object
    manager.create<paddle>(
        sf::Vector2f{ constants::window_width / 2.0f, constants::window_height - constants::paddle_height },
        sf::Vector2f{ constants::paddle_speed, 0.0f },
        sf::Vector2f{ constants::paddle_scale_width, constants::paddle_scale_height },
        constants::white
    );

    // Create random number generator and uniform distribution
    thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> color_dist(0, static_cast<int>(vcolor.size()) - 1);

    for (int i = 0; i < constants::brick_columns; ++i) {
        for (int j = 0; j < constants::brick_rows; ++j) {
            // Calculate the brick's position
            float x = constants::brick_offset + (i + 1) * constants::brick_width;
            float y = (j + 2) * constants::brick_height;

            // Create the brick object
            sf::Color c = vcolor[j % vcolor.size()]; // Access the color at the correct index
            //sf::Color c = vcolor[color_dist(rng)]; // Pick a random color
            manager.create<brick>(
                sf::Vector2f{ x, y },
                sf::Vector2f{ 1.0f, 1.0f },
                c); // Create the brick with the color
        }
    }

    // Initialize bonus spawn
    bonus_clock.restart();
    next_bonus_time = bonus_delay_dist(rng);


    // Limit the framerate
    game_window.setFramerateLimit(60); // Max rate is 60 frames per second
}

// (Re)start the game
void game::run() {

    while (game_window.isOpen()) {

        // Clear the screen
        game_window.clear(sf::Color::Black);

        // Check for any events since the last loop iteration
        // If the user clicks on "close" or presses "Escape", we close the window program
        while (auto event = game_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                game_window.close();
            }

            if (event->is<sf::Event::KeyPressed>()) {

                // Start screen: any key starts
                if (state == game_state::start_screen) {
                    state = game_state::running;
                }

                // End screens: any key restarts
                if (state == game_state::game_over || state == game_state::player_wins) {
                    reset();
                    state = game_state::running;
                }
            }
        }

        // Start screen draw
        if (state == game_state::start_screen) {
            game_window.draw(text_instructions);
            game_window.display();
            continue;
        }

        // If the user presses "Escape", we jump out of the loop and terminate the program
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            break;

        bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
        if (pressed && !pause_key_active) {
            state = (state == game_state::paused) ? game_state::running : game_state::paused;
        }
        pause_key_active = pressed;

        // If the user presses "R", we reset the game
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            reset();
        }

        // If the user presses "F", the ball is Fireball
        pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);
        if (pressed && !fireball_key_active) {
            fireball_enabled = !fireball_enabled;
            manager.apply_all<ball>([&](ball& b) {
                b.set_fireball(fireball_enabled, 1.0f);
            });
        }
        fireball_key_active = pressed;

        // If the user presses "S", the ball is Fireball
        pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
        if (pressed && !paddle_scaleup_key_active) {
            paddle_scaleup_enabled = !paddle_scaleup_enabled;
            manager.apply_all<paddle>([&](paddle& b) {
                b.set_scale(paddle_scaleup_enabled, 2.0f);
            });
        }
        paddle_scaleup_key_active = pressed;

        // If the game is not running, the entities are not updated
        // They are redrawn only if the game is paused
        if (state == game_state::paused) {
            // Display the graphics
            manager.draw(game_window);
        }

        // Choose the correct text for the state of the game
        if (state != game_state::running) {
            switch (state) {
            case game_state::paused:
                text_state.setPosition({ constants::window_width / 2.0f - 65.0f, constants::window_height / 2.0f - 100.0f });
                text_state.setString("Paused");
                break;
            case game_state::game_over:
                text_state.setPosition({ constants::window_width / 16.0f, constants::window_height / 2.0f - 150.0f });
                text_state.setString(
                    "  Game Over!\n\n"
                    "  - Press any key to play again\n"
                    "  - Press escape to quit\n")
                    ;
                break;
            case game_state::player_wins:
                text_state.setPosition({ constants::window_width / 2.0f - 100.0f, constants::window_height / 2.0f - 100.0f });
                text_state.setString("Player Wins!");
                break;
            default:
                break;
            }

            game_window.draw(text_state);
        }

        // If the game is running
        else {

            // Count active bonuses by type 
            size_t life_count = 0;
            size_t powerup_count = 0;
            manager.apply_all<bonus>([&](bonus& b) {
                if (b.get_type() == bonus_type::life)
                    ++life_count;
                else if (b.get_type() == bonus_type::powerup)
                    ++powerup_count;
            });

            // If there are no remaining balls on the screen
            if (manager.get_all<ball>().empty()) {
                // Spawn a new one and reduce the player's remaining lives
                auto pos = sf::Vector2f{ constants::window_width / 2.f, constants::window_height / 2.f };
                auto vel = sf::Vector2f{ std::abs(current_ball_velocity.x), -std::abs(current_ball_velocity.y) };
                manager.create<ball>(
                    pos,
                    vel,
                    fireball_enabled ? sf::Vector2f{ 0.5f, 0.5f } : sf::Vector2f{ 0.5f, 0.5f },
                    fireball_enabled ? constants::orange : constants::steel,
                    fireball_enabled
                );
                --lives;
            }

            // Remember the speed and position of the ball while ball still exists
            else {
                manager.apply_all<ball>([this](ball& b) {
                    current_ball_position = b.get_position();
                    current_ball_velocity = b.get_velocity();
                });
            }

            // Remember speed and position of the paddle
            manager.apply_all<paddle>([this](paddle& p) {
                current_paddle_position = p.get_position();
                current_paddle_velocity = p.get_velocity();
                });

            // Bonus spawning logic
            if (bonus_clock.getElapsedTime().asSeconds() >= next_bonus_time) {

                // Spawn LIFE bonus if none exists
                if (life_count == 0 && std::bernoulli_distribution(0.5)(rng)) {
                    manager.create<bonus>(
                        bonus_type::life,
                        sf::Vector2f{ bonus_x_dist(rng), 0.f },
                        sf::Vector2f{ 0.f, constants::bonus_speed * life_jitter(rng) },
                        sf::Vector2f{ constants::bonus_scale, constants::bonus_scale },
                        constants::white
                    );
                }

                // Spawn POWERUP bonus if none exists
                if (powerup_count == 0 && std::bernoulli_distribution(0.5)(rng)) {
                    manager.create<bonus>(
                        bonus_type::powerup,
                        sf::Vector2f{ bonus_x_dist(rng), 0.f },
                        sf::Vector2f{ 0.f, constants::bonus_speed * powerup_jitter(rng) },
                        sf::Vector2f{ constants::bonus_scale, constants::bonus_scale },
                        constants::white
                    );
                }

                bonus_clock.restart();
                next_bonus_time = bonus_delay_dist(rng);
            }

            // If there are no remaining bricks on the screen, the player has won!
            if (manager.get_all<brick>().empty())
                state = game_state::player_wins;

            // If the player has used up all their lives, the game is over!
            if (lives <= 0)
                state = game_state::game_over;

            // Update the text for the number of remaining lives
            text_lives.setString("Lives: " + std::to_string(lives));

            // Calculate the updated graphics
            manager.update();

            // For every ball, call a function which
            //    For every brick, call a function which
            //         Calls handle_collision with the ball and the brick as arguments
            manager.apply_all<ball>([this](auto& the_ball) {
                manager.apply_all<brick>([&the_ball](auto& the_brick) {
                    handle_collision(the_ball, the_brick);
                });
            });

            // Paddle interaction
            manager.apply_all<ball>([this](auto& the_ball) {
                manager.apply_all<paddle>([&the_ball](auto& the_paddle) {
                    handle_collision(the_ball, the_paddle);
                });
            });
            
            // Apply to all bonuses (life or powerup)
            manager.apply_all<bonus>([this](auto& the_bonus) {
                manager.apply_all<paddle>([this, &the_bonus](auto& the_paddle) {
                    handle_collision(the_bonus, the_paddle, lives);
                });
            });

            // And refresh
            manager.refresh();

            // Display the updated graphics
            manager.draw(game_window);
        }

        game_window.draw(text_lives);
        game_window.display();
    }
}