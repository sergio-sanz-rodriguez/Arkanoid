#ifndef BONUS_CONFIG_H
#define BONUS_CONFIG_H

namespace bonus_config {

    inline constexpr int max_active_bonuses{ 2 };
    inline constexpr float bonus_speed{ 6.0f };
    inline constexpr float bonus_max_speed{ 16.0f };
    inline constexpr float bonus_min_speed{ 6.0f };
    inline constexpr float bonus_speed_jitter{ 0.9f }; // A jitter better 0.9 * speed and (1/0.9) * speed.
    inline constexpr float bonus_speed_step{ 0.5f };
    inline constexpr float powerup_prob{ 0.7f };

}

#endif // BONUS_CONFIG_H
#pragma once