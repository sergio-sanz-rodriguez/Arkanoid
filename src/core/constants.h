#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <string_view>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Code for a "breakout" game
// Based on a talk by Vittorio Romeo
// Uses the SFML graphics & audio library

struct constants {
    static constexpr const char* arkanoid_title{ "Arkanoid: Reclaiming the Solar System" };
    static constexpr int window_width{ 638 }; //48 - 498
    static constexpr int window_height{ 860 }; // { 960 };
    static constexpr float ball_speed{ 8.0f };
    static constexpr float ball_max_speed{ 16.0f };
    static constexpr float ball_min_speed{ 6.0f };
    static constexpr float ball_speed_step{ 0.5f };
    static constexpr float ballstorm_speed{ 10.0f };
    static constexpr float ballstorm_interval{ 0.2f }; // fire every 200 ms
    static constexpr float ballstorm_duration_in_sec{ 10.0f }; // lasts 5 secs
    static constexpr float paddle_width{ 75.0f };
    static constexpr float paddle_height{ 40.0f };
    static constexpr float paddle_speed{ 10.0f };
    static constexpr float paddle_max_speed{ 20.0f }; // { 21.33f };
    static constexpr float paddle_min_speed{ 8.0f };
    static constexpr float paddle_speed_step{
        ball_speed_step * (paddle_max_speed - paddle_min_speed) / (ball_max_speed - ball_min_speed) 
    };
    static constexpr float paddle_scale_width{ 0.411f };
    static constexpr float paddle_scale_height{ 0.25f };
    static constexpr float rotation_angle{ 1.0f };
    static constexpr float max_angle{ 60.0f };
    static constexpr float multiball_angle{ 2.0f };
    static constexpr int   multiball_extra_balls{ 3 };
    static constexpr float pi{ 3.14159265358979323846f };
    static constexpr int max_brick_strength{ 3 }; // The brick needs to be hit X times before it disappears
    static constexpr int indestructible_strength{ 255 };
    static constexpr int brick_columns{ 12 };
    static constexpr int brick_rows{ 12 };
    static constexpr float brick_width{ 43.0f };
    static constexpr float brick_height{ 20.0f };
    static constexpr float brick_offset_width{ brick_width * 0.5f }; // brick_width * 0.9f
    static constexpr float brick_offset_height{ brick_height * 0.5f }; // brick_width * 0.9f
    static constexpr int brick_alpha_hit3{ 80 };  // Dull
    static constexpr int brick_alpha_hit2{ 170 }; // Medium
    static constexpr int brick_alpha_hit1{ 255 }; // Bright
    static constexpr int max_active_bonuses{ 2 };
    static constexpr float bonus_speed{ 6.0f };
    static constexpr float bonus_max_speed{ 16.0f };
    static constexpr float bonus_min_speed{ 6.0f };
    static constexpr float bonus_speed_jitter{ 0.9f }; // A jitter better 0.9 * speed and (1/0.9) * speed.
    static constexpr float bonus_speed_step{ 0.5f };
    static constexpr float powerup_prob{ 0.7f };
    static constexpr int player_lives{ 3 };
    static constexpr int max_sounds{ 16 };
    static constexpr sf::Vector2f ball_scale{ 0.5f, 0.5f };
    static constexpr sf::Vector2f ballstorm_scale{ 0.5f, 0.5f };
    static constexpr sf::Vector2f brick_scale{ 1.0f, 1.0f };
    static constexpr sf::Vector2f bonus_scale{ 1.0f, 1.0f };
    static constexpr sf::Vector2f paddle_scale{ 0.411f, 0.25f };
    static constexpr sf::Color anthracite{ 53, 60, 64 };         // Anthracite (indestructible)
    static constexpr sf::Color white{ 255, 255, 255, 255 };      // White
    static constexpr sf::Color red{ 205, 92, 92, 255 };          // Light Coral
    static constexpr sf::Color orange{ 255, 129, 24, 255 };      // Dark Orange
    static constexpr sf::Color green{ 60, 179, 113, 255 };       // Medium Sea Green
    static constexpr sf::Color cyan{ 0, 206, 209, 255 };         // Dark Turquoise
    static constexpr sf::Color blue{ 70, 130, 180, 255 };        // Calming Blue
    static constexpr sf::Color steel{ 156, 208, 255, 255 };      // Steel Blue
    static constexpr sf::Color true_red{ 250, 0, 0, 255 };       // True Red
    static constexpr sf::Color true_green{ 0, 255, 0, 255 };     // True Green
    static constexpr sf::Color true_blue{ 0, 0, 255, 255 };      // True Blue
    static constexpr sf::Color gold_base{ 212, 175, 55, 255 };   // True Blue
    static constexpr sf::Color gold_shiny{ 255, 215, 0, 255 };   // True Blue
    static constexpr sf::Color gold_shadow{ 184, 134, 11, 255 }; // True Blue
    static constexpr std::string_view image_dir{ "..\\..\\assets\\images\\" };
    static constexpr std::string_view sfx_dir{ "..\\..\\assets\\sfx\\" };
    static constexpr std::string_view img_background_level1_filename{ "level_1.png" };
    static constexpr std::string_view img_background_level2_filename{ "level_2.png" };
    static constexpr std::string_view img_background_level3_filename{ "level_3.png" };
    static constexpr std::string_view img_background_level4_filename{ "level_4.png" };
    static constexpr std::string_view img_background_level5_filename{ "level_5.png" };
    static constexpr std::string_view img_background_level6_filename{ "level_6.png" };
    static constexpr std::string_view img_background_level7_filename{ "level_7.png" };
    static constexpr std::string_view img_background_level8_filename{ "level_8.png" };
    static constexpr std::string_view img_background_level9_filename{ "level_9.png" };
    static constexpr std::string_view img_background_level10_filename{ "level_10.png" };
    static constexpr std::string_view img_ball_filename{ "ball_llwhite.png" };
    static constexpr std::string_view img_ballstorm_filename{ "burst.png" };
    static constexpr std::string_view img_paddle_filename{ "paddle_gray.png" };
    static constexpr std::string_view img_brick_filename{ "brick_s.png" };
    static constexpr std::string_view img_plasma_ball_filename{ "ball_orange.png" };
    static constexpr std::string_view img_life_filename{ "ball_green.png" };
    static constexpr std::string_view img_powerup_filename{ "ball_blue.png" };
    static constexpr std::string_view sfx_ball_brick_filename{ "ball_brick.wav" };
    static constexpr std::string_view sfx_ball_paddle_filename{ "ball_paddle.wav" };
    static constexpr std::string_view sfx_ball_wall_filename{ "ball_wall.wav" };
    static constexpr std::string_view sfx_ballstorm_filename{ "ballstorm.wav" };
    static constexpr std::string_view sfx_game_over_filename{ "game_over.wav" };
    static constexpr std::string_view sfx_life_minus_filename{ "life_minus.wav" };
    static constexpr std::string_view sfx_player_wins_filename{ "player_wins.wav" };
    static constexpr std::string_view sfx_powerdown_filename{ "powerdown.wav" };
    static constexpr std::string_view sfx_powerup_filename{ "powerup.wav" };
    static constexpr std::string_view sfx_welcome_filename{ "intro.wav" };
    //static constexpr std::string_view font_filename{ "/usr/local/share/fonts/webfonts/verdana.ttf" };
    static constexpr std::string_view font_verdana{ "..\\..\\assets\\fonts\\verdana.ttf" };
    static constexpr std::string_view font_consola{ "..\\..\\assets\\fonts\\consola.ttf" };
    static std::string img_background_level1_path() { return std::string(image_dir) + std::string(img_background_level1_filename); }
    static std::string img_background_level2_path() { return std::string(image_dir) + std::string(img_background_level2_filename); }
    static std::string img_background_level3_path() { return std::string(image_dir) + std::string(img_background_level3_filename); }
    static std::string img_background_level4_path() { return std::string(image_dir) + std::string(img_background_level4_filename); }
    static std::string img_ball_path() { return std::string(image_dir) + std::string(img_ball_filename); }
    static std::string img_ballstorm_path() { return std::string(image_dir) + std::string(img_ballstorm_filename); }
    static std::string img_paddle_path() { return std::string(image_dir) + std::string(img_paddle_filename); }
    static std::string img_brick_path() { return std::string(image_dir) + std::string(img_brick_filename); }
    static std::string img_plasma_ball_path() { return std::string(image_dir) + std::string(img_plasma_ball_filename); }
    static std::string img_life_path() { return std::string(image_dir) + std::string(img_life_filename); }
    static std::string img_powerup_path() { return std::string(image_dir) + std::string(img_powerup_filename); }
    static std::string sfx_ball_brick_path() { return std::string(sfx_dir) + std::string(sfx_ball_brick_filename); }
    static std::string sfx_ball_paddle_path() { return std::string(sfx_dir) + std::string(sfx_ball_paddle_filename); }
    static std::string sfx_ball_wall_path() { return std::string(sfx_dir) + std::string(sfx_ball_wall_filename); }
    static std::string sfx_ballstorm_path() { return std::string(sfx_dir) + std::string(sfx_ballstorm_filename); }
    static std::string sfx_game_over_path() { return std::string(sfx_dir) + std::string(sfx_game_over_filename); }
    static std::string sfx_life_minus_path() { return std::string(sfx_dir) + std::string(sfx_life_minus_filename); }
    static std::string sfx_player_wins_path() { return std::string(sfx_dir) + std::string(sfx_player_wins_filename); }
    static std::string sfx_powerdown_path() { return std::string(sfx_dir) + std::string(sfx_powerdown_filename); }
    static std::string sfx_powerup_path() { return std::string(sfx_dir) + std::string(sfx_powerup_filename); }
    static std::string sfx_welcome_path() { return std::string(sfx_dir) + std::string(sfx_welcome_filename); }
    static constexpr std::string_view string_instructions{
        "      ARKANOID: RECLAIMING THE SOLAR SYSTEM        "
        "\n\n"
        "                    YEAR 3056.                     "
        "\n"
        "           AI-CREATED COSMIC STRUCTURES            "
        "\n"
        "           HAVE INVADED THE SOLAR SYSTEM.          "
        "\n"
        "          PLANET AFTER PLANET HAS FALLEN.          "
        "\n"
        "          THE SUN ITSELF IS UNDER THREAT.          "
        "\n\n"
        "  YOU ARE A HERO FROM THE ALPHA CENTAURI SYSTEM,   "
        "\n"
        "           SENT TO SAVE YOUR NEIGHBORS.            "
        "\n\n"
        "          YOU CONTROL THE LAST DEFENSE:            "
        "\n"
        "            A PADDLE AND ENERGY BALLS.             "
        "\n\n"
        "                BREAK THE BRICKS.                  "
        "\n"
        "               RECLAIM THE PLANETS.                "
        "\n"
        "                  SAVE THE SUN.                    "
        "\n\n"
        "                  INSTRUCTIONS:                    "
        "\n\n"
        "- LEFT ARROW / MOVE MOUSE LEFT: MOVE PADDLE LEFT   "
        "\n"
        "- RIGHT ARROW / MOVE MOUSE RIGHT: MOVE PADDLE RIGHT"
        "\n"
        "- P: PAUSE / RESUME                                "
        "\n"
        "- R: RESET                                         "
        "\n"
        "- CATCH BLUE AND ORANGE BALLS TO GET A POWER-UP    "
        "\n"
        "- CATCH THE GREEN BALL TO GAIN AN EXTRA LIFE       "
        "\n\n"
        "            PRESS ANY KEY TO CONTINUE.             "
    };
    static constexpr std::string_view string_paused{ "PAUSED" };
    static constexpr std::string_view string_game_over{
        "  GAME OVER!"
        "\n\n"
        "  YOUR MISSION TO RECLAIM THE SOLAR SYSTEM FAILED."
        "\n"
        "  THE SUN IS STILL UNDER THREAT."
        "\n\n"
        "  - PRESS ANY KEY TO TRY AGAIN AND SAVE THE SUN"
        "\n"
        "  - PRESS ESCAPE TO QUIT"
    };
    static constexpr std::string_view string_player_wins{
        "          SUCESSFULL MISSION!           "
        "\n\n"
        "  YOU HAVE DEFEATED THE COSMIC BRICKS.  "
        "\n"
        "    ALL PLANETS HAVE BEEN RECLAIMED,    "
        "\n"
        "       AND THE SUN IS SAFE AGAIN.       "
        "\n\n"
        "      THE SOLAR SYSTEM THANKS YOU,      "
        "\n"
        "       HERO FROM ALPHA CENTAURI!        "
        "\n\n"
        "      PRESS ANY KEY TO PLAY AGAIN       "
        "\n"
        "         PRESS ESCAPE TO QUIT           "
    };
};

#endif // CONSTANTS_H
#pragma once