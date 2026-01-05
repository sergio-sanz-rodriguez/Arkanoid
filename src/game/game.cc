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
    rng(std::random_device{}()),
    text_state(font),
    text_fireball(font),
    text_lives(font),
    text_powerup(font),
    text_instructions(font) {

    // Limit the framerate
    game_window.setFramerateLimit(60);      // Max rate is 60 frames per second

    // Set window in paddle to allow mouse interaction
    paddle::set_window(game_window);

    // Load a font from file
    if (!font.openFromFile(constants::font_consola)) {
        std::cerr << "Failed to load font!" << std::endl;
        // Handle font loading failure (could exit, use default font, etc.)
    }

    // Configure our text objects
    text_state.setFont(font);
    text_state.setPosition(
        { (constants::window_width / 2.0f) - std::ceilf(constants::window_width / 5.1f),
        (constants::window_height / 2.0f) - std::ceilf(constants::window_height / 8.6f) });
    text_state.setCharacterSize(35);
    text_state.setFillColor(constants::white);
    text_state.setString("PAUSED");

    text_fireball.setFont(font);
    text_fireball.setPosition(
        { (constants::window_width / 2.0f) - std::ceilf(constants::window_width / 32.0f),
        constants::window_height - std::ceilf(constants::window_height / 31.8f)});
    text_fireball.setCharacterSize(13);
    text_fireball.setFillColor(constants::orange);
    text_fireball.setString("");

    text_lives.setFont(font);
    text_lives.setPosition(
        { constants::window_width - std::ceilf(constants::window_width / 8.5f),
        constants::window_height - std::ceilf(constants::window_height / 31.8f) });
    text_lives.setCharacterSize(13);
    text_lives.setFillColor(constants::true_green);
    text_lives.setString("Lives: " + std::to_string(lives));

    text_powerup.setFont(font);
    text_powerup.setPosition(
        { std::ceilf(constants::window_width / 25.0f),
        constants::window_height - std::ceilf(constants::window_height / 31.8f) });
    text_powerup.setCharacterSize(13);
    text_powerup.setFillColor(constants::true_blue);
    text_powerup.setString("");

    text_instructions.setFont(font);
    text_instructions.setPosition(
        { constants::window_width / 16.0f,
        constants::window_height / 8.0f });
    text_instructions.setCharacterSize(20);
    text_instructions.setFillColor(constants::white);
    text_instructions.setString(
        "WELCOME TO ARKANOID: RECLAIMING THE SOLAR SYSTEM\n\n"
        "YEAR 3056.\n"
        "COSMIC BRICKS HAVE INVADED THE SOLAR SYSTEM.\n"
        "PLANET AFTER PLANET HAS FALLEN.\n"
        "THE SUN ITSELF IS UNDER THREAT.\n\n"
        "YOU ARE A HERO FROM THE ALPHA CENTAURI SYSTEM,\n"
        "SENT TO SAVE YOUR NEIGHBORS.\n\n"
        "YOU CONTROL THE LAST DEFENSE:\n"
        "A PADDLE AND ENERGY BALLS.\n\n"
        "BREAK THE BRICKS.\n"
        "RECLAIM THE PLANETS.\n"
        "SAVE THE SUN.\n\n"
        "INSTRUCTIONS:\n\n"
        "- LEFT ARROW / MOVE MOUSE LEFT: MOVE PADDLE LEFT\n"
        "- RIGHT ARROW / MOVE MOUSE RIGHT: MOVE PADDLE RIGHT\n"
        "- P: PAUSE / RESUME\n"
        "- R: RESET\n"
        "- CATCH BLUE AND ORANGE BALLS TO GET A POWER-UP\n"
        "- CATCH THE GREEN BALL TO GAIN AN EXTRA LIFE\n\n"
        "PRESS ANY KEY TO START."
    );


    // Load sound effects
    audio.load(sfx_id::ball_brick, constants::sfx_ball_brick_path());
    audio.load(sfx_id::ball_paddle, constants::sfx_ball_paddle_path());
    audio.load(sfx_id::ball_wall, constants::sfx_ball_wall_path());
    audio.load(sfx_id::ballstorm, constants::sfx_ballstorm_path());
    audio.load(sfx_id::game_over, constants::sfx_game_over_path());
    audio.load(sfx_id::life_minus, constants::sfx_life_minus_path());
    audio.load(sfx_id::player_wins, constants::sfx_player_wins_path());
    audio.load(sfx_id::powerdown, constants::sfx_powerdown_path());
    audio.load(sfx_id::powerup, constants::sfx_powerup_path());
    audio.load(sfx_id::welcome, constants::sfx_welcome_path());

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

    // Create background picture
    manager.create<background>(0.0f, 0.0f);

    // Create ball object
    manager.create<bouncing_ball>(
        sf::Vector2f{ constants::window_width / 2.0f,
                      constants::window_height - constants::paddle_height },
        sf::Vector2f{ constants::ball_speed,
                     -constants::ball_speed },
        constants::ballstorm_scale,
        constants::steel
    );

    // Create paddle object
    manager.create<paddle>(
        sf::Vector2f{ constants::window_width / 2.0f,
                      constants::window_height - constants::paddle_height },
        sf::Vector2f{ constants::paddle_speed, 0.0f },
        constants::paddle_scale,
        constants::white
    );

    load_level(current_level);

    // Create random number generator and uniform distribution
    //thread_local std::mt19937 rng(std::random_device{}());
    //std::uniform_int_distribution<int> color_dist(0, static_cast<int>(vcolor.size()) - 1);

    //for (int i = 0; i < constants::brick_columns; ++i) {
    //    for (int j = 0; j < constants::brick_rows; ++j) {
    //        // Calculate the brick's position
    //        float x = constants::brick_offset + (i + 1) * constants::brick_width;
    //        float y = (j + 2) * constants::brick_height;

    //        // Create the brick object
    //        sf::Color c = vcolor[j % vcolor.size()]; // Access the color at the correct index
    //        //sf::Color c = vcolor[color_dist(rng)]; // Pick a random color
    //        manager.create<brick>(
    //            sf::Vector2f{ x, y },
    //            constants::brick_scale,
    //            c); // Create the brick with the color
    //    }
    //}

    // Initialize bonus spawn
    bonus_clock.restart();
    next_bonus_time = bonus_delay_dist(rng);

    // Limit the framerate
    game_window.setFramerateLimit(60); // Max rate is 60 frames per second

}

