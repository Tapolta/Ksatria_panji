#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Sound.h"

class Player : public GameObject {
  protected:
    void renderFrames(float dt) override;
    Sound* running = nullptr;
    Sound* failed_sound = nullptr;
    Sound* success_sound = nullptr;

  private:
    float move_distance;
  
  public:
    Player(SDL_Renderer* renderer, float moveX);
    ~Player();
    void update(float dt) override;
    void failed();
    void success();
};

#endif