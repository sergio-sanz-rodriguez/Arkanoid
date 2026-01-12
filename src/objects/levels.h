#ifndef LEVELS_H
#define LEVELS_H

#include <cstdint>
#include <vector>
#include "constants.h"

// Possible color maps
enum class color_map_type {
    arcade,
    cosmic,
    planets,
    tierra,
    starfield,
    alien,
    darkmatter,
    rocks,
    grayscale,
    medal
};

const std::vector<sf::Color>& get_color_vector(color_map_type map);

// Cell struct specifying brick properties
struct level_cell {
    uint8_t strength{ 0 };     // 0 = empty, constants::max_brick_strength + 1 = indestructible
    uint8_t color_idx{ 0 };    // index into vcolor
};

struct level_data {
    int columns = 0;
    int rows = 0;
    float width_offset = 4.0f;
    float height_offset = 3.0f;
    std::vector<level_cell> grid; // size = columns * rows
    color_map_type color_map = color_map_type::arcade;
    std::string background_path;
    std::string level_title;
    

    const level_cell& at(int x, int y) const {
        return grid[y * columns + x];
    }
};

const level_data& get_level(int index);
int level_count();


#endif // LEVELS_H
#pragma once