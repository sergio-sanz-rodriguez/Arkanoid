#ifndef AUDIO_H
#define AUDIO_H

#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include "constants.h"

enum class sfx_id {
    none,
    ball_brick,
    ball_paddle,
    ball_wall,
    ballstorm,
    enlarge,
    game_over,
    life_minus,
    player_wins,
    powerdown,
    powerup,
    welcome
};

struct sfx_id_hash {
    std::size_t operator()(sfx_id id) const noexcept {
        return static_cast<std::size_t>(id);
    }
};

//Class that handles the sound effects
class audio_manager {
private:

    std::unordered_map<sfx_id, sf::SoundBuffer, sfx_id_hash> buffers;
    std::vector<sf::Sound> active_sounds;
    std::unordered_map<sfx_id, float, sfx_id_hash> sfx_volume = {
        { sfx_id::enlarge, constants::sfx_enlarge_volume },
        // everything else will default to 80.0f
    };

public:

    audio_manager() = default;

    // Load audio file
    bool load(sfx_id id, const std::string& path);

    // Play audio
    void play(sfx_id id);

    // Stop audio
    void stop(sfx_id id);

    //Clean up buffers
    void cleanup();

};

#endif // AUDIO_H