#include <stdexcept>
#include <algorithm>
#include <random>
#include "levels.h"
#include "assets.h"
#include "brick_colors.h"
#include "brick_config.h"

// Return the color vector corresponding to a given color map
const std::vector<sf::Color>& get_color_vector(color_map_type map) {
    switch (map) {
    case color_map_type::arcade: return brick_colors::arcade;
    case color_map_type::cosmic: return brick_colors::cosmic;
    case color_map_type::planets: return brick_colors::planets;
    case color_map_type::starfield: return brick_colors::starfield;
    case color_map_type::alien: return brick_colors::alien;
    case color_map_type::darkmatter: return brick_colors::darkmatter;
    case color_map_type::grayscale: return brick_colors::grayscale;
    case color_map_type::medal: return brick_colors::medal;
    }
    return brick_colors::arcade; // fallback
}

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
    const std::vector<std::string>& rows_strength,
    const std::vector<std::string>& rows_color,
    color_map_type color_map,
    bool apply_random_idx
) {
    // Validate grid height
    if ((int)rows_strength.size() != H) {
        throw std::runtime_error("parse_grid(): rows.size() must match H.");
    }

    // Allocate the grid cells
    std::vector<level_cell> g(W * H);

    // Get number of colors in the selected color map
    size_t max_c = get_color_vector(color_map).size();

    // Random color shift for the whole level
    size_t X = 0;
    if (apply_random_idx) {
        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<size_t> dist(0, max_c - 1);
        X = dist(rng);
    }

    // Iterate over rows
    for (int y = 0; y < H; ++y) {

        // Validate row width
        if ((int)rows_strength[y].size() != W) {
            throw std::runtime_error("parse_grid(): each row must have exactly W characters.");
        }

        // Iterate over columns
        for (int x = 0; x < W; ++x) {
            const char s = rows_strength[y][x]; // Strength of the brick
            const char c = rows_color[y][x];  // Color of the brick

            // Initialize cell
            level_cell cell{};
            cell.strength = 0;
            cell.color_idx = 0;

            // Empty cell?
            if (s == '.' || s == ' ') {
                cell.strength = 0;
            }

            // Destructible brick (strength 1..3)
            else if (s >= '1' && s <= '3') {
                // Destructible strength 1..3
                cell.strength = static_cast<uint8_t>(s - '0');

                // Apply color index with random offset
                size_t base = static_cast<size_t>(c - '0');
                cell.color_idx = static_cast<uint8_t>((base + X) % max_c);
            }

            // Indestructible brick
            else if (s == '#') {
                cell.strength = static_cast<uint8_t>(brick_config::indestructible_strength);

                // You can choose a special color index, e.g. dark gray.
                // Here we store 0; later you can map '#' to a dedicated gray color in brick creation.
                cell.color_idx = 0;
            }

            // Invalid character
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

// Generate random color rows based on brick positions
static std::vector<std::string> random_rows_color(
    int W,
    int H,
    const std::vector<std::string>& rows_strength,
    color_map_type color_map
) {

    // Random number generator (initialized once)
    static std::mt19937 rng{ std::random_device{}() };

    // Number of available colors in the selected color map
    size_t max_c = get_color_vector(color_map).size();
    
    // Distribution to select a random color index
    std::uniform_int_distribution<int> dist(0, static_cast<int>(max_c));

    // Container for generated color rows
    std::vector<std::string> rows;
    rows.reserve(H);

    // Iterate over rows
    for (int y = 0; y < H; ++y) {

        // Initialize row with empty cells
        std::string row(W, '.');

        // Iterate over columns
        for (int x = 0; x < W; ++x) {
            char s = rows_strength[y][x];

            // Assign random color index only to destructible bricks
            if (s >= '1' && s <= '3') {
                row[x] = static_cast<char>('0' + dist(rng));
            }
            // empty '.' and '#' stay '.'
        }

        // Store the generated row
        rows.emplace_back(std::move(row));
    }

    // Return the generated color grid
    return rows;
}

// Ensure ASCII input matches WxH, padding missing rows/columns
static std::vector<std::string> grid_from_strings(
    int W,
    int H,
    std::initializer_list<const char*> lines
) {

    // Preallocate space for exactly H rows to avoid reallocations
    std::vector<std::string> rows;
    rows.reserve(H);

    // Copy input rows, clamping to the requested height
    std::size_t row = 0;
    for (const char* s : lines) {
        // Too many input rows is considered a logic error
        if (row++ >= static_cast<std::size_t>(H)) {
            throw std::runtime_error(
                "grid_from_strings(): too many rows provided."
            );
        }

        // Copy at most W characters from the input string
        std::size_t cols = std::min<std::size_t>(std::strlen(s), W);
        std::string r(s, cols);

        // Pad short rows so every row is exactly W characters wide
        if (r.size() < static_cast<std::size_t>(W)) {
            r.append(W - r.size(), '.');
        }

        rows.emplace_back(std::move(r));
    }

    // Pad missing rows at the bottom so the grid has exactly H rows
    while (rows.size() < static_cast<std::size_t>(H)) {
        rows.emplace_back(W, '.');
    }

    return rows;
}

// Design the arrangement of level 1 (Neptune)
static level_data level1 = {
    brick_config::brick_columns, // columns
    brick_config::brick_rows, // rows
    4.0f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = brick_config::brick_columns;
        const int H = brick_config::brick_rows;

        // Define brick strengths
        const auto rows_strength = grid_from_strings(W, H, {
            "...............",
            "...............",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
        });

        // Define brick colors
        const auto rows_color = grid_from_strings(W, H, {
            "...............",
            "...............",
            "..00000000000..",
            "..11111111111..",
            "..55555555555..",
            "..22222222222..",
            "..88888888888..",
            "..44444444444..",
            "..66666666666..",
            "..99999999999..",
            "..77777777777..",
            "..33333333333..",
            "..00000000000..",
            "..55555555555..",
            "..44444444444..",
            "..11111111111..",
            "..88888888888..",
            "..77777777777..",
        });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, color_map_type::arcade, true);
    }(),
    color_map_type::arcade, // Color map type
    assets::img_background_level1_path(), // Background asset
    "\n\n"
    "FIRST MISSION (1/10): NEPTUNE\n\n"
    "    PRESS SPACE TO START     " // Intro text
};


// Design the arrangement of level 2 (Uranus)
static level_data level2 = {
    brick_config::brick_columns, // columns
    brick_config::brick_rows, // rows
    3.7f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = brick_config::brick_columns;
        const int H = brick_config::brick_rows;

        // Define brick strengths
        const auto rows_strength = grid_from_strings(W, H, {
            "...............",
            "...............",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "..11111111111..",
            "...............",
            "...............",
            "...............",
            "...............",
            "##...........##",
        });

        // Define brick colors  (random)
        const auto rows_color = random_rows_color(W, H, rows_strength, color_map_type::arcade);

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, color_map_type::arcade, false);
    }(),
    color_map_type::arcade, // Color map type
    assets::img_background_level2_path(), // Background asset
    "     CONTRATULATIONS!      "
    "\n\n"
    "NEXT MISSION (2/10): URANUS"
    "\n\n"
    "   PRESS SPACE TO START    " // Intro text
};

