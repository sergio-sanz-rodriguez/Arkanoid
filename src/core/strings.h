#ifndef STRINGS_H
#define STRINGS_H

#include <SFML/Graphics.hpp>

namespace strings {

    inline constexpr const char* arkanoid_title{ "Arkanoid: Reclaiming the Solar System" };
    inline constexpr std::string_view string_instructions{
        "      ARKANOID: RECLAIMING THE SOLAR SYSTEM        "
        "\n\n"
        "                    YEAR 3056.                     "
        "\n"
        "           AI-CREATED COSMIC STRUCTURES            "
        "\n"
        "           HAVE INVADED THE SOLAR SYSTEM.          "
        "\n"
        "          PLANET AFTER PLANET HAS FALLEN.          "
        "\n"
        "          THE SUN ITSELF IS UNDER THREAT.          "
        "\n\n"
        "  YOU ARE A HERO FROM THE ALPHA CENTAURI SYSTEM,   "
        "\n"
        "           SENT TO SAVE YOUR NEIGHBORS.            "
        "\n\n"
        "          YOU CONTROL THE LAST DEFENSE:            "
        "\n"
        "            A PADDLE AND ENERGY BALLS.             "
        "\n\n"
        "                BREAK THE BRICKS.                  "
        "\n"
        "               RECLAIM THE PLANETS.                "
        "\n"
        "                  SAVE THE SUN.                    "
        "\n\n"
        "                  INSTRUCTIONS:                    "
        "\n\n"
        "- LEFT ARROW / MOVE MOUSE LEFT: MOVE PADDLE LEFT   "
        "\n"
        "- RIGHT ARROW / MOVE MOUSE RIGHT: MOVE PADDLE RIGHT"
        "\n"
        "- SPACE: CONTINUE / LAUNCH BALL                    "
        "\n"
        "- P: PAUSE / RESUME                                "
        "\n"
        "- R: RESET                                         "
        "\n"
        "- CATCH BLUE AND ORANGE BALLS TO GET A POWER-UP    "
        "\n"
        "- CATCH THE GREEN BALL TO GAIN AN EXTRA LIFE       "
        "\n\n"
        "             PRESS SPACE TO CONTINUE.              "
    };
    inline constexpr std::string_view string_paused{ "PAUSED" };
    inline constexpr std::string_view string_game_over{
        "  GAME OVER!"
        "\n\n"
        "  YOUR MISSION TO RECLAIM THE SOLAR SYSTEM FAILED."
        "\n"
        "  THE SUN IS STILL UNDER THREAT."
        "\n\n"
        "  - PRESS ANY KEY TO TRY AGAIN AND SAVE THE SUN"
        "\n"
        "  - PRESS ESCAPE TO QUIT"
    };
    inline constexpr std::string_view string_player_wins{
        "          SUCESSFULL MISSION!           "
        "\n\n"
        "  YOU HAVE DEFEATED THE COSMIC BRICKS.  "
        "\n"
        "    ALL PLANETS HAVE BEEN RECLAIMED,    "
        "\n"
        "       AND THE SUN IS SAFE AGAIN.       "
        "\n\n"
        "      THE SOLAR SYSTEM THANKS YOU,      "
        "\n"
        "       HERO FROM ALPHA CENTAURI!        "
        "\n\n"
        "      PRESS ANY KEY TO PLAY AGAIN       "
        "\n"
        "         PRESS ESCAPE TO QUIT           "
    };
    inline constexpr std::string_view string_first_level_keys{
        ""
        "\n\n"
        "CHOOSE YOUR FIRST TARGET (1-9):"
    };
    inline constexpr std::string_view string_next_level_keys{
        "MISSION COMPLETE!"
        "\n\n"
        "CHOOSE YOUR NEXT TARGET (1-9):"
    };
}

#endif // STRINGS_H
#pragma once