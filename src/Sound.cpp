#include <SDL3/SDL.h>
#include "../include/class/Sound.h"
#include <iostream>

Sound::Sound() {

}

Sound::~Sound() {
  for (auto& s : activeSounds) {
    SDL_DestroyAudioStream(s.stream);
  }

  for (auto& ls : loopSounds) {
    SDL_DestroyAudioStream(ls.stream);
    SDL_free(ls.buffer);
  }
}

void Sound::playSound(const std::string& path) {
  SDL_AudioSpec spec{};
  Uint8* wav_buffer = nullptr;
  Uint32 wav_length = 0;

  if (!SDL_LoadWAV(path.c_str(), &spec, &wav_buffer, &wav_length)) {
    std::cerr << "SDL_LoadWAV gagal: " << SDL_GetError() << '\n';
    return;
  }

  SDL_AudioStream* stream =
    SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);

  if (!stream) {
    SDL_free(wav_buffer);
    return;
  }

  if (!SDL_PutAudioStreamData(stream, wav_buffer, wav_length)) {
    SDL_DestroyAudioStream(stream);
    SDL_free(wav_buffer);
    return;
  }

  SDL_FlushAudioStream(stream);
  SDL_ResumeAudioStreamDevice(stream);

  activeSounds.push_back({ stream });

  SDL_free(wav_buffer);
}

void Sound::playSoundLoop(const std::string& path) {
  SDL_AudioSpec spec{};
  Uint8* wav_buffer = nullptr;
  Uint32 wav_length = 0;

  if (!SDL_LoadWAV(path.c_str(), &spec, &wav_buffer, &wav_length)) {
    std::cerr << "SDL_LoadWAV gagal: " << SDL_GetError() << '\n';
    return;
  }

  SDL_AudioStream* stream =
    SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);

  if (!stream) {
    SDL_free(wav_buffer);
    return;
  }

  SDL_PutAudioStreamData(stream, wav_buffer, wav_length);
  SDL_FlushAudioStream(stream);
  SDL_ResumeAudioStreamDevice(stream);

  int bytes_per_sample = (spec.format & 0xFF) / 8;
  int bytes_per_second = spec.freq * spec.channels * bytes_per_sample;
  float duration = (float)wav_length / (float)bytes_per_second;

  loopSounds.push_back({ stream, wav_buffer, wav_length, duration, 0.0f });
}



void Sound::update() {
  for (int i = 0; i < activeSounds.size(); i++) {
    SDL_AudioStream* stream = activeSounds[i].stream;

    if (SDL_GetAudioStreamAvailable(stream) == 0 &&
      !SDL_GetAudioStreamQueued(stream)) {

      SDL_DestroyAudioStream(stream);
      activeSounds.erase(activeSounds.begin() + i);
      i--;
    }
  }
}

void Sound::updateLoop(float dt) {
  for (auto& ls : loopSounds) {
    ls.timer += dt;

    if (ls.timer >= ls.duration) {
      SDL_PutAudioStreamData(ls.stream, ls.buffer, ls.length);
      SDL_FlushAudioStream(ls.stream);

      ls.timer = 0.0f;
    }
  }
}

