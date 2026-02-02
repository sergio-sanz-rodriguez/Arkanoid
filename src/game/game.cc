#include "game.h"

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

// ********** PUBLIC FUNCTIONS **********//

game::game() :
    rng(std::random_device{}()),
    text_state(font),
    text_plasma_ball(font),
    text_antimatter_ball(font),
    text_lives(font),
    text_powerup(font),
    text_press_space(font),
    text_instructions(font),
    level_menu_header(font),
    spr_ui_bg(tex_ui_bg),
    level_achieved(static_cast<size_t>(level_count()), 0) {

    // Set fullscreen mode by default
    game_window.create(
        sf::VideoMode::getDesktopMode(),
        strings::arkanoid_title,
        sf::Style::None
    );

    // Limit the framerate
    game_window.setFramerateLimit(60); // Max rate is 60 frames per second
    
    // Hide system cursor inside the game window
    game_window.setMouseCursorVisible(false);

    // Set window in paddle to allow mouse interaction
    paddle::set_window(game_window);

    // Define the view with the default size and center it
    view.setSize({ constants::window_width, constants::window_height });
    view.setCenter({ constants::window_width / 2.f, constants::window_height / 2.f });

    // Set fullscreen mode
    //set_fullscreen(true);

    // Apply the view with correct letterboxing for the current window size
    update_view();

    // Initialize game state
    previous_state = game_state::running;

    // Load a font from file
    if (!font.openFromFile(assets::font_consola_path())) {
        std::cerr << "Failed to load font!" << std::endl;
        // Handle font loading failure (could exit, use default font, etc.)
    }

    // Load the background texture for the texts
    if (!tex_ui_bg.loadFromFile(assets::txt_background_path())) {
        std::cerr << "Failed to load ui background\n";
    }
    else {
        spr_ui_bg.setTexture(tex_ui_bg, true);

        const auto ts = tex_ui_bg.getSize();
        if (ts.x > 0 && ts.y > 0) {
            const float sx = constants::window_width / static_cast<float>(ts.x);
            const float sy = constants::window_height / static_cast<float>(ts.y);
            spr_ui_bg.setScale({ sx, sy });
            spr_ui_bg.setPosition({ 0.f, 0.f });
        }
    }

    // Configure text objects with default values
    text_state.setFont(font);
    text_state.setPosition(
        { (constants::window_width / 2.0f) - std::ceilf(constants::window_width / 5.1f),
        (constants::window_height / 2.0f) - std::ceilf(constants::window_height / 8.6f) });
    text_state.setCharacterSize(35);
    text_state.setFillColor(ui_colors::off_white);
    text_state.setString("");

    text_plasma_ball.setFont(font);
    text_plasma_ball.setPosition(
        { (constants::window_width / 2.0f) - std::ceilf(constants::window_width / 20.0f),
        constants::window_height - std::ceilf(constants::window_height / 31.8f) });
    text_plasma_ball.setCharacterSize(13);
    text_plasma_ball.setFillColor(ball_color_maps::plasma_ball);
    text_plasma_ball.setString("");

    text_antimatter_ball.setFont(font);
    text_antimatter_ball.setPosition(
        { (constants::window_width / 2.0f) - std::ceilf(constants::window_width / 12.0f),
        constants::window_height - std::ceilf(constants::window_height / 31.8f) });
    text_antimatter_ball.setCharacterSize(13);
    text_antimatter_ball.setFillColor(ball_color_maps::antimatter_ball);
    text_antimatter_ball.setString("");

    text_lives.setFont(font);
    text_lives.setPosition(
        { constants::window_width - std::ceilf(constants::window_width / 8.5f),
        constants::window_height - std::ceilf(constants::window_height / 31.8f) });
    text_lives.setCharacterSize(13);
    text_lives.setFillColor(colors::true_green);
    text_lives.setString("Lives: " + std::to_string(lives));

    text_powerup.setFont(font);
    text_powerup.setPosition(
        { std::ceilf(constants::window_width / 25.0f),
        constants::window_height - std::ceilf(constants::window_height / 31.8f) });
    text_powerup.setCharacterSize(13);
    text_powerup.setFillColor(colors::true_blue);
    text_powerup.setString("");

    text_press_space.setFont(font);
    text_press_space.setPosition(
        { std::ceilf(constants::window_width / 25.0f),
        constants::window_height - std::ceilf(constants::window_height / 31.8f) });
    text_press_space.setCharacterSize(13);
    text_press_space.setFillColor(ui_colors::light_blue);
    text_press_space.setString("PRESS SPACE TO START");

    text_instructions.setFont(font);
    text_instructions.setPosition(
        { constants::window_width / 16.0f,
        constants::window_height / 9.0f });
    text_instructions.setCharacterSize(20);
    text_instructions.setFillColor(ui_colors::off_white);
    text_instructions.setString(static_cast<std::string>(strings::string_instructions));

    // Build level menu window
    level_menu_header.setFont(font);
    level_menu_header.setCharacterSize(20);
    level_menu_header.setFillColor(ui_colors::off_white);
    level_menu_header.setString(static_cast<std::string>(strings::string_first_level_keys));
    level_menu_header.setPosition({ constants::window_width / 6.0f, constants::window_height / 10.0f });

    level_menu_items.clear();
    level_menu_items.reserve(static_cast<size_t>(level_count()));

    float x = constants::window_width / 8.5f;
    float y = constants::window_height / 6.0f + 80.0f; // below header
    float line_step = 52.0f;

    for (int i = 0; i < level_count(); ++i) {
        const auto& lvl = get_level(i);
        sf::Text t(font);
        t.setCharacterSize(20);
        t.setString(lvl.menu_label);
        t.setPosition({ x, y + static_cast<float>(i) * line_step });
        t.setFillColor(ui_colors::off_white);

        level_menu_items.push_back(t);
    }

    // Load sound effects
    audio.load(sfx_id::ball_brick,  assets::sfx_ball_brick_path());
    audio.load(sfx_id::ball_paddle, assets::sfx_ball_paddle_path());
    audio.load(sfx_id::ball_wall,   assets::sfx_ball_wall_path());
    audio.load(sfx_id::ballstorm,   assets::sfx_ballstorm_path());
    audio.load(sfx_id::game_over,   assets::sfx_game_over_path());
    audio.load(sfx_id::life_minus,  assets::sfx_life_minus_path());
    audio.load(sfx_id::enlarge,     assets::sfx_paddle_enlarge_path());
    audio.load(sfx_id::player_wins, assets::sfx_player_wins_path());
    audio.load(sfx_id::powerdown,   assets::sfx_powerdown_path());
    audio.load(sfx_id::powerup,     assets::sfx_powerup_path());
    audio.load(sfx_id::welcome,     assets::sfx_welcome_path());
    
    // Force audio backend/device initialization at startup to avoid first-play frame hitch
    audio.warmup_all();

}