// Design the arrangement of level 3 (Titan)
static level_data level3 = {
    brick_config::brick_columns, // columns
    brick_config::brick_rows, // rows
    7.2f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = brick_config::brick_columns;
        const int H = brick_config::brick_rows;

        // Define brick strengths
        const auto rows_strength = grid_from_strings(W, H, {
            "...............",
            "...............",
            ".1111111111111.",
            ".3111111111113.",
            ".3211111111123.",
            ".3221111111223.",
            ".3222111112223.",
            ".3222211122223.",
            ".3222221222223.",
            ".3222222222223.",
            ".3222221222223.",
            ".3222211122223.",
            ".3222111112223.",
            ".3221111111223.",
            ".3211111111123.",
            ".3111111111113.",
            ".1111113111111.",
            "......333......",
            ".......3.......",
            "...............",
            "...##..........",
            "...............",
            ".........##....",
            "...............",
        });

        // Define brick colors
        const auto rows_color = grid_from_strings(W, H, {
            "...............",
            "...............",
            ".0000000000000.",
            ".2000000000002.",
            ".2100000000012.",
            ".2110000000112.",
            ".2111000001112.",
            ".2111100011112.",
            ".2111110111112.",
            ".2111111111112.",
            ".2111110111112.",
            ".2111100011112.",
            ".2111000001112.",
            ".2110000000112.",
            ".2100000000012.",
            ".2000000000002.",
            ".0000002000000.",
            "......222......",
            ".......2.......",
            "...............",
            "...##..........",
            "...............",
            ".........##....",
            "...............",
            });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, color_map_type::medal, true);
    }(),
    color_map_type::medal, // Color map type
    assets::img_background_level3_path(), //Background asset
    "            CONTRATULATIONS!             "
    "\n\n"
    "NEXT MISSION (3/10): TITAN - SATURN'S MOON"
    "\n\n"
    "          PRESS SPACE TO START           "// Intro text
};


