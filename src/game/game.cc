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

void game::spawn_extra_balls_up_to() {

    const size_t ball_count = manager.get_all<ball>().size();
    if (ball_count >= constants::multiball_extra_balls) return;

    // Reference ball
    auto* ref = manager.get_first<ball>();
    if (!ref) return;

    const sf::Vector2f pos = ref->get_position();
    const sf::Vector2f vel = ref->get_velocity();

    // How many new balls we need
    const size_t needed = constants::multiball_extra_balls - ball_count;

    // Angle step (you can tune this!)
    const float step = 2.0f * constants::multiball_angle / (constants::multiball_extra_balls - 1.0f);

    // Spawn needed balls with symmetric angle offsets around 0°
    for (size_t i = 0; i < needed; ++i) {

        auto& b = manager.create<ball>(
            pos,
            vel,
            sf::Vector2f{ 0.5f, 0.5f },
            active_powerups.fireball ? constants::orange : constants::steel,
            active_powerups.fireball
        );

        // Compute symmetric offset
        const float center = (static_cast<float>(needed) - 1.f) / 2.f;
        const float offset = static_cast<float>(i) - center;

        const float angle = offset * step;
        b.rotate(angle, false);
    }
}


// Helper functions to handle powerups in the game
void game::apply_powerups_to_entities() {

    // Multiball (one-shot effect)
    if (active_powerups.multiball) {
        spawn_extra_balls_up_to();
        active_powerups.multiball = false;
    }

    // Ball effects
    manager.apply_all<ball>([this](ball& b) {

        // Fireball
        b.set_fireball(active_powerups.fireball, 1.0f);

        // Speed change
        float target_ball_speed = constants::ball_speed;
        if (active_powerups.ball_faster)      target_ball_speed = constants::ball_max_speed;
        else if (active_powerups.ball_slower) target_ball_speed = constants::ball_min_speed;
        b.set_velocity(target_ball_speed);

    });

    // Paddle effects
    manager.apply_all<paddle>([this](paddle& p) {
        
        // Scale change
        if (active_powerups.paddle_wider) {
            p.set_scale(true, 2.0f);
        }
        else if (active_powerups.paddle_narrower) {
            p.set_scale(true, 0.5f);
        }
        else {
            p.set_scale(false, 1.0f); // or reset to default
        }

        // Speed change (the paddle speed changes according to the ball speed)
        float target_paddle_speed = constants::paddle_speed;
        if (active_powerups.ball_faster)      target_paddle_speed = constants::paddle_max_speed;
        else if (active_powerups.ball_slower) target_paddle_speed = constants::paddle_min_speed;
        p.set_velocity(target_paddle_speed);

    });
}

powerup_type game::random_powerup() {
    std::uniform_int_distribution<int> dist(0, static_cast<int>(powerup_candidates.size()) - 1);
    powerup_type chosen = powerup_candidates[static_cast<size_t>(dist(rng))];
    // Avoid same powerup twice in a row (try a few times)
    for (int tries = 0; tries < 5 && last_powerup && chosen == *last_powerup; ++tries) {
        chosen = powerup_candidates[static_cast<size_t>(dist(rng))];
    }
    last_powerup = chosen;
    return chosen;
}

game::game() :
    rng(std::random_device{}()),
    text_state(verdana),
    text_fireball(verdana),
    text_lives(verdana),
    text_powerup(verdana),
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
    text_state.setPosition({ constants::window_width / 2.0f - std::ceilf(constants::window_width / 5.1f), (constants::window_height / 2.0f) - std::ceilf(constants::window_height / 8.6f) });
    text_state.setCharacterSize(35);
    text_state.setFillColor(constants::white);
    text_state.setString("Paused");

    text_fireball.setFont(verdana);
    text_fireball.setPosition({ (constants::window_width / 2.0f) - std::ceilf(constants::window_width / 32.0f), constants::window_height - std::ceilf(constants::window_height / 31.8f)});
    text_fireball.setCharacterSize(12);
    text_fireball.setFillColor(constants::orange);
    text_fireball.setString("");

    text_lives.setFont(verdana);
    text_lives.setPosition({ constants::window_width - std::ceilf(constants::window_width / 8.5f), constants::window_height - std::ceilf(constants::window_height / 31.8f) });
    text_lives.setCharacterSize(12);
    text_lives.setFillColor(constants::true_green);
    text_lives.setString("Lives: " + std::to_string(lives));

    text_powerup.setFont(verdana);
    text_powerup.setPosition({ std::ceilf(constants::window_width / 25.0f), constants::window_height - std::ceilf(constants::window_height / 31.8f) });
    text_powerup.setCharacterSize(12);
    text_powerup.setFillColor(constants::true_blue);
    text_powerup.setString("");

    text_instructions.setFont(verdana);
    text_instructions.setPosition({ constants::window_width / 16.0f, constants::window_height / 4.0f });
    text_instructions.setCharacterSize(20);
    text_instructions.setFillColor(constants::white);
    text_instructions.setString(
        "Welcome to Arkanoid!\n\n"
        "Instructions:\n\n"
        "- Left arrow / Move mouse left, to move paddle left\n"
        "- Right Arrow / Move mouse right to move paddle right\n"
        "- P button to pause and resume the game\n"
        "- R button to reset the game\n"
        "- Catch the blue and oragnge balls to apply a powerup\n"
        "- Catch the green ball to get an extra life\n\n"
        "Press any key to start."
    );

}