// Full reset to start screen
void game::reset_game() {
    reset_progress();
    level_menu_header.setString(static_cast<std::string>(strings::string_first_level_keys));
    state = game_state::start_screen;
}

// (Re)start the game
void game::run_game() {

    while (game_window.isOpen()) {

        // Check for any events since the last loop iteration: start, close
        handle_window_events();

        // If the window was closed from events, stop
        if (!game_window.isOpen()) break;

        // Handle global inputs
        if (handle_global_inputs()) break;

        // Only recompute overlay text when the state changed
        if (state != previous_state) {
            update_state_text();
        }

        // Update gameplay states
        switch (state) {

            case game_state::running:     update_running_frame(); break;
            case game_state::start_level: update_serve_frame(); break;
            default: break;
        }

        // State might have changed inside update_running_frame()/ensure_ball_exists()
        if (state != previous_state) {
            update_state_text();
        }

        // Draw frame: entities and UI
        draw_frame();
    }
}

// ********** PRIVATE AND HELPER FUNCTIONS **********//

// Determine if a level has been achieved
bool game::is_level_achieved(std::size_t i) const noexcept {
    return i < level_achieved.size() && level_achieved[i];
}

// Mark level as achieved
void game::mark_level_achieved(std::size_t i) {
    if (i < level_achieved.size())
        level_achieved[i] = 1;
}

// Reset the current level
void game::reset_level() {

    // Set up level
    const level_data& lvl = get_level(current_level);
    setup_level(lvl, current_level, false);

    // Change the state of the game
    state = game_state::start_level;

    // Avoid immediat lauch if space is already held
    space_key_active = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
}

void game::reset_progress() {
    std::fill(level_achieved.begin(), level_achieved.end(), 0);
    update_level_menu_colors();
}

// Set up current level 
void game::setup_level(const level_data & lvl, int level_index, bool full_reset) {

    // Clear entity buffers
    manager.clear();

    // Safely check level_index
    const int num_levels = static_cast<int>(bonus_config::level_probs.size());
    if (level_index < 0) level_index = 0;
    if (level_index >= num_levels) level_index = num_levels - 1;

    // Full reset, beginning of the game
    if (full_reset) {
        lives = constants::player_lives;
        current_level = level_index;
    }

    // Reset powerups and timers
    reset_powerups();
    reset_bonus_timers();

    manager.create<background>(0.0f, 0.0f, lvl.background_path);
    spawn_bricks_from_level(lvl);

    // Spawn the paddle first (NOTE: requires lvl.paddle_theme in level_data)
    spawn_paddle(
        { constants::window_width / 2.0f,
          constants::window_height - constants::paddle_height },
        lvl.paddle_color
    );

    // Now spawn the ball relative to the paddle
    if (auto* p = manager.get_first<paddle>()) {
        const float y = p->get_position().y - p->get_height() - 1; // constants::ball_radius;
        spawn_ball({ p->get_position().x, y }, lvl.ball_color);
    }

    // Set velocity to zero and specify the state of the ball as not launched yet.
    manager.apply_all<bouncing_ball>([](bouncing_ball& b) {
        b.reset_for_serve();
    });
}

