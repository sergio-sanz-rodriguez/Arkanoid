#ifndef ASSETS_H
#define ASSETS_H

#include <SFML/Graphics.hpp>

namespace assets {

    inline constexpr std::string_view image_dir{ "..\\..\\assets\\images\\" };
    inline constexpr std::string_view sfx_dir{ "..\\..\\assets\\sfx\\" };
    inline constexpr std::string_view img_background_level1_filename{ "level_1.png" };
    inline constexpr std::string_view img_background_level2_filename{ "level_2.png" };
    inline constexpr std::string_view img_background_level3_filename{ "level_3.png" };
    inline constexpr std::string_view img_background_level4_filename{ "level_4.png" };
    inline constexpr std::string_view img_background_level5_filename{ "level_5.png" };
    inline constexpr std::string_view img_background_level6_filename{ "level_6.png" };
    inline constexpr std::string_view img_background_level7_filename{ "level_7.png" };
    inline constexpr std::string_view img_background_level8_filename{ "level_8.png" };
    inline constexpr std::string_view img_background_level9_filename{ "level_9.png" };
    inline constexpr std::string_view img_ball_filename{ "ball_llwhite.png" };
    inline constexpr std::string_view img_ballstorm_filename{ "burst.png" };
    inline constexpr std::string_view img_paddle_lightg_filename{ "paddle_light_gray.png" };
    inline constexpr std::string_view img_paddle_darkg_filename{ "paddle_dark_gray.png" };
    inline constexpr std::string_view img_paddle_gold_filename{ "paddle_gold.png" };
    inline constexpr std::string_view img_brick_filename{ "brick_s.png" };
    inline constexpr std::string_view img_plasma_ball_filename{ "ball_orange.png" };
    inline constexpr std::string_view img_life_filename{ "ball_green.png" };
    inline constexpr std::string_view img_powerup_filename{ "ball_blue.png" };
    inline constexpr std::string_view sfx_ball_brick_filename{ "ball_brick.wav" };
    inline constexpr std::string_view sfx_ball_paddle_filename{ "ball_paddle.wav" };
    inline constexpr std::string_view sfx_ball_wall_filename{ "ball_wall.wav" };
    inline constexpr std::string_view sfx_ballstorm_filename{ "ballstorm.wav" };
    inline constexpr std::string_view sfx_game_over_filename{ "game_over.wav" };
    inline constexpr std::string_view sfx_life_minus_filename{ "life_minus.wav" };
    inline constexpr std::string_view sfx_player_wins_filename{ "player_wins.wav" };
    inline constexpr std::string_view sfx_powerdown_filename{ "powerdown.wav" };
    inline constexpr std::string_view sfx_powerup_filename{ "powerup.wav" };
    inline constexpr std::string_view sfx_welcome_filename{ "intro.wav" };
    //inline constexpr std::string_view font_filename{ "/usr/local/share/fonts/webfonts/verdana.ttf" };
    inline constexpr std::string_view font_verdana{ "..\\..\\assets\\fonts\\verdana.ttf" };
    inline constexpr std::string_view font_consola{ "..\\..\\assets\\fonts\\consola.ttf" };
    inline std::string img_background_level1_path() { return std::string(image_dir) + std::string(img_background_level1_filename); }
    inline std::string img_background_level2_path() { return std::string(image_dir) + std::string(img_background_level2_filename); }
    inline std::string img_background_level3_path() { return std::string(image_dir) + std::string(img_background_level3_filename); }
    inline std::string img_background_level4_path() { return std::string(image_dir) + std::string(img_background_level4_filename); }
    inline std::string img_background_level5_path() { return std::string(image_dir) + std::string(img_background_level5_filename); }
    inline std::string img_background_level6_path() { return std::string(image_dir) + std::string(img_background_level6_filename); }
    inline std::string img_background_level7_path() { return std::string(image_dir) + std::string(img_background_level7_filename); }
    inline std::string img_background_level8_path() { return std::string(image_dir) + std::string(img_background_level8_filename); }
    inline std::string img_background_level9_path() { return std::string(image_dir) + std::string(img_background_level9_filename); }
    inline std::string img_ball_path() { return std::string(image_dir) + std::string(img_ball_filename); }
    inline std::string img_ballstorm_path() { return std::string(image_dir) + std::string(img_ballstorm_filename); }
    inline std::string img_paddle_lightg_path() { return std::string(image_dir) + std::string(img_paddle_lightg_filename); }
    inline std::string img_paddle_darkg_path() { return std::string(image_dir) + std::string(img_paddle_darkg_filename); }
    inline std::string img_paddle_gold_path() { return std::string(image_dir) + std::string(img_paddle_gold_filename); }
    inline std::string img_brick_path() { return std::string(image_dir) + std::string(img_brick_filename); }
    inline std::string img_plasma_ball_path() { return std::string(image_dir) + std::string(img_plasma_ball_filename); }
    inline std::string img_life_path() { return std::string(image_dir) + std::string(img_life_filename); }
    inline std::string img_powerup_path() { return std::string(image_dir) + std::string(img_powerup_filename); }
    inline std::string sfx_ball_brick_path() { return std::string(sfx_dir) + std::string(sfx_ball_brick_filename); }
    inline std::string sfx_ball_paddle_path() { return std::string(sfx_dir) + std::string(sfx_ball_paddle_filename); }
    inline std::string sfx_ball_wall_path() { return std::string(sfx_dir) + std::string(sfx_ball_wall_filename); }
    inline std::string sfx_ballstorm_path() { return std::string(sfx_dir) + std::string(sfx_ballstorm_filename); }
    inline std::string sfx_game_over_path() { return std::string(sfx_dir) + std::string(sfx_game_over_filename); }
    inline std::string sfx_life_minus_path() { return std::string(sfx_dir) + std::string(sfx_life_minus_filename); }
    inline std::string sfx_player_wins_path() { return std::string(sfx_dir) + std::string(sfx_player_wins_filename); }
    inline std::string sfx_powerdown_path() { return std::string(sfx_dir) + std::string(sfx_powerdown_filename); }
    inline std::string sfx_powerup_path() { return std::string(sfx_dir) + std::string(sfx_powerup_filename); }
    inline std::string sfx_welcome_path() { return std::string(sfx_dir) + std::string(sfx_welcome_filename); }

}

#endif // ASSETS_H
#pragma once