// (Re)initialize the game
void game::reset() {

    // Reset the number of lives
    lives = constants::player_lives;

    // Destroy all the entities and re-create them
    manager.clear();

    // Reset powerups
    active_powerups.reset();
    text_fireball.setString("");
    text_powerup.setString("");

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
    //thread_local std::mt19937 rng(std::random_device{}());
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
            size_t life_fireball_count = 0;
            size_t powerup_count = 0;
            manager.apply_all<bonus>([&](bonus& b) {
                if (b.get_type() == bonus_type::powerup)
                    ++powerup_count;
                else
                    ++life_fireball_count;
            });

            // If there are no remaining balls on the screen
            if (manager.get_all<ball>().empty()) {
                // Spawn a new one and reduce the player's remaining lives
                auto pos = sf::Vector2f{ constants::window_width / 2.f, constants::window_height / 2.f };
                auto vel = sf::Vector2f{ std::abs(current_ball_velocity.x), -std::abs(current_ball_velocity.y) };
                manager.create<ball>(
                    pos,
                    vel,
                    sf::Vector2f{ 0.5f, 0.5f },
                    constants::steel,
                    false
                );
                active_powerups.reset();
                text_fireball.setString("");
                text_powerup.setString("");
                --lives;
            }

            // Bonus spawning logic
            if (bonus_clock.getElapsedTime().asSeconds() >= next_bonus_time) {

                // Spawn either FIREBALL or LIFE bonus if none exists
                if (life_fireball_count == 0 && std::bernoulli_distribution(1.0f - constants::powerup_prob)(rng)) {

                    static std::bernoulli_distribution spawn_fireball(0.5); // 50% fireball, 50% life
                    const bonus_type type = spawn_fireball(rng) ? bonus_type::fireball : bonus_type::life;

                    float x = std::uniform_real_distribution<float>(
                        bonus::half_width_for(type),
                        constants::window_width - bonus::half_width_for(bonus_type::life)
                    )(rng);

                    manager.create<bonus>(
                        type,
                        sf::Vector2f{ x, 0.f },
                        sf::Vector2f{ 0.f, constants::bonus_speed * life_jitter(rng) },
                        sf::Vector2f{ constants::bonus_scale, constants::bonus_scale },
                        constants::white
                    );
                }

                // Spawn POWERUP bonus if none exists
                if (powerup_count == 0 && std::bernoulli_distribution(constants::powerup_prob)(rng)) {

                    float x = std::uniform_real_distribution<float>(
                        bonus::half_width_for(bonus_type::powerup),
                        constants::window_width - bonus::half_width_for(bonus_type::powerup)
                    )(rng);

                    manager.create<bonus>(
                        bonus_type::powerup,
                        sf::Vector2f{ x, 0.f },
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
            // For every ball, call a function which
            //    For every paddle, call a function which
            //         Calls handle_collision with the ball and the paddle as arguments
            manager.apply_all<ball>([this](auto& the_ball) {
                manager.apply_all<paddle>([&the_ball](auto& the_paddle) {
                    handle_collision(the_ball, the_paddle);
                });
            });
            
            // We will store the message for the last collected bonus / powerup here.
            // Important: we update the text only once per frame (outside loops),
            // instead of setting it multiple times inside apply_all lambdas.
            std::string powerup_msg;

            // Iterate over all bonus entities
            manager.apply_all<bonus>([this, &powerup_msg](bonus& the_bonus) {
                // Iterate over paddles(usually only one paddle exists)
                manager.apply_all<paddle>([this, &the_bonus, &powerup_msg](paddle& the_paddle) {

                    // If bonus and paddle are not interacting, do nothing
                    if (!handle_collision(the_bonus, the_paddle))
                        return;

                    // At this point, a collision happened.
                    // handle_collision() should have destroyed the bonus,
                    // and we now apply the effect (life or powerup).

                    // LIFE bonus: immediately increase lives
                    if (the_bonus.get_type() == bonus_type::life) {
                        ++lives;
                        return;
                    }

                    // FIREBALL bonus: set fireball powerup and change the message color
                    if (the_bonus.get_type() == bonus_type::fireball) {
                        active_powerups.apply(powerup_type::fireball);
                        return;
                    }

                    // POWERUP bonus : pick a random powerup type
                    powerup_type chosen = random_powerup();

                    // Update the active powerups state
                    active_powerups.apply(chosen);

                    // Choose a user-friendly message for the UI based on the chosen powerup
                    switch (chosen) {
                    //case powerup_type::fireball:        powerup_msg = "Fireball!"; break;
                    case powerup_type::multiball:       powerup_msg = "Multiball"; break;
                    case powerup_type::ball_faster:     powerup_msg = "Faster ball"; break;
                    case powerup_type::ball_slower:     powerup_msg = "Slower ball"; break;
                    case powerup_type::paddle_wider:    powerup_msg = "Wider paddle"; break;
                    case powerup_type::paddle_narrower: powerup_msg = "Narrower paddle"; break;
                    case powerup_type::reset_powerups:  powerup_msg = "Reset powerups"; break;
                    default:
                        // If we ever add a new powerup and forget to handle it here,
                        // we won’t crash; but this helps avoid uninitialized messages.
                        powerup_msg.clear();
                        break;
                    }
                });
            });

            // Update lives, fireball, and powerup texts
            text_lives.setString("Lives: " + std::to_string(lives));
            text_fireball.setString(active_powerups.fireball ? "Fireball" : "");
            if (!powerup_msg.empty())
                text_powerup.setString(powerup_msg);

            // Apply the currently active powerup state to entities
            // (e.g. fireball state, ball speed multiplier, paddle scale, etc.)
            // NOTE: apply_powerups_to_entities() should NOT overwrite raw velocity direction.
            // It should modify speed/scale while keeping movement logic intact.
            apply_powerups_to_entities();

            // And refresh
            manager.refresh();

            // Display the updated graphics
            manager.draw(game_window);
        }

        game_window.draw(text_lives);
        game_window.draw(text_fireball);
        game_window.draw(text_powerup);
        game_window.display();
    }
}