// Load the current difficulty level
void game::load_level(int level) {
    current_level = level;
    spawn_bricks_from_level(get_level(current_level));
}

// Create the layout of the bricks for the current level
void game::spawn_bricks_from_level(const level_data& lvl) {

    // std::uniform_int_distribution<int> color_dist(0, static_cast<int>(vcolor.size()) - 1);
    for (int x = 0; x < lvl.columns; ++x) {
        for (int y = 0; y < lvl.rows; ++y) {

            // Calculate the brick's position
            const auto& cell = lvl.at(x, y);

            // Check if that position has a brick or not; if not, then return
            if (cell.strength == 0)
                continue;

            // Create the brick object: position, scale, and color
            float px = constants::brick_offset + (x + 1) * constants::brick_width;
            float py = (y + 2) * constants::brick_height;

            sf::Color c = vcolor[cell.color_idx % vcolor.size()];
            //sf::Color c = vcolor[color_dist(rng)]; // Pick a random color

            auto& b = manager.create<brick>(
                sf::Vector2f{ px, py },
                constants::brick_scale,
                c
            );

            // Optional: set brick strength if your brick supports it
            b.set_strength(cell.strength);
        }
    }
}

// Function to spawn a multiball object
void game::spawn_multiball() {

    // How many balls are allowed in total after multiball?
    const size_t target_total = constants::multiball_extra_balls;

    // Safety: if target_total is 0 or 1, multiball makes no sense
    if (target_total < 2) return;
    
    // Check if the maximum allowed balls is greated than the current count
    const size_t ball_count = manager.count<bouncing_ball>();
    if (ball_count >= target_total) return;

    // Reference ball
    auto* ref = manager.get_first<bouncing_ball>();
    if (!ref) return;

    const sf::Vector2f pos = ref->get_position();
    const sf::Vector2f vel = ref->get_velocity();

    // How many new balls we need
    const size_t needed = target_total - ball_count;

    // Angle step
    const float divisor = static_cast<float>(target_total - 1);
    const float step = 2.0f * constants::multiball_angle / divisor;

    // Center offset for symmetric distribution (-... 0 ... +)
    const float center = (static_cast<float>(needed) - 1.f) / 2.f;

    // Spawn needed balls with symmetric angle offsets around 0°
    for (size_t i = 0; i < needed; ++i) {

        auto& b = manager.create<bouncing_ball>(
            pos,
            vel,
            constants::ball_scale,
            active_powerups.fireball ? constants::orange : constants::steel,
            active_powerups.fireball
        );

        // Compute symmetric offset around 0°
        const float offset = static_cast<float>(i) - center;
        const float angle = offset * step;
        b.rotate(angle, false);
    }
}

