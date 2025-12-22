#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <string_view>
#include <SFML/Graphics.hpp>

// Code for a "breakout" game
// Based on a talk by Vittorio Romeo
// Uses the SFML graphics library

struct constants {
    static constexpr int window_width{ 638 };
    static constexpr int window_height{ 860 }; // { 960 };
    static constexpr float ball_speed{ 6.0f };
    static constexpr float ball_max_speed{ 16.0f };
    static constexpr float ball_min_speed{ 6.0f };
    static constexpr float ball_speed_step{ 0.5f };
    static constexpr float paddle_width{ 75.0f };
    //static constexpr float paddle_width_by_2{ 37.5f };
    static constexpr float paddle_height{ 40.0f };
    static constexpr float paddle_speed{ 8.0f };
    static constexpr float paddle_max_speed{ 21.33f };
    static constexpr float paddle_min_speed{ 8.0f };
    static constexpr float paddle_speed_step{
        ball_speed_step * (paddle_max_speed - paddle_min_speed) / (ball_max_speed - ball_min_speed) 
    };
    static constexpr float paddle_scale_width{ 0.411f };
    static constexpr float paddle_scale_height{ 0.25f };
    static constexpr float rotation_angle{ 1.0f };
    static constexpr float pi{ 3.14159265358979323846f };
    static constexpr int brick_strength{ 1 }; // The brick needs to be hit 3 times before it disappears 
    static constexpr int brick_columns{ 12 };
    static constexpr int brick_rows{ 12 };
    static constexpr int brick_alpha_hit3{ 80 };  // Dull
    static constexpr int brick_alpha_hit2{ 170 }; // Medium
    static constexpr int brick_alpha_hit1{ 255 }; // Bright
    static constexpr float live_speed{ 6.0f };
    static constexpr float live_max_speed{ 16.0f };
    static constexpr float live_min_speed{ 6.0f };
    static constexpr float live_speed_step{ 0.5f };
    static constexpr float live_scale{ 1.0f };
    static constexpr sf::Color white{ 255, 255, 255, 255 }; // White
    static constexpr sf::Color red{ 205, 92, 92, 255 };     // Light Coral
    static constexpr sf::Color orange{ 255, 129, 24, 255 }; // Dark Orange
    static constexpr sf::Color green{ 60, 179, 113, 255 };  // Medium Sea Green
    static constexpr sf::Color cyan{ 0, 206, 209, 255 };    // Dark Turquoise
    static constexpr sf::Color blue{ 70, 130, 180, 255 };   // Calming Blue
    static constexpr sf::Color steel{ 156, 208, 255, 255 }; // Steel Blue
    static constexpr float brick_width{ 43.0f };
    static constexpr float brick_height{ 20.0f };
    static constexpr float brick_offset{ brick_width * 0.8f };
    static constexpr int player_lives{ 3 };
    static constexpr std::string_view image_dir{ "..\\..\\assets\\images\\" };
    static constexpr std::string_view background_filename{ "background_stars.jpg" };
    static constexpr std::string_view ball_filename{ "ball_llwhite.png" };
    static constexpr std::string_view paddle_filename{ "paddle_dark_orange.png" };
    static constexpr std::string_view brick_filename{ "brick_s.png" };
    static constexpr std::string_view live_filename{ "extra_live_s.png" };
    //static constexpr std::string_view font_filename{ "/usr/local/share/fonts/webfonts/verdana.ttf" };
    static constexpr std::string_view font_verdana{ "C:\\Windows\\fonts\\verdana.ttf" };
    static std::string background_path() { return std::string(image_dir) + std::string(background_filename); }
    static std::string ball_path() { return std::string(image_dir) + std::string(ball_filename); }
    static std::string paddle_path() { return std::string(image_dir) + std::string(paddle_filename); }
    static std::string brick_path() { return std::string(image_dir) + std::string(brick_filename); }
    static std::string live_path() { return std::string(image_dir) + std::string(live_filename); }
};

#endif // CONSTANTS_H
