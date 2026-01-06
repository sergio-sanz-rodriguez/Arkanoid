#include "levels.h"

// Design the arragement of level 1
static level_data level1 = {
    constants::brick_columns, // columns
    constants::brick_rows, // rows
    [] {
        std::vector<level_cell> g(constants::brick_columns * constants::brick_rows);
        for (int y = 0; y < constants::brick_rows; ++y) {
            for (int x = 0; x < constants::brick_columns; ++x) {
                g[y * constants::brick_columns + x] = level_cell{ 1, uint8_t(y % constants::brick_rows) };
            }
        }
        return g;
    }(),
        constants::img_background_level1_path(),
        "  MISSION 1: NEPTUNE\n\n"
        "PRESS ANY KEY TO START"
};

// Stack the difficulty levels
static const level_data* all_levels[] = {
    &level1, // Pointer
    // add up to 10
};

// Helper function to get the level
const level_data& get_level(int index) {
    index = std::clamp(index, 0, (int)(std::size(all_levels)) - 1);
    return *all_levels[index];
}

// Return the number of levels
int level_count() {
    return (int)std::size(all_levels);
}