// Function to spaw the storm of balls
void game::spawn_ballstorm() {

    // We assume exactly one paddle exists
    paddle* p = manager.get_first<paddle>();
    if (!p) return;

    const sf::Vector2f paddle_pos = p->get_position();

    // Spawn slightly above the paddle so it doesn't instantly collide
    const sf::Vector2f pos = { paddle_pos.x, paddle_pos.y - p->get_height()};

    // Straight up projectile velocity
    const sf::Vector2f vel = { 0.f, -constants::ballstorm_speed }; // tune speed

    manager.create<ballstorm>(
        pos,
        vel,
        constants::ballstorm_scale,
        constants::white // that is, default
    );

    // Play the sound effect
    audio.play(sfx_id::ballstorm);

}


// Helper functions to handle powerups in the game
// One-shot powerups trigger a single, immediate effect and then end
void game::apply_one_shot_powerups() {

    // Multiball: spawn extra balls only once when collected
    if (active_powerups.multiball) {
        spawn_multiball();
        active_powerups.multiball = false;   // consume the powerup
    }

    // Ball burst: spawn a projectile periodically while active
    if (active_powerups.ballstorm) {

        if (ballstorm_clock.getElapsedTime().asSeconds() >= constants::ballstorm_interval) {
            spawn_ballstorm();
            ballstorm_clock.restart();
        }

        // OPTIONAL: expire burst after N seconds
        if (ballstorm_duration_clock.getElapsedTime().asSeconds() >= constants::ballstorm_duration_in_sec) {
             active_powerups.ballstorm = false;
        }
    }
}

// Sync powerups modify entities continuously and remain active until they are deactivated or replaced
void game::sync_powerups_to_entities() {

    // Ball effects
    manager.apply_all<bouncing_ball>([this](bouncing_ball& b) {

        // Fireball flag controls color + scale internally
        b.set_fireball(active_powerups.fireball, 1.0f);

        // Adjust speed WITHOUT changing direction
        float target_ball_speed = constants::ball_speed;
        if (active_powerups.ball_faster)      target_ball_speed = constants::ball_max_speed;
        else if (active_powerups.ball_slower) target_ball_speed = constants::ball_min_speed;
        b.set_velocity(target_ball_speed);

    });

    // Paddle effects
    manager.apply_all<paddle>([this](paddle& p) {

        // Paddle scale (mutually exclusive)
        if (active_powerups.paddle_wider) {
            p.set_scale(true, 2.0f);
        }
        else if (active_powerups.paddle_narrower) {
            p.set_scale(true, 0.5f);
        }
        else {
            p.set_scale(false, 1.0f); // or reset to default
        }

        // Paddle speed depends on ball speed powerups
        float target_paddle_speed = constants::paddle_speed;
        if (active_powerups.ball_faster)      target_paddle_speed = constants::paddle_max_speed;
        else if (active_powerups.ball_slower) target_paddle_speed = constants::paddle_min_speed;
        p.set_velocity(target_paddle_speed);

    });

}

// Randomly choose one of the available powerups when the bonus object is picked up
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

// Check for any events since the last loop iteration: start, close
void game::handle_window_events() {

    // Handle window events (close button, key presses for start/restart screens).
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

            // Reinitialize previous state
            previous_state = game_state::start_screen;
        }
    }
}

// Function to handle scape, pause, and reset inputs
bool game::handle_global_inputs() {

    // If the user presses "Escape", notify it and get out of the while loop
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        return true;

    // If the user pressees "P", pause the game
    bool ppressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
    if (ppressed && !pause_key_active) {
        state = (state == game_state::paused) ? game_state::running : game_state::paused;
    }
    pause_key_active = ppressed;

    // If the user presses "R", reset the game
    bool rpressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);
    if (rpressed && !reset_key_active) {
        reset();
        state = game_state::running;
    }
    reset_key_active = rpressed;

    return false;

}

