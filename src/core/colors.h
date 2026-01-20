#ifndef COLORS_H
#define COLORS_H

#pragma once
#include <SFML/Graphics.hpp>

namespace colors {

    inline constexpr sf::Color anthracite{ 53, 60, 64 };
    inline constexpr sf::Color black{ 0, 0, 0 };
    inline constexpr sf::Color dark_gray{ 64, 64, 64 };
    inline constexpr sf::Color white{ 255, 255, 255 };
    inline constexpr sf::Color red{ 205, 92, 92 };
    inline constexpr sf::Color orange{ 255, 129, 24 };
    inline constexpr sf::Color green{ 60, 179, 113 };
    inline constexpr sf::Color cyan{ 0, 206, 209 };
    inline constexpr sf::Color blue{ 70, 130, 180 };
    inline constexpr sf::Color steel{ 156, 208, 255 };
    inline constexpr sf::Color yellow{ 240, 220, 70 }; // Warm Yellow
    inline constexpr sf::Color purple{ 155, 89, 182 }; // Amethyst
    inline constexpr sf::Color blueviolet{ 138, 43, 226 }; // Blue Violet
    inline constexpr sf::Color pink{ 255, 105, 180 }; // Hot Pink
    inline constexpr sf::Color brown{ 139, 90, 43 }; // Asteroid Brown
    inline constexpr sf::Color copper{ 184, 115, 51 }; // Copper Strong bricks
    inline constexpr sf::Color gold{ 212, 175, 55 };   // Gold Strong bricks
    inline constexpr sf::Color gold_ball{ 216, 172, 99 };   // Gold for ball
    inline constexpr sf::Color silver{ 187, 198, 204 };
    inline constexpr sf::Color bronze{ 176, 116, 59 };
    inline constexpr sf::Color steel_blue_tint(180, 180, 200);

    // True color
    inline constexpr sf::Color true_red{ 250, 0, 0 };
    inline constexpr sf::Color true_green{ 0, 255, 0 };
    inline constexpr sf::Color true_blue{ 0, 0, 255 };

    // Gold (paddle)
    inline constexpr sf::Color gold_base{ 212, 175, 55 };
    inline constexpr sf::Color gold_shiny{ 255, 215, 0 };
    inline constexpr sf::Color gold_shadow{ 184, 134, 11 };

}

#endif // COLORS_H
#pragma once