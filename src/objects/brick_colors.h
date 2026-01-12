#ifndef BRICK_COLORS_H
#define BRICK_COLORS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "colors.h"

namespace brick_colors {

    inline const std::vector<sf::Color> arcade = {
        colors::red,
        colors::orange,
        colors::gold,
        colors::yellow,
        colors::green,
        colors::cyan,
        colors::steel,
        colors::blue,
        colors::purple,
        colors::pink
    };

    inline const std::vector<sf::Color> cosmic = {
        sf::Color(255,  51, 153), // pink nebula
        sf::Color(102, 204, 255), // cyan gas
        sf::Color(204, 102, 255), // violet dust
        sf::Color(51, 255, 204),  // teal haze
        sf::Color(255, 204, 102), // orange glow
        sf::Color(255, 255, 102)  // yellow star glow
    };

    inline const std::vector<sf::Color> planets = {
        sf::Color(204, 102, 0),   // Mars red-brown
        sf::Color(255, 204, 153), // Venus pale orange
        sf::Color(102, 153, 255), // Neptune blue
        sf::Color(255, 255, 204), // Mercury pale yellow
        sf::Color(153, 102, 51),  // Jupiter brown
        sf::Color(192, 192, 192), // Moon / grey rocks
    };

    inline const std::vector<sf::Color> tierra = {
        colors::blue,   // Water
        colors::brown,  // Rocks
        colors::orange, // Core
    };

    inline const std::vector<sf::Color> starfield = {
        sf::Color(255, 255, 255), // white stars
        sf::Color(255, 255, 0),   // yellow star
        sf::Color(255, 128, 0),   // orange flare
        sf::Color(0, 255, 255),   // cyan nova
        sf::Color(255, 0, 255),   // purple supernova
    };

    inline const std::vector<sf::Color> alien = {
        sf::Color(0, 255, 128),   // neon green
        sf::Color(255, 0, 128),   // pinkish alien
        sf::Color(128, 0, 255),   // violet
        sf::Color(0, 128, 255),   // cyan tech
        sf::Color(255, 255, 0),   // yellow highlight
    };

    inline const std::vector<sf::Color> darkmatter = {
        sf::Color(25, 25, 25),    // near-black
        sf::Color(64, 0, 128),    // dark purple
        sf::Color(0, 64, 128),    // deep blue
        sf::Color(255, 128, 0),   // orange highlight
        sf::Color(255, 255, 255)  // white sparks
    };

    inline const std::vector<sf::Color> rocks = {
        sf::Color(169, 169, 169), // Mercury gray
        sf::Color(205, 133, 63),  // Venus sandy/brown
        sf::Color(34, 139, 34),   // Earth green
        sf::Color(139, 69, 19),   // Mars dark brown
        sf::Color(222, 184, 135), // Light tan / desert
        sf::Color(128, 128, 128), // Medium gray
        sf::Color(178, 34, 34),   // Mars red
        sf::Color(107, 142, 35),  // Olive green / vegetation hint
        sf::Color(210, 180, 140), // Sandy beige
        sf::Color(160, 82, 45)    // Rusty brown
    };

    inline const std::vector<sf::Color> grayscale = {
        sf::Color(25, 25, 25),   // near black
        sf::Color(64, 64, 64),   // dark gray
        sf::Color(128, 128, 128),// medium gray
        sf::Color(192, 192, 192),// light gray
        sf::Color(220, 220, 220),// very light gray
        sf::Color(255, 255, 255) // white
    };

    inline const std::vector<sf::Color> medal = {
        colors::bronze,
        colors::silver,
        colors::gold
    };

    inline constexpr sf::Color indestructible = colors::anthracite;

}

#endif // BRICK_COLORS_H
#pragma once