// Update state text for the state of the game: paused, game over, player wins
void game::update_state_text() {
    switch (state) {
    case game_state::paused:
        text_state.setPosition({ constants::window_width / 2.0f - 65.0f, (constants::window_height / 2.0f) - (constants::window_height / 8.6f) });
        text_state.setCharacterSize(30);
        text_state.setString("PAUSED");
        break;
    case game_state::game_over:
        text_state.setPosition({ constants::window_width / 16.0f, (constants::window_height / 2.0f) - (constants::window_height / 5.7f) });
        text_state.setCharacterSize(20);
        text_state.setString(
            "  GAME OVER!\n\n"
            "  YOUR MISSION TO RECLAIM THE SOLAR SYSTEM FAILED.\n"
            "  THE SUN IS STILL UNDER THREAT.\n\n"
            "  - PRESS ANY KEY TO TRY AGAIN AND SAVE THE SUN\n"
            "  - PRESS ESCAPE TO QUIT\n"
        );
        break;
    case game_state::player_wins:
        //text_state.setPosition({ constants::window_width / 2.0f - 100.0f, constants::window_height / 2.0f - 100.0f });
        text_state.setPosition({ constants::window_width / 10.0f, (constants::window_height / 2.0f) - (constants::window_height / 5.7f) });
        text_state.setCharacterSize(22);
        text_state.setString(
            "  CONGRATULATIONS!\n\n"
            "  YOU HAVE DEFEATED THE COSMIC BRICKS.\n"
            "  ALL PLANETS HAVE BEEN RECLAIMED,\n"
            "  AND THE SUN IS SAFE AGAIN.\n\n"
            "  THE SOLAR SYSTEM THANKS YOU,\n"
            "  HERO FROM ALPHA CENTAURI!\n\n"
            "  - PRESS ANY KEY TO PLAY AGAIN\n"
            "  - PRESS ESCAPE TO QUIT\n"
        );
        break;
    default:
        text_state.setString("");
        break;
    }
}
// Draw entities + UI
void game::draw_frame() {

    // Always draw on a clean black background
    game_window.clear(sf::Color::Black);

    // START SCREEN: show only instructions
    if (state == game_state::start_screen) {
        if (previous_state == state) {
            audio.play(sfx_id::welcome);
            previous_state = game_state::running;
        }
        game_window.draw(text_instructions);
        game_window.display();
        return;
    }

    // GAME OVER / PLAYER WINS: show only the end-screen text
    if (state == game_state::game_over || state == game_state::player_wins) {
        if (previous_state != state) {
            switch (state) {
            case game_state::game_over:   audio.play(sfx_id::game_over); break;
            case game_state::player_wins: audio.play(sfx_id::player_wins); break;
            default:                      break;
            }
            previous_state = state;
        }
        game_window.draw(text_state);
        game_window.display();
        return;
    }

    // RUNNING / PAUSED: draw the game world
    manager.draw(game_window);

    // PAUSED: draw paused overlay on top
    if (state == game_state::paused) {
        game_window.draw(text_state);
    }

    // UI texts are always visible
    game_window.draw(text_lives);
    game_window.draw(text_fireball);
    game_window.draw(text_powerup);

    // Present the frame
    game_window.display();
}

// Respawn ball if none
void game::ensure_ball_exists() {

    if (manager.has_any<bouncing_ball>())
        return;

    // Spawn one ball, so we need to set the position and velocity
    auto pos = sf::Vector2f{ constants::window_width / 2.f, constants::window_height / 2.f };
    auto vel = sf::Vector2f{ std::abs(current_ball_velocity.x), -std::abs(current_ball_velocity.y) };

    // And create it
    manager.create<bouncing_ball>(
        pos,
        vel,
        constants::ball_scale,
        constants::steel,
        false
    );

    // Losing a ball resets powerups and clocks
    active_powerups.reset();
    text_fireball.setString("");
    text_powerup.setString("");
    ballstorm_ui_active = false;
    ballstorm_clock.restart();
    // fireball_clock.restart();

    // Decrease the number of lives
    --lives;
    audio.play(sfx_id::life_minus);

    // And finish the game if the player runs out of lives
    if (lives <= 0)
        state = game_state::game_over;

}

