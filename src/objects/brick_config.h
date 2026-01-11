#ifndef BRICK_CONFIG_H
#define BRICK_CONFIG_H

namespace brick_config {

    inline constexpr int max_brick_strength{ 3 }; // The brick needs to be hit up to 3 times before it disappears
    inline constexpr int indestructible_strength{ 255 };
    inline constexpr int brick_columns{ 15 };
    inline constexpr int brick_rows{ 45 };
    inline constexpr float brick_width{ 43.0f };
    inline constexpr float brick_height{ 20.0f };
    inline constexpr float brick_offset_width{ brick_width * 0.5f };
    inline constexpr float brick_offset_height{ brick_height * 0.5f };
    inline constexpr int brick_alpha_hit1{ 255 }; // Bright
    inline constexpr int brick_alpha_hit2{ 170 }; // Medium
    inline constexpr int brick_alpha_hit3{  80 };  // Dull

}

#endif // BRICK_CONFIG_H
#pragma once