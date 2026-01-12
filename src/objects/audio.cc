#include "audio.h"

// Load audio
bool audio_manager::load(sfx_id id, const std::string& path) {
    auto& buf = buffers[id];
    return buf.loadFromFile(path);
}

// Play audio
void audio_manager::play(sfx_id id) {
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

// Stop audio
void audio_manager::stop(sfx_id id) {
    auto it = buffers.find(id);
    if (it == buffers.end())
        return;

    const sf::SoundBuffer* target = &it->second;

    for (auto& s : active_sounds) {
        if (&s.getBuffer() == target) {
            s.stop();
        }
    }

    // Optional
    cleanup();
}


// Clean up active audios
void audio_manager::cleanup() {

    // Remove all sounds that finished playing
    active_sounds.erase(
        std::remove_if(active_sounds.begin(), active_sounds.end(),
            [](const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Status::Stopped;
    }),
        active_sounds.end()
    );
}