// Spawn bonus entities
void game::spawn_bonuses() {

    // Count active bonuses
    size_t life_fireball_count = 0;
    size_t powerup_count = 0;
    manager.apply_all<bonus>([&](bonus& b) {
        if (b.get_type() == bonus_type::powerup) ++powerup_count;
        else ++life_fireball_count;
    });

    // Timer check
    if (bonus_clock.getElapsedTime().asSeconds() < next_bonus_time)
        return;

    // Spawn LIFE or FIREBALL
    if (life_fireball_count == 0 && std::bernoulli_distribution(1.0f - constants::powerup_prob)(rng)) {

        static std::bernoulli_distribution spawn_fireball(0.5);
        const bool is_fireball = spawn_fireball(rng);

        const bonus_type type = is_fireball ? bonus_type::fireball : bonus_type::life;

        float x = std::uniform_real_distribution<float>(
            bonus::half_width_for(type),
            constants::window_width - bonus::half_width_for(type)
        )(rng);

        manager.create<bonus>(
            type,
            sf::Vector2f{ x, 0.f },
            sf::Vector2f{ 0.f, constants::bonus_speed * life_jitter(rng) },
             constants::bonus_scale,
            constants::white
        );
    }

    // Spawn POWERUP
    if (powerup_count == 0 && std::bernoulli_distribution(constants::powerup_prob)(rng)) {

        float x = std::uniform_real_distribution<float>(
            bonus::half_width_for(bonus_type::powerup),
            constants::window_width - bonus::half_width_for(bonus_type::powerup)
        )(rng);

        manager.create<bonus>(
            bonus_type::powerup,
            sf::Vector2f{ x, 0.f },
            sf::Vector2f{ 0.f, constants::bonus_speed * powerup_jitter(rng) },
            constants::bonus_scale,
            constants::white
        );
    }

    // Reset timer
    bonus_clock.restart();
    next_bonus_time = bonus_delay_dist(rng);

}

// Powerup logic + message
std::string game::handle_bonus_pickups(paddle& the_paddle) {

    std::string powerup_msg;

    // There is only one paddle
    manager.apply_all<bonus>([this, &powerup_msg, &the_paddle](bonus& the_bonus) {

        // If bonus and paddle are not interacting, do nothing
        if (!handle_collision(the_bonus, the_paddle))
            return;

        // LIFE bonus: increase lives
        if (the_bonus.get_type() == bonus_type::life) {
            ++lives;
            audio.play(sfx_id::powerup);
            return;
        }

        // FIREBALL bonus: set fireball powerup and change the message color
        if (the_bonus.get_type() == bonus_type::fireball) {
            active_powerups.apply(powerup_type::fireball);
            // Optional: restart a fireball timer
            // fireball_clock.restart();
            audio.play(sfx_id::powerup);
            return;
        }

        // POWERUP bonus: apply a random powerup type and choose a user-friendly message for the UI
        powerup_type chosen = random_powerup();
        active_powerups.apply(chosen);

        // Set game mesages and play sound effects
        switch (chosen) {

            case powerup_type::ballstorm:
                powerup_msg = "Ballstorm";
                audio.play(sfx_id::powerup);
                ballstorm_clock.restart();
                ballstorm_duration_clock.restart(); // Optional duration timer
                ballstorm_ui_active = true;
                ballstorm_time_left = constants::ballstorm_duration_in_sec;
                break;

            case powerup_type::ball_faster:
                powerup_msg = "Faster ball";
                audio.play(sfx_id::powerup);
                break;

            case powerup_type::ball_slower:
                powerup_msg = "Slower ball";
                audio.play(sfx_id::powerup);
                break;

            case powerup_type::fireball:
                // Fireball can also expire after X seconds:
                // fireball_clock.restart();
                break;

            case powerup_type::multiball:
                powerup_msg = "Multiball";
                audio.play(sfx_id::powerup);
                break;

            case powerup_type::paddle_wider:
                powerup_msg = "Wider paddle";
                audio.play(sfx_id::powerup);
                break;

            case powerup_type::paddle_narrower:
                powerup_msg = "Narrower paddle";
                audio.play(sfx_id::powerdown);
                break;

            case powerup_type::reset_powerups:
                powerup_msg = "Reset powerups";
                audio.play(sfx_id::powerdown);
                ballstorm_clock.restart();
                ballstorm_duration_clock.restart();
                // fireball_clock.restart();
                break;

            default:
                powerup_msg.clear();
                break;
        }
    });

    return powerup_msg;
}