// Update level menu colors: black/dark_gray if the level is achieved, white if doesn't
void game::update_level_menu_colors() {
    for (size_t i = 0; i < level_menu_items.size(); ++i) {
        bool achieved = is_level_achieved(i);
        level_menu_items[i].setFillColor(achieved ? ui_colors::dark_gray : ui_colors::off_white);
    }
}

// Function to reset powerups including UI texts
void game::reset_powerups() {

    // Rest all powerups
    active_powerups.reset();

    // Clean strings
    multiball_ui_active = false;
    multiball_has_spawned = false;
    text_plasma_ball.setString("");
    text_antimatter_ball.setString("");
    text_powerup.setString("");

    // Change the bouncing ball(s) to original color
    manager.apply_all<bouncing_ball>([](bouncing_ball& b) {
        //b.set_scale(constants::paddle_scale),
        b.set_ball_type(ball_type::regular, 1.0f);
    });

    // Set scale of paddel to original
    manager.apply_all<paddle>([this](paddle& p) {
        p.set_scale(false, 1.0f);
    });

    // Ballstorm UI
    ballstorm_ui_active = false;
    ballstorm_time_left = 0.f;

    // Restart powerup clocks
    ballstorm_clock.restart();
    ballstorm_duration_clock.restart();
}

// Function to reset bonus timers
void game::reset_bonus_timers() {
    bonus_clock.restart();
    next_bonus_time = bonus_delay_dist(rng);
}


// Returns a "letterboxed" view so the game keeps its original aspect ratio.
// Example:
// - The default game board is 600x800 (3:4)
// - The user resizes the window to 1920x1080 (16:9)
// Without letterboxing: the game board gets stretched (distorted).
// With letterboxing: the board keeps 3:4 and SFML adds black bars.
void game::update_view() {

    // Get the size of the current game board
    const auto size = game_window.getSize();

    // Aspect ratio of the *window* (what the user resized to)
    const float windowRatio = static_cast<float>(size.x) / static_cast<float>(size.y);

    // Aspect ratio of the view
    const float viewRatio = view.getSize().x / view.getSize().y;

    // These four values define the viewport rectangle inside the window:
    // - sizeX/sizeY: the relative width/height (0..1) used by the view
    // - posX/posY: where that viewport starts (0..1)
    // Default is full window:
    float sizeX = 1.0f;
    float sizeY = 1.0f;
    float posX = 0.0f;
    float posY = 0.0f;

    // If the window is wider than the view, we need bars on the left and right.
    // If the window is taller than the view, we need bars on the top and bottom.
    const bool windowIsWider = (windowRatio > viewRatio);

    if (windowIsWider) {

        // Window is too wide: shrink the viewport width.
        // The height stays full (1.0).
        sizeX = viewRatio / windowRatio;

        // Center the viewport horizontally: leftover space is divided by 2.
        posX = (1.f - sizeX) / 2.f;

    }
    else {

        // Window is too tall: shrink the viewport height.
        // The width stays full (1.0).
        sizeY = windowRatio / viewRatio;

        // Center the viewport vertically.
        posY = (1.f - sizeY) / 2.f;

    }

    // Apply the viewport: this tells SFML what portion of the window is used by the view.
    // The rest becomes "black bars" because your window is cleared with black.
    view.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));

    //Update the game board (or window)
    game_window.setView(view);
}

// Spawn the bouncing ball
void game::spawn_ball(sf::Vector2f pos, ball_colors color) {
    manager.create<bouncing_ball>(
        pos,
        sf::Vector2f{ 0.0f, 0.0f }, // No movement { constants::ball_speed, -constants::ball_speed },
        constants::ball_scale,
        bouncing_ball::to_sf_color(color),
        ball_type::regular
    );
}

