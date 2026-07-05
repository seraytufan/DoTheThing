#pragma once


// AudioSettings

// Global master volume multipliers (0..1) for music and sound effects, set by
// the Settings popup and read wherever audio is produced:
// music: Game multiplies kGameMusicVolume by musicVolume01 each frame.
// sfx:   SoundPool multiplies every play() volume by sfxVolume01.

class AudioSettings
{
public:
    static AudioSettings& instance()
    {
        static AudioSettings settings;
        return settings;
    }

    AudioSettings(const AudioSettings&) = delete;
    AudioSettings& operator=(const AudioSettings&) = delete;

    float musicVolume01 = 1.f;
    float sfxVolume01 = 1.f;

private:
    AudioSettings() = default;
};