#ifndef ASSETS_H
#define ASSETS_H

#include <SFML/Graphics.hpp>
#include <windows.h>
#include <filesystem>
#include <string>
#include <string_view>

namespace assets {

    // --- Runtime root: <exe_dir>\assets ---
    inline std::filesystem::path exe_dir()
    {
        wchar_t buf[MAX_PATH]{};
        GetModuleFileNameW(nullptr, buf, MAX_PATH);
        return std::filesystem::path(buf).parent_path();
    }

    inline std::filesystem::path root_dir()
    {
        return exe_dir() / "assets";
    }

    inline std::filesystem::path images_dir()
    {
        return root_dir() / "images";
    }

    inline std::filesystem::path sfx_dir()
    {
        return root_dir() / "sfx";
    }

    inline std::filesystem::path fonts_dir()
    {
        return root_dir() / "fonts";
    }

    //inline constexpr std::string_view image_dir{ "..\\..\\assets\\images\\" };
    //inline constexpr std::string_view sfx_dir{ "..\\..\\assets\\sfx\\" };
    inline constexpr std::string_view txt_background_filename{ "background.png" };
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
    inline constexpr std::string_view img_antimatter_ball_filename{ "ball_blueviolet.png" };
    inline constexpr std::string_view img_life_filename{ "ball_green.png" };
    inline constexpr std::string_view img_powerup_filename{ "ball_blue.png" };
    inline constexpr std::string_view sfx_ball_brick_filename{ "ball_brick.wav" };
    inline constexpr std::string_view sfx_ball_paddle_filename{ "ball_paddle.wav" };
    inline constexpr std::string_view sfx_ball_wall_filename{ "ball_wall.wav" };
    inline constexpr std::string_view sfx_ballstorm_filename{ "ballstorm.wav" };
    inline constexpr std::string_view sfx_paddle_enlarge_filename{ "enlarge.wav" };
    inline constexpr std::string_view sfx_game_over_filename{ "game_over.wav" };
    inline constexpr std::string_view sfx_life_minus_filename{ "life_minus.wav" };
    inline constexpr std::string_view sfx_player_wins_filename{ "player_wins.wav" };
    inline constexpr std::string_view sfx_powerdown_filename{ "powerdown.wav" };
    inline constexpr std::string_view sfx_powerup_filename{ "powerup.wav" };
    inline constexpr std::string_view sfx_welcome_filename{ "intro.wav" };

    inline constexpr std::string_view font_verdana_filename{ "verdana.ttf" };
    inline constexpr std::string_view font_consola_filename{ "consola.ttf" };

    // helper: join dir + filename and return std::string (SFML takes std::string / path-like)
    inline std::string join_to_string(const std::filesystem::path& dir, std::string_view filename)
    {
        return (dir / std::filesystem::path(filename)).string();
    }

    // Paths
    inline std::string txt_background_path() { return join_to_string(images_dir(), txt_background_filename); }
    inline std::string img_background_level1_path() { return join_to_string(images_dir(), img_background_level1_filename); }
    inline std::string img_background_level2_path() { return join_to_string(images_dir(), img_background_level2_filename); }
    inline std::string img_background_level3_path() { return join_to_string(images_dir(), img_background_level3_filename); }
    inline std::string img_background_level4_path() { return join_to_string(images_dir(), img_background_level4_filename); }
    inline std::string img_background_level5_path() { return join_to_string(images_dir(), img_background_level5_filename); }
    inline std::string img_background_level6_path() { return join_to_string(images_dir(), img_background_level6_filename); }
    inline std::string img_background_level7_path() { return join_to_string(images_dir(), img_background_level7_filename); }
    inline std::string img_background_level8_path() { return join_to_string(images_dir(), img_background_level8_filename); }
    inline std::string img_background_level9_path() { return join_to_string(images_dir(), img_background_level9_filename); }

    inline std::string img_ball_path() { return join_to_string(images_dir(), img_ball_filename); }
    inline std::string img_ballstorm_path() { return join_to_string(images_dir(), img_ballstorm_filename); }
    inline std::string img_paddle_lightg_path() { return join_to_string(images_dir(), img_paddle_lightg_filename); }
    inline std::string img_paddle_darkg_path() { return join_to_string(images_dir(), img_paddle_darkg_filename); }
    inline std::string img_paddle_gold_path() { return join_to_string(images_dir(), img_paddle_gold_filename); }
    inline std::string img_brick_path() { return join_to_string(images_dir(), img_brick_filename); }
    inline std::string img_plasma_ball_path() { return join_to_string(images_dir(), img_plasma_ball_filename); }
    inline std::string img_antimatter_ball_path() { return join_to_string(images_dir(), img_antimatter_ball_filename); }
    inline std::string img_life_path() { return join_to_string(images_dir(), img_life_filename); }
    inline std::string img_powerup_path() { return join_to_string(images_dir(), img_powerup_filename); }

    inline std::string sfx_ball_brick_path() { return join_to_string(sfx_dir(), sfx_ball_brick_filename); }
    inline std::string sfx_ball_paddle_path() { return join_to_string(sfx_dir(), sfx_ball_paddle_filename); }
    inline std::string sfx_ball_wall_path() { return join_to_string(sfx_dir(), sfx_ball_wall_filename); }
    inline std::string sfx_ballstorm_path() { return join_to_string(sfx_dir(), sfx_ballstorm_filename); }
    inline std::string sfx_paddle_enlarge_path() { return join_to_string(sfx_dir(), sfx_paddle_enlarge_filename); }
    inline std::string sfx_game_over_path() { return join_to_string(sfx_dir(), sfx_game_over_filename); }
    inline std::string sfx_life_minus_path() { return join_to_string(sfx_dir(), sfx_life_minus_filename); }
    inline std::string sfx_player_wins_path() { return join_to_string(sfx_dir(), sfx_player_wins_filename); }
    inline std::string sfx_powerdown_path() { return join_to_string(sfx_dir(), sfx_powerdown_filename); }
    inline std::string sfx_powerup_path() { return join_to_string(sfx_dir(), sfx_powerup_filename); }
    inline std::string sfx_welcome_path() { return join_to_string(sfx_dir(), sfx_welcome_filename); }
    inline std::string font_verdana_path() { return join_to_string(fonts_dir(), font_verdana_filename); }
    inline std::string font_consola_path() { return join_to_string(fonts_dir(), font_consola_filename); }
}

#endif // ASSETS_H
#pragma once