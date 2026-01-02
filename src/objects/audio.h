#ifndef AUDIO_H
#define AUDIO_H

#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

enum class sfx_id {
    none,
    ball_brick,
    ball_paddle,
    ball_wall,
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

class audio_manager {
public:
    audio_manager() = default;

    bool load(sfx_id id, const std::string& path) {
        auto& buf = buffers[id];
        return buf.loadFromFile(path);
    }

    void play(sfx_id id) {

        // Remove old finished sounds so the vector doesn't grow forever
        cleanup();

        // If we are at capacity, just skip playing the new sound
        if (active_sounds.size() >= constants::max_sounds)
            return;

        auto it = buffers.find(id);
        if (it == buffers.end())
            return;

        // Sound must be constructed with a buffer
        active_sounds.emplace_back(it->second);
        active_sounds.back().play();
    }

    void cleanup() {
        // Remove all sounds that finished playing
        active_sounds.erase(
            std::remove_if(active_sounds.begin(), active_sounds.end(),
                [](const sf::Sound& s) {
            return s.getStatus() == sf::Sound::Status::Stopped;
        }),
            active_sounds.end()
        );
    }

private:
    std::unordered_map<sfx_id, sf::SoundBuffer, sfx_id_hash> buffers;
    std::vector<sf::Sound> active_sounds;
};

#endif // AUDIO_H