// Design the arrangement of level 4 (Europa)
static level_data level4 = {
    brick_config::brick_columns, // columns
    brick_config::brick_rows, // rows
    8.0f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = brick_config::brick_columns;
        const int H = brick_config::brick_rows;

        // Define brick strengths
        const auto rows_strength = grid_from_strings(W, H, {
            "...............",
            "...............",
            ".......1.......",
            "...#..111..#...",
            ".....12221.....",
            "....1233321....",
            "...123333321...",
            "..12333333321..",
            ".1233333333321.",
            "123232323232321",
            ".1233333333321.",
            "..12333333321..",
            "...123333321...",
            ".#..1233321..#.",
            ".....12221.....",
            "......111......",
            "...............",
            "...............",
            "...............",
            "...............",
            "# # # # # # # #",
            "...............",
            "...............",
            "...............",
            "......111......",
            "......131......",
            "......111......",
        });

        // Define brick colors
        const auto rows_color = grid_from_strings(W, H, {
            "...............",
            "...............",
            ".......0.......",
            "...#..000..#...",
            ".....01110.....",
            "....0122210....",
            "...012222210...",
            "..01222222210..",
            ".0122222222210.",
            "012121212121210",
            ".0122222222210.",
            "..01222222210..",
            "...012222210...",
            ".#..0122210..#.",
            ".....01110.....",
            "......000......",
            ".......0.......",
            "...............",
            "...............",
            "...............",
            "# # # # # # #.#",
            "...............",
            "...............",
            "...............",
            "......000......",
            "......020......",
            "......000......",
            });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, color_map_type::cosmic, true);
    }(),
    color_map_type::cosmic, // Color map type
    assets::img_background_level4_path(), // Background asset
    "              CONTRATULATIONS!              "
    "\n\n"
    "NEXT MISSION (4/10): EUROPA - JUPITER'S MOON"
    "\n\n"
    "           PRESS ANY KEY TO START           " // Intro text
};


// Design the arrangement of level 2 (Uranus)
static level_data level5 = {
    brick_config::brick_columns, // columns
    brick_config::brick_rows, // rows
    3.5f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = brick_config::brick_columns;
        const int H = brick_config::brick_rows;

        // Define brick strengths
        const auto rows_strength = grid_from_strings(W, H, {
            "...............",
            ".2222222222222.",
            "...............",
            "111111111111111",
            "...............",
            ".2222222222222.",
            "...............",
            "111111111111111",
            "...............",
            "..33333333333..",
            "...............",
            "111111111111111",
            "...............",
            ".2222222222222.",
            "...............",
            "111111111111111",
            "...............",
            "..33333333333..",
            "...............",
            ".2222222222222.",
            "...............",
            ".2222222222222.",
            "...............",
            "111111111111111",
            "...............",
            ".2222222222222.",
            "...............",
            ".2222222222222.",
            "...............",
            "111111111111111",
            "...............",
            "111111111111111",
            "...............",
            "111111111111111",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
        });

        // Define brick colors
        const auto rows_color = grid_from_strings(W, H, {
            "...............",
            "000000000000000",
            "...............",
            "111111111111111",
            "...............",
            "222222222222222",
            "...............",
            "333333333333333",
            "...............",
            "444444444444444",
            "...............",
            "555555555555555",
            "...............",
            "666666666666666",
            "...............",
            "777777777777777",
            "...............",
            "888888888888888",
            "...............",
            "999999999999999",
            "...............",
            "000000000000000",
            "...............",
            "111111111111111",
            "...............",
            "222222222222222",
            "...............",
            "333333333333333",
            "...............",
            "444444444444444",
            "...............",
            "555555555555555",
            "...............",
            "666666666666666",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
            });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, color_map_type::rocks, true);
    }(),
    color_map_type::rocks, // Color map type
    assets::img_background_level1_path(), // Background asset
    "             CONTRATULATIONS!             "
    "\n\n"
    "NEXT MISSION (5/10): MARS - THE RED PLANET"
    "\n\n"
    "          PRESS ANY KEY TO START          " // Intro text
};

// Design the arrangement of level 2 (Uranus)
static level_data level6 = {
    brick_config::brick_columns, // columns
    brick_config::brick_rows, // rows
    4.0f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = brick_config::brick_columns;
        const int H = brick_config::brick_rows;

        // Define brick strengths
        const auto rows_strength = grid_from_strings(W, H, {
            "...............",
            ".#############.",
            ".#11111111111#.",
            ".#11222222211#.",
            ".#11222222211#.",
            ".#11223332211#.",
            ".#11223332211#.",
            ".#11223332211#.",
            "##11223332211##",
            "  11223332211..",
            "..11223332211..",
            "..#122333221#..",
            "..#122333221#..",
            "..#122333221#..",
            "..#122333221#..",
            "..#122222221#..",
            "..#111111111#..",
            "..#3#######3#..",
        });

        // Define brick colors
        const auto rows_color = grid_from_strings(W, H, {
            "...............",
            ".#############.",
            ".#00000000000#.",
            ".#00111111100#.",
            ".#00111111100#.",
            ".#00112221100#.",
            ".#00112221100#.",
            ".#00112221100#.",
            "##00112221100##",
            "  00112221100..",
            "..00112221100..",
            "..#011222110#..",
            "..#011222110#..",
            "..#011222110#..",
            "..#011111111#..",
            "..#011111110#..",
            "..#000000000#..",
            "..#3#######3#..",
            });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, color_map_type::tierra, false);
    }(),
    color_map_type::tierra, // Color map type
    assets::img_background_level4_path(), // Background asset
    "             CONTRATULATIONS!             "
    "\n\n"
    "NEXT MISSION (5/10): EARTH - HUMANS' HOME"
    "\n\n"
    "          PRESS ANY KEY TO START          " // Intro text
    };

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
    &level2,
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