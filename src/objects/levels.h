#ifndef LEVELS_H
#define LEVELS_H

#include "constants.h"
#include <vector>
#include <cstdint>

// Cell struct specifying brick properties
struct level_cell {
    uint8_t strength = 0;     // 0 = empty
    uint8_t color_idx = 0;    // index into vcolor
};

struct level_data {
    int columns = 0;
    int rows = 0;
    std::vector<level_cell> grid; // size = columns * rows

    const level_cell& at(int x, int y) const {
        return grid[y * columns + x];
    }
};

const level_data& get_level(int index);
int level_count();


#endif // LEVELS_H
#pragma once