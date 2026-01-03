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
    static constexpr int window_width{ 638 };
    static constexpr int window_height{ 860 }; // { 960 };
    static constexpr float ball_speed{ 8.0f };
    static constexpr float ball_max_speed{ 16.0f };
    static constexpr float ball_min_speed{ 6.0f };
    static constexpr float ball_speed_step{ 0.5f };
    static constexpr float burst_speed{ 10.0f };
    static constexpr float burst_interval{ 0.25f }; // fire every 0.25 sec
    static constexpr float burst_duration_in_sec{ 5.0f }; // lasts 5 secs
    static constexpr float paddle_width{ 75.0f };
    static constexpr float paddle_height{ 40.0f };
    static constexpr float paddle_speed{ 10.0f };
    static constexpr float paddle_max_speed{ 20.0f }; // { 21.33f };
    static constexpr float paddle_min_speed{ 6.0f };
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
    static constexpr int brick_strength{ 1 }; // The brick needs to be hit X times before it disappears 
    static constexpr int brick_columns{ 12 };
    static constexpr int brick_rows{ 12 };
    static constexpr int brick_alpha_hit3{ 80 };  // Dull
    static constexpr int brick_alpha_hit2{ 170 }; // Medium
    static constexpr int brick_alpha_hit1{ 255 }; // Bright
    static constexpr int max_active_bonuses{ 2 };
    static constexpr float bonus_speed{ 6.0f };
    static constexpr float bonus_max_speed{ 16.0f };
    static constexpr float bonus_min_speed{ 6.0f };
    static constexpr float bonus_speed_jitter{ 0.9f }; // a jitter better 0.9 * speed and (1/0.9) * speed.
    static constexpr float bonus_speed_step{ 0.5f };
    static constexpr float bonus_scale{ 1.0f };
    static constexpr float powerup_prob{ 0.7f };
    static constexpr float brick_width{ 43.0f };
    static constexpr float brick_height{ 20.0f };
    static constexpr float brick_offset{ brick_width * 0.9f };
    static constexpr int player_lives{ 3 };
    static constexpr int max_sounds{ 16 };
    static constexpr sf::Color white{ 255, 255, 255, 255 }; // White
    static constexpr sf::Color red{ 205, 92, 92, 255 };     // Light Coral
    static constexpr sf::Color orange{ 255, 129, 24, 255 }; // Dark Orange
    static constexpr sf::Color green{ 60, 179, 113, 255 };  // Medium Sea Green
    static constexpr sf::Color cyan{ 0, 206, 209, 255 };    // Dark Turquoise
    static constexpr sf::Color blue{ 70, 130, 180, 255 };   // Calming Blue
    static constexpr sf::Color steel{ 156, 208, 255, 255 }; // Steel Blue
    static constexpr sf::Color true_red{ 250, 0, 0, 255 };   // True Red
    static constexpr sf::Color true_green{ 0, 255, 0, 255 }; // True Green
    static constexpr sf::Color true_blue{ 0, 0, 255, 255 }; // True Blue
    static constexpr std::string_view image_dir{ "..\\..\\assets\\images\\" };
    static constexpr std::string_view sfx_dir{ "..\\..\\assets\\sfx\\" };
    static constexpr std::string_view img_background_filename{ "background_stars.jpg" };
    static constexpr std::string_view img_ball_filename{ "ball_llwhite.png" };
    static constexpr std::string_view img_burst_filename{ "burst.png" };
    static constexpr std::string_view img_paddle_filename{ "paddle_gray.png" };
    static constexpr std::string_view img_brick_filename{ "brick_s.png" };
    static constexpr std::string_view img_fireball_filename{ "ball_orange.png" };
    static constexpr std::string_view img_life_filename{ "ball_green.png" };
    static constexpr std::string_view img_powerup_filename{ "ball_blue.png" };
    static constexpr std::string_view sfx_ball_brick_filename{ "ball_brick.wav" };
    static constexpr std::string_view sfx_ball_paddle_filename{ "ball_paddle.wav" };
    static constexpr std::string_view sfx_ball_wall_filename{ "ball_wall.wav" };
    static constexpr std::string_view sfx_ball_burst_filename{ "ball_burst.wav" };
    static constexpr std::string_view sfx_game_over_filename{ "game_over.wav" };
    static constexpr std::string_view sfx_life_minus_filename{ "life_minus.wav" };
    static constexpr std::string_view sfx_player_wins_filename{ "player_wins.wav" };
    static constexpr std::string_view sfx_powerdown_filename{ "powerdown.wav" };
    static constexpr std::string_view sfx_powerup_filename{ "powerup.wav" };
    static constexpr std::string_view sfx_welcome_filename{ "welcome.wav" };
    //static constexpr std::string_view font_filename{ "/usr/local/share/fonts/webfonts/verdana.ttf" };
    static constexpr std::string_view font_verdana{ "..\\..\\assets\\fonts\\verdana.ttf" };
    static constexpr std::string_view font_consola{ "..\\..\\assets\\fonts\\consola.ttf" };
    static std::string img_background_path() { return std::string(image_dir) + std::string(img_background_filename); }
    static std::string img_ball_path() { return std::string(image_dir) + std::string(img_ball_filename); }
    static std::string img_burst_path() { return std::string(image_dir) + std::string(img_burst_filename); }
    static std::string img_paddle_path() { return std::string(image_dir) + std::string(img_paddle_filename); }
    static std::string img_brick_path() { return std::string(image_dir) + std::string(img_brick_filename); }
    static std::string img_fireball_path() { return std::string(image_dir) + std::string(img_fireball_filename); }
    static std::string img_life_path() { return std::string(image_dir) + std::string(img_life_filename); }
    static std::string img_powerup_path() { return std::string(image_dir) + std::string(img_powerup_filename); }
    static std::string sfx_ball_brick_path() { return std::string(sfx_dir) + std::string(sfx_ball_brick_filename); }
    static std::string sfx_ball_paddle_path() { return std::string(sfx_dir) + std::string(sfx_ball_paddle_filename); }
    static std::string sfx_ball_wall_path() { return std::string(sfx_dir) + std::string(sfx_ball_wall_filename); }
    static std::string sfx_ball_burst_path() { return std::string(sfx_dir) + std::string(sfx_ball_burst_filename); }
    static std::string sfx_game_over_path() { return std::string(sfx_dir) + std::string(sfx_game_over_filename); }
    static std::string sfx_life_minus_path() { return std::string(sfx_dir) + std::string(sfx_life_minus_filename); }
    static std::string sfx_player_wins_path() { return std::string(sfx_dir) + std::string(sfx_player_wins_filename); }
    static std::string sfx_powerdown_path() { return std::string(sfx_dir) + std::string(sfx_powerdown_filename); }
    static std::string sfx_powerup_path() { return std::string(sfx_dir) + std::string(sfx_powerup_filename); }
    static std::string sfx_welcome_path() { return std::string(sfx_dir) + std::string(sfx_welcome_filename); }

};

#endif // CONSTANTS_H
#pragma once