// Spawn the paddle
void game::spawn_paddle(sf::Vector2f pos, paddle_colors color) {
    manager.create<paddle>(
        pos,
        sf::Vector2f{ constants::paddle_speed, 0.0f },
        constants::paddle_scale,
        colors::white,
        color
    );
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
            float px = brick_config::brick_offset_width  + x * brick_config::brick_width;
            float py = brick_config::brick_offset_height + y * brick_config::brick_height;

            // Define colors
            const auto& colors = get_color_vector(lvl.brick_color_map);
            const bool is_indestructible = cell.strength == brick_config::indestructible_strength;
            sf::Color color = is_indestructible ? brick_color_maps::indestructible : colors[cell.color_idx];
            //sf::Color c = vcolor[color_dist(rng)]; // Pick a random color

            auto& b = manager.create<brick>(
                sf::Vector2f{ px, py },
                constants::brick_scale,
                color
            );

            // Optional: set brick strength if your brick supports it
            b.set_strength(cell.strength);
            b.set_indestructible(is_indestructible);
        }
    }
}

// Function to spawn a multiball object
void game::spawn_multiball() {

    // Get current ball count
    const size_t before = manager.count<bouncing_ball>();

    // How many balls are allowed in total after multiball?
    const size_t target_total = static_cast<size_t>(constants::multiball_total_balls);

    // Safety: if target_total is 0 or 1, multiball makes no sense
    if (target_total < 2) return;
    
    // Check if the maximum allowed balls is greated than the current count
    const size_t ball_count = manager.count<bouncing_ball>();
    if (ball_count >= target_total) return;

    // Set reference ball
    auto* ref = manager.get_first<bouncing_ball>();
    if (!ref) return;

    // Determine position, velocity, and scale of the reference ball
    const sf::Vector2f pos = ref->get_position();
    const sf::Vector2f vel = ref->get_velocity();
    const sf::Vector2f scale = constants::ball_scale;

    // If ref ball isn't moving for any reason, abort (or set a default upward vel)
    const float len2 = vel.x * vel.x + vel.y * vel.y;
    if (len2 < 0.0001f) return;

    // Decide ball type and color based on active powerups
    ball_type type = ball_type::regular;
    sf::Color color = ball_color_maps::bouncing_ball;
    if (active_powerups.plasma_ball) {
        type = ball_type::plasma;
        color = ball_color_maps::plasma_ball;
    }
    else if (active_powerups.antimatter_ball) {
        type = ball_type::antimatter;
        color = ball_color_maps::antimatter_ball;
    }

    // How many new balls we need
    const size_t needed = target_total - ball_count;

    // Angle step
    const float divisor = static_cast<float>(target_total - 1);
    const float step = 2.0f * constants::multiball_angle / divisor;

    // Center offset for symmetric distribution (-... 0 ... +)
    const float center = std::fmaxf(((float)needed - 1.f) / 2.f, 0.5f);

    // Spawn needed balls with symmetric angle offsets around 0°
    for (size_t i = 0; i < needed; ++i) {

        auto& b = manager.create<bouncing_ball>(pos, vel, scale, color, type);

        // Compute symmetric offset around 0°
        const float offset = static_cast<float>(i) - center;
        const float angle = offset * step;
        b.launch_keep_velocity();
        b.rotate(angle, false);
    }

    const size_t after = manager.count<bouncing_ball>();
    if (after > 1 && after > before) {
        multiball_has_spawned = true;
    }
}