// Lives, fireball, powerup msg
void game::update_ui_texts(const std::string& powerup_msg) {

    text_lives.setString("Lives: " + std::to_string(lives));

    // persistent state
    text_fireball.setString(active_powerups.fireball ? "Fireball" : "");

    // last pickup message (event)
    if (!powerup_msg.empty())
        text_powerup.setString(powerup_msg);

    // If no new powerup message this frame, show burst countdown (if active)
    if (!ballstorm_ui_active) return;
    
    float elapsed = ballstorm_duration_clock.getElapsedTime().asSeconds();
    float remaining = constants::ballstorm_duration_in_sec - elapsed;

    if (remaining <= 0.0f) {
        ballstorm_ui_active = false;
        text_powerup.setString("");
        return;
    }

    // Update UI text
    std::ostringstream oss;
    oss << "Burst projectiles (" << std::fixed << std::setprecision(0) << remaining << "s)";
    text_powerup.setString(oss.str());

}

// Ball-brick, ball-paddle, bonus-paddle
std::string game::resolve_collisions() {

    // Bouncing ball vs brick
    manager.apply_all<bouncing_ball>([this](bouncing_ball& the_ball) {
        manager.apply_all<brick>([&](brick& the_brick) {
            if (handle_collision(the_ball, the_brick) == sfx_id::ball_brick) {
                audio.play(sfx_id::ball_brick);
            }
        });
    });

    // Bouncing ball vs wall
    manager.apply_all<bouncing_ball>([this](bouncing_ball& the_ball) {
        if (the_ball.consumed_wall_hit())
            audio.play(sfx_id::ball_wall);
    });

    // Burst ball vs brick
    manager.apply_all<ballstorm>([this](ballstorm& the_ball) {
        manager.apply_all<brick>([&](brick& the_brick) {
            if (handle_collision(the_ball, the_brick) == sfx_id::ball_brick) {
                audio.play(sfx_id::ball_brick);
            }
        });
    });

    // Ball vs paddle (we assume exactly one paddle exists)
    paddle* the_paddle = manager.get_first<paddle>();
    if (!the_paddle) return {}; // Something went wrong
    manager.apply_all<bouncing_ball>([this, the_paddle](bouncing_ball& the_ball) {
        if (handle_collision(the_ball, *the_paddle) == sfx_id::ball_paddle) {
            audio.play(sfx_id::ball_paddle);
        }
    });

    // Bonus vs paddle (returns the powerup message)
    return handle_bonus_pickups(*the_paddle);

}

// Checks if the player wins, that is, when all bricks are destroyed
void game::check_win_condition() {
    if (!manager.has_any<brick>())
        state = game_state::player_wins;
}

// Running game function
void game::update_running_frame() {

    // Respawn ball if none
    ensure_ball_exists();

    // Randomly spawn bonus entities
    spawn_bonuses();

    // Update physics / movement
    manager.update();

    // Resolve all collisions
    const std::string msg = resolve_collisions();
    
    // Update UI strings once per frame
    update_ui_texts(msg);

    // Apply the current active powerup state to entities.
    apply_one_shot_powerups();     // Spawns extra balls if needed
    sync_powerups_to_entities();   // Updates ball/paddle properties

    // Cleanup destroyed entities
    manager.refresh();

    // If all bricks are destroyed, then the player wins
    check_win_condition();
}

// (Re)start the game
void game::run() {

    while (game_window.isOpen()) {

        // Clear the screen
        //game_window.clear(sf::Color::Black);

        // Check for any events since the last loop iteration: start, close
        handle_window_events();

        // If the window was closed from events, stop
        if (!game_window.isOpen()) break;

        // Handle global inputs
        if (handle_global_inputs()) break;

        // Update gameplay only when the game is running
        if (state == game_state::running) {
            update_running_frame();
        }

        // Otherwise, update the state overlay text (Paused / Game Over / Win screen)
        else {
            update_state_text();
        }

        // Draw frame: entities and UI
        draw_frame();
    }

}
