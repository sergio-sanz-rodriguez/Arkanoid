#include <stdexcept>
#include <algorithm>
#include <random>
#include "levels.h"
#include "assets.h"

// Return the color vector corresponding to a given color map
const std::vector<sf::Color>& get_color_vector(brick_colors map) {
    switch (map) {
    case brick_colors::arcade:    return brick_color_maps::arcade;
    case brick_colors::cosmic:    return brick_color_maps::cosmic;
    case brick_colors::planets:   return brick_color_maps::planets;
    case brick_colors::starfield: return brick_color_maps::starfield;
    case brick_colors::alien:     return brick_color_maps::alien;
    case brick_colors::darkmatter:return brick_color_maps::darkmatter;
    case brick_colors::grayscale: return brick_color_maps::grayscale;
    case brick_colors::medal:     return brick_color_maps::medal;
    case brick_colors::rocks:     return brick_color_maps::rocks;
    case brick_colors::tierra:    return brick_color_maps::tierra;
    }
    return brick_color_maps::arcade;
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
    brick_colors color_map,
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
    brick_colors color_map
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
            "..33333333333.."
        });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, brick_colors::arcade, true);
    }(),
    assets::img_background_level1_path(), // Background asset
    "1: NEPTUNE - FROZEN FRONTIER [EASY]\n\n", // Menu text
    ball_colors::steel, // Ball color
    paddle_colors::light_gray, // Paddle color
    brick_colors::arcade // Brick colors
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
        const auto rows_color = random_rows_color(W, H, rows_strength, brick_colors::arcade);

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, brick_colors::arcade, false);
    }(),
    assets::img_background_level2_path(), // Background asset
    "2: URANUS - ICE ANOMALY [EASY]\n\n", // Menu text
    ball_colors::steel, // Ball color
    paddle_colors::light_gray, // Paddle color
    brick_colors::arcade // Brick colors
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
        return parse_grid(W, H, rows_strength, rows_color, brick_colors::medal, true);
    }(),
    assets::img_background_level3_path(), //Background asset
    "3: TITAN - SATURN'S MOON [MEDIUM]\n\n", // Menu text
    ball_colors::steel, // Ball color
    paddle_colors::light_gray, // Paddle color
    brick_colors::medal // Brick colors
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
        return parse_grid(W, H, rows_strength, rows_color, brick_colors::cosmic, true);
    }(),
    assets::img_background_level4_path(), // Background asset
    "4: EUROPA - JUPITER'S MOON [MEDIUM]\n\n", // Menu text
    ball_colors::steel, // Ball color
    paddle_colors::dark_gray, // Paddle color
    brick_colors::cosmic // Brick colors
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
            "...............",
            "...............",
            "...............",
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
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
            });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, brick_colors::rocks, true);
    }(),
    assets::img_background_level5_path(), // Background asset
    "5: MARS - THE RED PLANET [MEDIUM]\n\n", // Menu text
    ball_colors::steel, // Ball color
    paddle_colors::light_gray, // Paddle color
    brick_colors::rocks // Brick colors
};

// Design the arrangement of level 6 (Earth)
static level_data level6 = {
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
            ".#############.",
            ".#11111111111#.",
            ".#11222222211#.",
            ".#11222222211#.",
            ".#11223332211#.",
            ".#11223332211#.",
            "##11223332211##",
            "#.11223332211.#",
            ". 11223332211..",
            "..11223332211..",
            "..11223332211..",
            "..#122333221#..",
            "..#122333221#..",
            "..#122222221#..",
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
            "##00112221100##",
            "#.00112221100.#",
            ". 00112221100..",
            "..00112221100..",
            "..00112221100..",
            "..#011222110#..",
            "..#011222110#..",
            "..#011111110#..",
            "..#011111110#..",
            "..#000000000#..",
            "..#2#######2#..",
            });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, brick_colors::tierra, false);
    }(),
    assets::img_background_level6_path(), // Background asset
    "6: EARTH - LAST HUMAN STRONGHOLD [DIFFICULT]\n\n", // Menu text
    ball_colors::steel, // Ball color
    paddle_colors::light_gray, // Paddle color
    brick_colors::tierra // Brick colors
    };

// Design the arrangement of level 7 (Venus)
static level_data level7 = {
    brick_config::brick_columns, // columns
    brick_config::brick_rows, // rows
    3.7f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = brick_config::brick_columns;
        const int H = brick_config::brick_rows;

        // Define brick strengths
        const auto rows_strength = grid_from_strings(W, H, {
            "#..............",
            ".........#.....",
            "...............",
            "..#1111#1111#..",
            "..11121113121.2",
            "3.11#11#11311..",
            "..111#2#11111..",
            "..1211111#2#1..",
            "3311112321111#3",
            "..11213#31211..",
            "..11111311111..",
            "..1111232#111..",
            "2.11#23222111..",
            "..1#111111111..",
            "..21111#1111#2.",
            "....11##113....",
            ".... 123111....",
            "....113211 ....",
            "...#22222221...",
            "....122222#....",
            "3....1333#..2..",
            "......#31......",
            "..3............",
            ".....1....3....",
            "#..............",
            "..............#",
        });

        // Define brick colors
        const auto rows_color = grid_from_strings(W, H, {
            "#..............",
            ".........#.....",
            "...............",
            "..#0000#0000#..",
            "..00010102010.1",
            "2.00#00#00200..",
            "..000#1#00000..",
            "..0100000#1#0..",
            "2200001210000#2",
            "..00102#20100..",
            "..00000200000..",
            "..0000121#000..",
            "1.00#12111000..",
            "..0#000000000..",
            "..10000#0000#1.",
            "....00##002....",
            ".... 012000....",
            "....002100 ....",
            "...#11111110...",
            "....011111#....",
            "2....0222#..1..",
            "......#20......",
            "..2............",
            ".....0....2....",
            "#..............",
            "..............#",
        });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, brick_colors::alien, true);
    }(),
    assets::img_background_level7_path(), // Background asset
    "7: VENUS - 500°C INFERNO [DIFFICULT]\n\n", // Menu text
    ball_colors::steel, // Ball color
    paddle_colors::light_gray, // Paddle color
    brick_colors::alien // Brick colors
};

