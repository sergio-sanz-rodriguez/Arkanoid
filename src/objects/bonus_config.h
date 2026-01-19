#ifndef BONUS_CONFIG_H
#define BONUS_CONFIG_H

namespace bonus_config {

    // --------------------
    // Bonus motion
    // --------------------
    inline constexpr float bonus_speed{ 6.0f };
    inline constexpr float bonus_min_speed{ 6.0f };
    inline constexpr float bonus_max_speed{ 16.0f };
    inline constexpr float bonus_speed_jitter{ 0.9f };
    inline constexpr float bonus_speed_step{ 0.5f };

    // --------------------
    // Spawn timing
    // --------------------
    // Matches old uniform [5,15] feel
    inline constexpr float min_spawn_time{ 5.0f }; // 4.0f
    inline constexpr float mean_spawn_time{ 10.0f }; //9.0f  // average

    // --------------------
    // Special type mix
    // --------------------
    inline constexpr std::array<std::array<float, 5>, 9> level_probs{ {

            // Probabilites for life, plasma, antimatter are with respect to the special probability
            // powerup, special, life,  plasma, antimatter
            {  0.70f,   0.30f,   0.50f, 0.25f,  0.25f }, // Level 1
            {  0.70f,   0.30f,   0.50f, 0.40f,  0.10f }, // Level 2
            {  0.70f,   0.30f,   0.50f, 0.30f,  0.20f }, // Level 3
            {  0.70f,   0.30f,   0.50f, 0.20f,  0.30f }, // Level 4
            {  0.70f,   0.30f,   0.50f, 0.10f,  0.40f }, // Level 5
            {  0.70f,   0.30f,   0.50f, 0.10f,  0.40f }, // Level 6
            {  0.70f,   0.30f,   0.50f, 0.10f,  0.40f }, // Level 7
            {  0.70f,   0.30f,   0.50f, 0.10f,  0.40f }, // Level 8
            {  0.50f,   0.50f,   0.50f, 0.05f,  0.45f }  // Level 9
        } };

    // --------------------
    // Max active bonuses (optional)
    // --------------------
    inline constexpr int max_active_bonuses{ 2 };
}

#endif // BONUS_CONFIG_H
#pragma once