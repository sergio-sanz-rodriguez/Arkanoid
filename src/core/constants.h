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
    
    static constexpr int window_width{ 628 }; // 638 }; //48 - 498
    static constexpr int window_height{ 860 }; // { 960 };
    static constexpr float ball_speed{ 8.0f };
    static constexpr float ball_max_speed{ 16.0f };
    static constexpr float ball_min_speed{ 6.0f };
    static constexpr float ball_speed_step{ 0.5f };
    static constexpr float ball_radius{ 24.0f };
    static constexpr float ballstorm_speed{ 10.0f };
    static constexpr float ballstorm_interval{ 0.2f }; // fire every 200 ms
    static constexpr float ballstorm_duration_in_sec{ 10.0f }; // lasts 10 secs
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

    static constexpr int player_lives{ 3 };
    static constexpr int max_sounds{ 16 };
    static constexpr sf::Vector2f ball_scale{ 0.5f, 0.5f };
    static constexpr sf::Vector2f ballstorm_scale{ 0.5f, 0.5f };
    static constexpr sf::Vector2f brick_scale{ 1.0f, 1.0f };
    static constexpr sf::Vector2f bonus_scale{ 1.0f, 1.0f };
    static constexpr sf::Vector2f paddle_scale{ 0.411f, 0.25f };
};

#endif // CONSTANTS_H
#pragma once