// Design the arrangement of level 8 (Mercury)
static level_data level8 = {
    brick_config::brick_columns, // columns
    brick_config::brick_rows, // rows
    4.5f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = brick_config::brick_columns;
        const int H = brick_config::brick_rows;

        // Define brick strengths
        const auto rows_strength = grid_from_strings(W, H, {
            "321............",
            "....#..........",
            "........1.#....",
            "1.31331...#11#.",
            "..33131...#11#.",
            "..22111.1.#11#.",
            "#.2232.1..#11#.",
            "..1112#...#11#.",
            "..1213112.#11#.",
            ".#11111...#11#.",
            "..123123..#11#.",
            "11113111.1#11#.",
            "21131213..#11#.",
            "21331112..#11#.",
            "331111211.#11#.",
            "111#11111.#11#.",
            "221132111.#11#.",
            "#11332113.#11#.",
            "11222111..####.",
            "21131113....1..",
            "121121123..1...",
            "3221131111#11#.",
            "3212121311####.",
            "111111111313111",
            "112111223112231",
            "223112131132111",
            "...............",
            "...............",
            "...............",
            ".......#.......",
        });

        // Define brick colors
        const auto rows_color = grid_from_strings(W, H, {
            "210............",
            "....#..........",
            "........0.#....",
            "0.20220...#00#.",
            "..22020...#00#.",
            "..11000.0.#00#.",
            "#.1121.0..#00#.",
            "..0001#...#00#.",
            "..0102001.#00#.",
            ".#00000...#00#.",
            "..012012..#00#.",
            "00002000.0#00#.",
            "10020102..#00#.",
            "10220001..#00#.",
            "220000101.#00#.",
            "000#00000.#00#.",
            "110021000.#00#.",
            "#00221002.#00#.",
            "00111000..####.",
            "10020002....0..",
            "010010012..0...",
            "2110020000#00#.",
            "2101010100####.",
            "000000000202000",
            "011010112011120",
            "112010102021000",
            "...............",
            "...............",
            "...............",
            ".......#.......",
        });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, brick_colors::planets, true);
    }(),
    assets::img_background_level8_path(), // Background asset
    "8: MERCURY - THE BURNING ROCK [DIFFICULT]\n\n", // Menu text
    ball_colors::steel, // Ball color
    paddle_colors::light_gray, // Paddle color
    brick_colors::planets // Brick colors
};

// Design the arrangement of level 9 (Sun)
static level_data level9 = {
    brick_config::brick_columns, // columns
    brick_config::brick_rows, // rows
    5.0f, // width offset, to center the text in the screen
    3.0f, // height offset
    [] {
        const int W = brick_config::brick_columns;
        const int H = brick_config::brick_rows;

        // Define brick strengths
        const auto rows_strength = grid_from_strings(W, H, {
            "111111111111111",
            "222222222222222",
            "222222222222222",
            "333333333333333",
            "333333333333333",
            "333333333333333",
            "...............",
            "...............",
            "#....#.....#...",
            "...............",
            "...............",
            "...##...##...##",
            "...............",
            "...............",
            "333333333333333",
            "222222222222222",
            "222222222222222",
            "222222222222222",
            "111111111111111",
            "111111111111111",
            "111111111111111",
            "...............",
            "...............",
            "...............",
            "#..##..##..##..",
            "...............",
            "...............",
            ".##..##..##..##",
            "...............",
            "...............",
            "..##..##..##..#",
            "...............",
            "...............",
            "##..##..##..##.",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
        });

        // Define brick colors
        const auto rows_color = grid_from_strings(W, H, {
            "000000000000000",
            "111111111111111",
            "111111111111111",
            "222222222222222",
            "222222222222222",
            "222222222222222",
            "...............",
            "...............",
            "#....#.....#...",
            "...............",
            "...............",
            "...##...##...##",
            "...............",
            "...............",
            "222222222222222",
            "111111111111111",
            "111111111111111",
            "111111111111111",
            "000000000000000",
            "000000000000000",
            "000000000000000",
            "...............",
            "...............",
            "...............",
            "#..##..##..##..",
            "...............",
            "...............",
            ".##..##..##..##",
            "...............",
            "...............",
            "..##..##..##..#",
            "...............",
            "...............",
            "##..##..##..##.",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
            "...............",
        });

        // Parse grid
        return parse_grid(W, H, rows_strength, rows_color, brick_colors::starfield, true);
    }(),
    assets::img_background_level9_path(), // Background asset
    "9: SUN - CORE ASSAULT [VERY DIFFICULT]", // Menu text
    ball_colors::gold, // Ball color
    paddle_colors::gold, // Paddle color
    brick_colors::starfield // Brick colors
};

// Stack the difficulty levels
static const level_data* all_levels[] = {
    &level1,
    //&level2,
    //&level3,
    //&level4,
    //&level5,
    //&level6,
    //&level7,
    //&level8,
    //&level9
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