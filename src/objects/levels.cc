#include "levels.h"
#include <stdexcept>
#include <algorithm>

// Convert a vector<string> of ASCII into a vector<level_cell> grid.
// Rules:
//  '.'  -> empty cell (strength = 0)
//  '1'  -> brick strength 1
//  '2'  -> brick strength 2
//  '3'  -> brick strength 3
//  '#'  -> indestructible brick (strength = constants::indestructible_strength)
//
// Color index strategy (simple, nice-looking):
//  - For '1','2','3': color depends on row (like your old code: y % rows)
//  - For '#': fixed dark gray color index (we’ll store idx=0 here, and you can override later if needed)
static std::vector<level_cell> parse_grid(
    int W,
    int H,
    const std::vector<std::string>& rows
) {
    if ((int)rows.size() != H) {
        throw std::runtime_error("parse_grid(): rows.size() must match H.");
    }

    std::vector<level_cell> g(W * H);

    for (int y = 0; y < H; ++y) {
        if ((int)rows[y].size() != W) {
            throw std::runtime_error("parse_grid(): each row must have exactly W characters.");
        }

        for (int x = 0; x < W; ++x) {
            const char c = rows[y][x];

            level_cell cell{};
            cell.strength = 0;
            cell.color_idx = 0;

            if (c == '.' || c == ' ') {
                // empty
                cell.strength = 0;
            }
            else if (c >= '1' && c <= '3') {
                // destructible strength 1..3
                cell.strength = static_cast<uint8_t>(c - '0');

                // color depends on row (same pattern as your level1)
                cell.color_idx = static_cast<uint8_t>(y % H);
            }
            else if (c == '#') {
                // indestructible
                cell.strength = static_cast<uint8_t>(constants::indestructible_strength);

                // You can choose a special color index, e.g. dark gray.
                // Here we store 0; later you can map '#' to a dedicated gray color in brick creation.
                cell.color_idx = 0;
            }
            else {
                // Unknown character -> treat as empty, OR throw.
                // Throwing is safer so you detect mistakes early.
                throw std::runtime_error(std::string("parse_grid(): invalid char '") + c + "'");
            }

            g[y * W + x] = cell;
        }
    }

    return g;
}

// Ensures the ASCII level is exactly WxH (padding with empty rows if needed).
// Use this when you're prototyping and some levels have fewer rows.
static std::vector<std::string> grid_from_strings(
    int W,
    int H,
    std::initializer_list<const char*> lines
) {
    std::vector<std::string> rows;
    rows.reserve(H);

    for (const char* s : lines) {
        rows.emplace_back(s);
    }

    // If less rows than H, add empty rows at bottom
    while ((int)rows.size() < H) {
        rows.emplace_back(std::string(W, '.'));
    }

    // If too many rows, throw
    if ((int)rows.size() > H) {
        throw std::runtime_error("grid_from_strings(): too many rows provided.");
    }

    // Validate width
    for (auto& r : rows) {
        if ((int)r.size() != W) {
            throw std::runtime_error("grid_from_strings(): a row has wrong width.");
        }
    }

    return rows;
}

// Design the arrangement of level 1 (Neptune)
static level_data level1 = {
    12, // columns
    12, // rows
    4.0f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = 12;
        const int H = 12;
        const auto rows = grid_from_strings(W, H, {
            "111111111111",
            "111111111111",
            "111111111111",
            "111111111111",
            "111111111111",
            "111111111111",
            "111111111111",
            "111111111111",
            "111111111111",
            "111111111111",
            "111111111111",
            "111111111111"
        });
        return parse_grid(W, H, rows);
    }(),
    constants::img_background_level1_path(),
    "\n\n"
    "FIRST MISSION (1/10): NEPTUNE\n\n"
    "   PRESS ANY KEY TO START"
};

// Design the arrangement of level 2 (Uranus)
static level_data level2 = {
    12, // columns
    17, // rows
    4.0f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = 12;
        const int H = 17;
        const auto rows = grid_from_strings(W, H, {
            "111111111111", //  1
            "111111111111", //  2
            "111111111111", //  3
            "111111111111", //  4
            "111111111111", //  5
            "111111111111", //  6
            "111111111111", //  7
            "111111111111", //  8
            "111111111111", //  9
            "111111111111", // 10
            "111111111111", // 11
            "111111111111", // 12
            "............", // 13
            ".....##.....", // 14
            "............", // 15
            "............", // 16
            "##........##"  // 17
        });
        return parse_grid(W, H, rows);
    }(),
    constants::img_background_level2_path(),
    "     CONTRATULATIONS!\n\n"
    "NEXT MISSION (2/10): URANUS\n\n"
    "  PRESS ANY KEY TO START"
};


// Design the arrangement of level 2 (The Titan Mond)
static level_data level3 = {
    12, // columns
    22, // rows
    7.2f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = 12;
        const int H = 22;
        const auto rows = grid_from_strings(W, H, {
            "111111111111", //  1
            "311111111113", //  2
            "321111111123", //  3
            "322111111223", //  4
            "322211112223", //  5
            "322221122223", //  6
            "322221122223", //  7
            "322211112223", //  8
            "322111111223", //  9
            "321111111123", // 10
            "311111111113", // 11
            "111111111111", // 12
            "............", // 13
            "............", // 14
            "............", // 15
            "............", // 16
            "............", // 17
            "............", // 18
            "............", // 19
            ".....##.....", // 20
            "....####....", // 21
            ".....##....."  // 22
        });
        return parse_grid(W, H, rows);
    }(),
    constants::img_background_level3_path(),
    "            CONTRATULATIONS!             "
    "\n\n"
    "NEXT MISSION (3/10): TITAN - SATURN'S MOON"
    "\n\n"
    "         PRESS ANY KEY TO START          "
};


/* Level 4 (Jupiter)
..................
......111111......
.....12222221.....
....1233333321....
...123333333321...
...123333333321...
...123333333321...
....1233333321....
.....12222221.....
......111111......
..................
..................
*/

/* Level 5 (Mars)
..................
...###......###...
...#1111111111#...
...#1222222221#...
...#1233333321#...
...#1233333321#...
...#1233333321#...
...#1222222221#...
...#1111111111#...
...###......###...
..................
..................
*/

/* Level 6 (Earth)
..................
...11111##11111...
...11111##11111...
...11111##11111...
...############...
...############...
...############...
...11111##11111...
...11111##11111...
...11111##11111...
..................
..................
*/


/* Level 7 (Venus)
..................
...############...
...#111....111#...
...#111.##.111#...
....333.##.222....
........##........
....333.##.222....
...#111.##.111#...
...#111....111#...
...############...
..................
..................
*/

/* Level 8 (Mercury)
..................
...111111..111111...
...122221..122221...
...123321..123321...
...123321..123321...
...123321..123321...
...123321..123321...
...123321..123321...
...122221..122221...
...111111..111111...
..................
..................
*/

/* Level 9 (Sun)
..................
...333333333333...
...322222222223...
...321111111123...
...321111111123...
...#####....#####...
...#####....#####...
...321111111123...
...321111111123...
...322222222223...
...333333333333...
..................
*/

/* Level 10 (A*)
..................
...333333333333...
...322222222223...
...321111111123...
...321##..##123...
...321##..##123...
...321######123...
...321######123...
...321111111123...
...322222222223...
...333333333333...
..................
*/

// Stack the difficulty levels
static const level_data* all_levels[] = {
    &level1,
    //&level2,
    //&level3
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