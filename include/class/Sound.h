#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <vector>
#include <SDL3/SDL_audio.h>

class Sound {
  private:
    struct ActiveSound {
      SDL_AudioStream* stream;
    };
    struct LoopSound {
      SDL_AudioStream* stream;
      Uint8* buffer;
      Uint32 length;
      float duration;
      float timer;
    };
    std::vector<ActiveSound> activeSounds;
    std::vector<LoopSound> loopSounds;

  public:
    Sound();
    ~Sound();
    void playSound(const std::string& path);
    void update();
    void playSoundLoop(const std::string& path);
    void updateLoop(float dt);
};
#endif