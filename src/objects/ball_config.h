#ifndef BALL_CONFIG_H
#define BALL_CONFIG_H

#include "colors.h"

enum class ball_type {
    regular,
    plasma,
    antimatter
};

enum class ball_colors {
    steel,
    gold,
    orange,
    blueviolet
};

namespace ball_color_maps {

    inline constexpr sf::Color bouncing_ball = colors::steel; // steel;
    inline constexpr sf::Color bouncing_gold_ball = colors::gold_ball; // steel;
    inline constexpr sf::Color plasma_ball = colors::orange;
    inline constexpr sf::Color antimatter_ball = colors::blueviolet;
}

#endif // BALL_CONFIG_H
#pragma once