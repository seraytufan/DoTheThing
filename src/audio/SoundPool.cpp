#include "SoundPool.h"
#include "../core/AudioSettings.h"
#include <algorithm>

void SoundPool::init(std::shared_ptr<sf::SoundBuffer> soundBuffer, std::size_t voiceCount)
{
    buffer = std::move(soundBuffer);

    voices.clear();
    voices.resize(voiceCount);
    for (auto& voice : voices)
    {
        if (buffer) voice.setBuffer(*buffer);
    }
}

void SoundPool::play(float pitch, float volume)
{
    if (voices.empty()) return;

    // Scale by the global SFX master volume set in the Settings popup.
    float effectiveVolume = volume * AudioSettings::instance().sfxVolume01;

    for (auto& voice : voices)
    {
        if (voice.getStatus() != sf::Sound::Playing)
        {
            voice.setPitch(pitch);
            voice.setVolume(effectiveVolume);
            voice.play();
            return;
        }
    }

    // Every voice is busy: steal one in round robin order.
    sf::Sound& stolen = voices[nextStealIndex];
    stolen.setPitch(pitch);
    stolen.setVolume(effectiveVolume);
    stolen.play();
    nextStealIndex = (nextStealIndex + 1) % voices.size();
}

void SoundPool::beginFadeOut(float duration)
{
    fadingOut = true;
    fadeTimer = 0.f;
    fadeDuration = duration;

    fadeBaseVolume.assign(voices.size(), 0.f);
    for (std::size_t i = 0; i < voices.size(); ++i)
    {
        fadeBaseVolume[i] = voices[i].getVolume();
    }
}

void SoundPool::update(float deltaTime)
{
    if (!fadingOut) return;

    fadeTimer += deltaTime;
    float t = std::clamp(fadeTimer / fadeDuration, 0.f, 1.f);
    float remaining = 1.f - t;

    for (std::size_t i = 0; i < voices.size(); ++i)
    {
        if (voices[i].getStatus() == sf::Sound::Playing)
        {
            voices[i].setVolume(fadeBaseVolume[i] * remaining);
        }
    }

    if (t >= 1.f)
    {
        for (auto& voice : voices)
        {
            if (voice.getStatus() == sf::Sound::Playing)
            {
                voice.stop();
            }
        }
        fadingOut = false;
    }
}