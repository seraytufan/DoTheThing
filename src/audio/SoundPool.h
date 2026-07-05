#pragma once

#include <SFML/Audio.hpp>
#include <memory>
#include <vector>


// SoundPool

// Used to prevent the same sound effect from cutting itself off when played repeatedly.
// Multiple sf::Sound objects share the same sf::SoundBuffer.
// When playing, it uses the first available sound channel; if all are busy, it reuses the next one.
// It also supports fade out, so active sounds can be lowered gradually instead of stopping suddenly.


class SoundPool
{
public:
    void init(std::shared_ptr<sf::SoundBuffer> buffer, std::size_t voiceCount);

    // Plays through an available channel, or reuses the next channel if all are busy.
    void play(float pitch, float volume);

    // Gradually lowers and stops active sounds, update() handles the fade.
    void beginFadeOut(float fadeDuration);
    void update(float deltaTime);

    bool isFadingOut() const { return fadingOut; }

private:
    std::shared_ptr<sf::SoundBuffer> buffer;
    std::vector<sf::Sound> voices;
    std::size_t nextStealIndex = 0;

    bool fadingOut = false;
    float fadeTimer = 0.f;
    float fadeDuration = 0.f;
    std::vector<float> fadeBaseVolume;
};
