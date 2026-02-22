#include "audio.h"

// Preallocate N empty voice slots (sf::Sound is not default-constructible in SFML 3).
audio_manager::audio_manager()
{
    active_sounds.resize(constants::max_sounds);
}

// Load a sound buffer from disk once (all file I/O should happen here, not in play()).
bool audio_manager::load(sfx_id id, const std::string& path)
{
    auto& buf = buffers[id];
    return buf.loadFromFile(path);
}

// Reclaim voice slots whose sounds have finished playing (keeps pool reusable).
void audio_manager::cleanup()
{
    for (auto& slot : active_sounds) {
        if (slot && slot->getStatus() == sf::Sound::Status::Stopped) {
            slot.reset();
        }
    }
}

// Play a preloaded sound effect using a free voice slot (no allocations/growth at runtime).
void audio_manager::play(sfx_id id)
{
    auto it = buffers.find(id);
    if (it == buffers.end())
        return;

    float vol = constants::sfx_default_volume;
    if (auto itv = sfx_volume.find(id); itv != sfx_volume.end())
        vol = itv->second;

    for (auto& slot : active_sounds) {
        if (!slot) {
            slot.emplace(it->second); // construct with buffer
        }

        if (slot->getStatus() != sf::Sound::Status::Playing) {
            slot->stop();
            slot->setBuffer(it->second);
            slot->setVolume(std::clamp(vol, 0.f, 100.f));
            slot->play();
            return;
        }
    }
    // drop sound if all voices are busy
    //
}

// Stop all currently playing voices that use the given buffer (e.g., stop welcome music SFX).
void audio_manager::stop(sfx_id id)
{
    // Abort if the requested sound wasn't loaded.
    auto it = buffers.find(id);
    if (it == buffers.end())
        return;

    // Match voices by buffer address so all instances of that SFX are stopped.
    const sf::SoundBuffer* target = &it->second;

    // Iterate the pool and stop any voice playing that buffer.
    for (auto& slot : active_sounds) {
        if (!slot) continue;
        if (&slot->getBuffer() == target) {
            slot->stop();
        }
    }

    // Release any voices that became stopped due to stop().
    cleanup();
}

// Pre-initialize audio backend and force each loaded buffer to be used once (silent) to avoid first-play hitch.
void audio_manager::warmup_all()
{
    if (buffers.empty() || active_sounds.empty())
        return;

    auto& slot = active_sounds[0];
    auto it = buffers.begin(); // pick the first loaded buffer

    if (!slot) {
        // must provide a buffer on construction
        slot.emplace(it->second);
    }
    else {
        // reuse existing sf::Sound, just reset buffer
        slot->setBuffer(it->second);
    }

    slot->setVolume(0.f);
    slot->play();
    slot->stop();
}