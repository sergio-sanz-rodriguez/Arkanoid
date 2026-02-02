#ifndef AUDIO_H
#define AUDIO_H
#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>

#include <SFML/Audio.hpp>
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

// Handles preloaded SFX buffers and plays them through a fixed pool of reusable voices.
class audio_manager {
private:
    std::unordered_map<sfx_id, sf::SoundBuffer, sfx_id_hash> buffers;
    std::vector<std::optional<sf::Sound>> active_sounds; // Voice slots (optional because sf::Sound has no default ctor in SFML 3).
    std::unordered_map<sfx_id, float, sfx_id_hash> sfx_volume = {
        { sfx_id::enlarge, constants::sfx_enlarge_volume }, // Per-sound volume overrides.
    };

public:

    // Preallocates voice slots to avoid allocations during gameplay
    audio_manager();

    // Loads a sound buffer from disk once at startup
    bool load(sfx_id id, const std::string& path);

    // Plays a preloaded sound effect using a free voice slot (no file I/O)
    void play(sfx_id id);

    // Stops all currently playing voices that use the given buffer
    void stop(sfx_id id);

    // Reclaims voice slots whose sounds finished playing
    void cleanup();

    // Forces audio backend initialization at startup to avoid first-play hitches
    void warmup_all();
};

#endif // AUDIO_H
