#ifndef LEVELS_H
#define LEVELS_H

#include <cstdint>
#include <vector>
#include <string>
#include "constants.h"
#include "paddle_config.h"
#include "ball_config.h"
#include "brick_config.h"

const std::vector<sf::Color>& get_color_vector(brick_colors map);

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
    std::string background_path;
    std::string menu_label;
    ball_colors ball_color = ball_colors::steel;
    paddle_colors paddle_color = paddle_colors::light_gray;
    brick_colors brick_color_map = brick_colors::arcade;

    const level_cell& at(int x, int y) const {
        return grid[y * columns + x];
    }

};

const level_data& get_level(int index);
int level_count();

#endif // LEVELS_H
#pragma once