// Function to spaw the storm of balls
void game::spawn_ballstorm() {

    // We assume exactly one paddle exists
    paddle* p = manager.get_first<paddle>();
    if (!p) return;

    // Get its position
    const sf::Vector2f paddle_pos = p->get_position();

    // Spawn slightly above the paddle so it doesn't instantly collide
    const sf::Vector2f pos = { paddle_pos.x, paddle_pos.y - p->get_height()};

    // Straight up projectile velocity
    const sf::Vector2f vel = { 0.f, -constants::ballstorm_speed }; // tune speed

    manager.create<ballstorm>(
        pos,
        vel,
        constants::ballstorm_scale,
        colors::white // that is, default
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
        if (manager.count<bouncing_ball>() > 1)
            multiball_has_spawned = true;
        active_powerups.multiball = false; // Consume the powerup
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

// Sync powerups to modify entities continuously and remain active until they are deactivated or replaced
void game::sync_powerups_to_entities() {

    // Ball effects
    manager.apply_all<bouncing_ball>([this](bouncing_ball& b) {

        // Determine ball type based on active powerups: color + scale internally
        if (active_powerups.plasma_ball)          b.set_ball_type(ball_type::plasma, 1.0f);
        else if (active_powerups.antimatter_ball) b.set_ball_type(ball_type::antimatter, 1.0f);
        else                                      b.set_ball_type(ball_type::regular, 1.0f);

        // Adjust speed WITHOUT changing direction
        if (active_powerups.ball_faster)          b.set_velocity(constants::ball_max_speed);
        else if (active_powerups.ball_slower)     b.set_velocity(constants::ball_min_speed);
        else                                      b.set_velocity(constants::ball_speed);

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

    // Handle window events (close button, key presses for start/restart screens, resizing).
    while (auto event = game_window.pollEvent()) {
        
        // We close the window
        if (event->is<sf::Event::Closed>()) {
            game_window.close();
            continue;
        }

        // We resize the window
        if (event->is<sf::Event::Resized>()) {
            update_view();
            continue;
        }

        // Use Space to continue the game
        if (auto* kp = event->getIf<sf::Event::KeyPressed>()) {

            // START SCREEN: Space -> level_select
            if (state == game_state::start_screen) {
                if (kp->code == sf::Keyboard::Key::Space) {
                    state = game_state::level_select;
                    space_key_active = true;
                    update_level_menu_colors();
                }
                continue;
            }
            
            // Allow ESC to quit is already in handle_global_inputs()
            if (state == game_state::level_select) {

                int chosen = -1;
                switch (kp->code) {
                    case sf::Keyboard::Key::Num1: chosen = 0; break;
                    case sf::Keyboard::Key::Num2: chosen = 1; break;
                    case sf::Keyboard::Key::Num3: chosen = 2; break;
                    case sf::Keyboard::Key::Num4: chosen = 3; break;
                    case sf::Keyboard::Key::Num5: chosen = 4; break;
                    case sf::Keyboard::Key::Num6: chosen = 5; break;
                    case sf::Keyboard::Key::Num7: chosen = 6; break;
                    case sf::Keyboard::Key::Num8: chosen = 7; break;
                    case sf::Keyboard::Key::Num9: chosen = 8; break;

                    // Optional: also support Numpad
                    case sf::Keyboard::Key::Numpad1: chosen = 0; break;
                    case sf::Keyboard::Key::Numpad2: chosen = 1; break;
                    case sf::Keyboard::Key::Numpad3: chosen = 2; break;
                    case sf::Keyboard::Key::Numpad4: chosen = 3; break;
                    case sf::Keyboard::Key::Numpad5: chosen = 4; break;
                    case sf::Keyboard::Key::Numpad6: chosen = 5; break;
                    case sf::Keyboard::Key::Numpad7: chosen = 6; break;
                    case sf::Keyboard::Key::Numpad8: chosen = 7; break;
                    case sf::Keyboard::Key::Numpad9: chosen = 8; break;
                    default: break;
                }

                if (chosen != -1) {

                    // DISABLE achieved levels
                    if (is_level_achieved(static_cast<std::size_t>(chosen))) {
                        continue;
                    }

                    current_level = chosen;

                    // Start from selected level with full lives (usually what you want)
                    lives = constants::player_lives;
                    //reset_powerups();
                    //reset_bonus_timers();

                    // Update level list header
                    level_menu_header.setString(static_cast<std::string>(strings::string_next_level_keys));

                    const level_data& lvl = get_level(current_level);
                    setup_level(lvl, current_level, /*full_reset=*/false);

                    // Stop welcome audio
                    audio.stop(sfx_id::welcome);

                    state = game_state::start_level;
                    //space_key_active = true;
                    space_key_active = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
                }

                continue;
            }

            // Handle the GAME OVER and PLAYER WINS status
            if (state == game_state::game_over || state == game_state::player_wins) {

                // Only Space restarts
                if (kp->code != sf::Keyboard::Key::Space)
                    continue;

                // Reset the level list only if the player finished the whole game
                //if (state == game_state::player_wins) {
                //    reset_progress();
                //}

                // Back to intro story/instructions or level select
                //reset_game(game_state::level_select);
                reset_game();

                // Avoid immediate re-triggering / accidental launch
                space_key_active = true;
                continue;
            }
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
        reset_level();
    }
    reset_key_active = rpressed;

    return false;
}

// Function to handle space to lauch the ball
void game::handle_ball_launch_input() {

    const bool space_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    // Only allow launching during serve state.
    // Still update the latch so transitions don't accidentally trigger a launch.
    if (state != game_state::start_level) {
        space_key_active = space_pressed;
        return;
    }

    // One-shot key press
    if (space_pressed && !space_key_active) {

        bool launched_any = false;

        manager.apply_all<bouncing_ball>([&](bouncing_ball& b) {
            if (!b.is_launched()) {
                b.launch();
                launched_any = true;
            }
        });

        if (launched_any)
            state = game_state::running;
    }

    space_key_active = space_pressed;
}

void game::stick_unlaunched_balls_to_paddle() {
    paddle* p = manager.get_first<paddle>();
    if (!p) return;

    // Put the ball centered on the paddle, slightly above it
    const float y = p->get_position().y - p->get_height() - 1; // or -some offset you like FIXME

    manager.apply_all<bouncing_ball>([&](bouncing_ball& b) {
        if (!b.is_launched()) {
            b.set_velocity( 0.0f );
            b.set_position({ p->get_position().x, y });
        }
    });
}

// Update state text for the state of the game: paused, game over, player wins
void game::update_state_text() {
    switch (state) {
    case game_state::paused:
        text_state.setPosition({ constants::window_width / 2.0f - 65.0f, (constants::window_height / 2.0f) - (constants::window_height / 8.6f) });
        text_state.setCharacterSize(30);
        text_state.setString(static_cast<std::string>(strings::string_paused));
        text_state.setFillColor(ui_colors::warm_amber);
        break;
    case game_state::game_over:
        text_state.setPosition({ constants::window_width / 16.0f, (constants::window_height / 2.0f) - (constants::window_height / 5.7f) });
        text_state.setCharacterSize(20);
        text_state.setString(static_cast<std::string>(strings::string_game_over));
        text_state.setFillColor(ui_colors::game_over);
        break;
    case game_state::player_wins:
        //text_state.setPosition({ constants::window_width / 2.0f - 100.0f, constants::window_height / 2.0f - 100.0f });
        text_state.setPosition({ constants::window_width / 10.0f, (constants::window_height / 2.0f) - (constants::window_height / 5.7f) });
        text_state.setCharacterSize(22);
        text_state.setString(static_cast<std::string>(strings::string_player_wins));
        text_state.setFillColor(ui_colors::soft_green);
        break;
    case game_state::start_level:
        //text_state.setPosition(level_menu_header.getPosition());
        //text_state.setCharacterSize(20);
        text_state.setString(""); // level_menu_header.getString());
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
        if (previous_state != state) {
            audio.play(sfx_id::welcome);
        }
        game_window.draw(spr_ui_bg);
        game_window.draw(text_instructions);
        game_window.display();
        previous_state = state;
        return;
    }

    // LEVEL SELECT: show the level list
    if (state == game_state::level_select) {
        game_window.draw(spr_ui_bg);
        game_window.draw(level_menu_header);
        for (auto& t : level_menu_items) game_window.draw(t);
        game_window.display();
        previous_state = state;
        return;
    }

    // GAME OVER / PLAYER WINS: show only the end-screen text
    if (state == game_state::player_wins || state == game_state::game_over) { // (state == game_state::start_level || 
        if (previous_state != state) {
            if (state == game_state::player_wins) audio.play(sfx_id::player_wins);
            else                                  audio.play(sfx_id::game_over);
        }
        game_window.draw(spr_ui_bg);
        game_window.draw(text_state);
        game_window.display();
        previous_state = state;
        return;
    }

    // GAMEPLAY (start_level, running, paused)
    manager.draw(game_window);

    // PAUSED: draw paused overlay on top
    if (state == game_state::paused) {
        game_window.draw(text_state);
    }

    // UI texts are always visible
    game_window.draw(text_lives);
    game_window.draw(text_plasma_ball);
    game_window.draw(text_antimatter_ball);
    game_window.draw(text_powerup);

    // START LEVEL: show a messange to press space to begin
    if (state == game_state::start_level) {
        game_window.draw(text_press_space);
    }

    // Present the frame
    game_window.display();

    // Track last drawn state
    previous_state = state;
}

// Respawn ball if none
void game::ensure_ball_exists() {

    // Check if there is at least one ball in the board
    if (manager.has_any<bouncing_ball>()) return;

    // Losing a ball resets powerups and clocks
    reset_powerups();
    reset_bonus_timers();

    // Spawn the ball from the center of the paddle.
    paddle* p = manager.get_first<paddle>();
    if (!p) return; // For safety
    const float y = p->get_position().y - p->get_height() - 1; // -constants::ball_radius;
    const level_data& lvl = get_level(current_level);
    spawn_ball({ p->get_position().x, y }, lvl.ball_color);

    // Reset ball state and velocity
    manager.apply_all<bouncing_ball>([](bouncing_ball& b) {
        b.reset_for_serve();
    });
    
    // Set the state to start_level
    state = game_state::start_level;

    //space_key_active = true; // Optional: avoids instant launch if space is held
    space_key_active = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    // Decrease the life count
    --lives;

    // Update sound effect
    audio.play(sfx_id::life_minus);

    // And enable the game-over flag if the player runs out of lives
    if (lives <= 0) {
        state = game_state::game_over;
        update_state_text(); // Optional
        lives = constants::player_lives;
    }
}

// Spawn bonus entities
void game::spawn_bonuses() {

    // Count active bonuses
    size_t life_plasma_ball_count = 0;
    size_t powerup_count = 0;
    manager.apply_all<bonus>([&](bonus& b) {
        if (b.get_type() == bonus_type::powerup) ++powerup_count;
        else ++life_plasma_ball_count;
    });

    // Timer check
    if (bonus_clock.getElapsedTime().asSeconds() < next_bonus_time)
        return;

    // Use the probability distribution for the current level for selecting the bonuses
    const auto& probs = bonus_config::level_probs[current_level];
    const float powerup_prob = probs[0];
    const float special_prob = probs[1];

    // Spawn LIFE, PLASMA BALL or ANTIMATTER BALL
    if (life_plasma_ball_count == 0 && std::bernoulli_distribution(special_prob)(rng)) {

        std::discrete_distribution<int> pick(probs.begin()+2, probs.end());
        int idx = pick(rng);
        bonus_type type =
            (idx == 0) ? bonus_type::life :
            (idx == 1) ? bonus_type::plasma_ball :
            bonus_type::antimatter_ball;

        float x = std::uniform_real_distribution<float>(
            bonus::half_width_for(type),
            constants::window_width - bonus::half_width_for(type)
        )(rng);

        // Create the bonus
        manager.create<bonus>(
            type,
            sf::Vector2f{ x, 0.f },
            sf::Vector2f{ 0.f, bonus_config::bonus_speed * life_jitter(rng) },
             constants::bonus_scale,
            colors::white
        );
    }

    // Spawn POWERUP
    if (powerup_count == 0 && std::bernoulli_distribution(powerup_prob)(rng)) {

        // Randomly select an X position to spawn the powerup bonus
        float x = std::uniform_real_distribution<float>(
            bonus::half_width_for(bonus_type::powerup),
            constants::window_width - bonus::half_width_for(bonus_type::powerup)
        )(rng);

        // Create the bonus
        manager.create<bonus>(
            bonus_type::powerup,
            sf::Vector2f{ x, 0.f },
            sf::Vector2f{ 0.f, bonus_config::bonus_speed * powerup_jitter(rng) },
            constants::bonus_scale,
            colors::white
        );
    }

    // Reset timer
    bonus_clock.restart();
    next_bonus_time = bonus_delay_dist(rng);
}

// Handling bonus pickups: logic + message
std::string game::handle_bonus_pickups(paddle& the_paddle) {

    std::string powerup_msg;
    bool picked = false;

    // There is only one paddle
    manager.apply_all<bonus>([&](bonus& the_bonus) {

        // Only one pickup per frame
        if (picked)
            return;

        // If bonus and paddle are not interacting, do nothing
        if (!handle_collision(the_bonus, the_paddle))
            return;

        picked = true;

        switch (the_bonus.get_type())
        {
            // LIFE bonus: increase lives
            case bonus_type::life:
                ++lives;
                audio.play(sfx_id::powerup);
                return;

            // PLASMA BALL bonus: set plasma_ball powerup and change the message color
            case bonus_type::plasma_ball:
                active_powerups.apply(powerup_type::plasma_ball);
                // Optional: restart a plasma_ball timer
                // plasma_ball_clock.restart();
                audio.play(sfx_id::powerup);
                return;

            // ANTIMATTER BALL bonus: set antimatter_ball powerup and change the message color
            case bonus_type::antimatter_ball:
                active_powerups.apply(powerup_type::antimatter_ball);
                audio.play(sfx_id::powerup);
                return;

            case bonus_type::powerup:
                break; // handled below
        }

        // POWERUP bonus: apply a random powerup type and choose a user-friendly message for the UI
        powerup_type chosen = random_powerup();
        active_powerups.apply(chosen);

        // Disable multiball message when required
        if (chosen != powerup_type::multiball)
            multiball_ui_active = false;

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

            case powerup_type::plasma_ball:
                // plasma_ball can also expire after X seconds:
                // plasma_ball_clock.restart();
                break;

            case powerup_type::antimatter_ball:
                // antimatter_ball can also expire after X seconds:
                // antimatter_ball_clock.restart();
                break;
            case powerup_type::multiball:
                powerup_msg = "Multiball";
                multiball_ui_active = true;
                multiball_has_spawned = false;
                audio.play(sfx_id::powerup);
                break;

            case powerup_type::paddle_wider:
                powerup_msg = "Wider paddle";
                //audio.play(sfx_id::powerup);
                audio.play(sfx_id::enlarge);
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
                // plasma_ball_clock.restart();
                break;

            default:
                powerup_msg.clear();
                break;
        }
    });

    return powerup_msg;
}

// Lives, plasma_ball, powerup msg
void game::update_ui_texts(const std::string& powerup_msg) {

    text_lives.setString("Lives: " + std::to_string(lives));

    // Persistent state
    if (active_powerups.plasma_ball) {
        text_plasma_ball.setString("Plasma ball");
        text_antimatter_ball.setString("");
    }
    else if (active_powerups.antimatter_ball) {
        text_plasma_ball.setString("");
        text_antimatter_ball.setString("Antimatter ball");
    }
    else {
        text_plasma_ball.setString("");
        text_antimatter_ball.setString("");
    }

    // Last pickup message (event)
    if (!powerup_msg.empty())
        text_powerup.setString(powerup_msg);
    
    // Multiball UI lifetime:
    // show after pickup until:
    // 1) only one ball is left (AFTER multiball actually produced >1 ball at least once)
    // 2) another powerup is caught except the one-shot ones (life, plasma, antimatter)
    // 3) a life is gone (reset_powerups already clears it)
    if (multiball_ui_active && multiball_has_spawned) {
        if (manager.count<bouncing_ball>() <= 1) {
            multiball_ui_active = false;
            multiball_has_spawned = false;
            if (text_powerup.getString() == "Multiball")
                text_powerup.setString("");
        }
    }

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
    oss << "Ballstorm (" << std::fixed << std::setprecision(0) << remaining << "s)";
    text_powerup.setString(oss.str());
}

// Ball-brick, ball-paddle, bonus-paddle
std::string game::resolve_collisions() {

    // Bouncing ball vs brick
    manager.apply_all<bouncing_ball>([this](bouncing_ball& the_ball) {
        bool hit_brick = false;
        manager.apply_all<brick>([&](brick& the_brick) {
            if (hit_brick) return;
            if (handle_collision(the_ball, the_brick) == sfx_id::ball_brick) {
                hit_brick = true;
                audio.play(sfx_id::ball_brick);
            }
        });
    });

    // Bouncing ball vs wall
    manager.apply_all<bouncing_ball>([this](bouncing_ball& the_ball) {
        if (the_ball.consumed_wall_hit()) {
            audio.play(sfx_id::ball_wall);
        }
    });

    // Burst ball vs brick
    manager.apply_all<ballstorm>([this](ballstorm& the_ball) {
        bool hit_brick = false;
        manager.apply_all<brick>([&](brick& the_brick) {
            if (hit_brick) return;
            if (handle_collision(the_ball, the_brick) == sfx_id::ball_brick) {
                hit_brick = true;
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

// Checks if the player wins (level cleared), i.e. when all DESTRUCTIBLE bricks are destroyed
void game::check_win_condition() {

    // Only check victory while actively running
    if (state != game_state::running)
        return;

    // If at least one brick is destructible, the level is NOT finished yet.
    bool has_destructible = false;
    manager.apply_all<brick>([&](const brick& b) {
        if (!b.is_indestructible())
            has_destructible = true;
    });
    if (has_destructible)
        return;

    // Cleared current level
    audio.play(sfx_id::player_wins);

    // LEVEL CLEARED
    // Mark this level as achieved (so the menu can render it differently)
    mark_level_achieved(static_cast<std::size_t>(current_level));
    update_level_menu_colors();

    // If ALL levels are achieved -> game finished
    bool all_done = true;
    for (int i = 0; i < level_count(); ++i) {
        if (!is_level_achieved(static_cast<std::size_t>(i))) {
            all_done = false;
            break;
        }
    }

    if (all_done) {
        state = game_state::player_wins;
        space_key_active = true; // avoid accidental input carryover
        return;
    }

    // Otherwise: go back to level selection (player picks next)
    state = game_state::level_select;

    // Prevent key carry-over (Space held down etc.)
    space_key_active = true;
}

// Running game function
void game::update_running_frame() {

    // Respawn ball if none
    ensure_ball_exists();

    // The ball should follow the paddle
    stick_unlaunched_balls_to_paddle();

    // DO NOT spawn bonuses or powerups until ball is launched
    if (state == game_state::running) {

        // Randomly spawn bonus entities
        spawn_bonuses();

        // Apply the current active powerup state to entities.
        apply_one_shot_powerups();     // Spawns extra balls if needed
        sync_powerups_to_entities();   // Updates ball/paddle properties
    }

    // Update physics / movement
    manager.update();

    // Resolve all collisions
    const std::string msg = resolve_collisions();

    // Update UI strings once per frame
    update_ui_texts(msg);

    // Cleanup destroyed entities
    manager.refresh();

    // If all bricks are destroyed, then the player wins
    check_win_condition();
}

// Just allow paddle movement and keep ball attached
void game::update_serve_frame() {
    
    // Let paddle update/move (if paddle reads input in update())
    manager.update();

    // Space launches (if pressed)
    handle_ball_launch_input();

    // Keep the ball on the paddle while not launched
    stick_unlaunched_balls_to_paddle();
}
