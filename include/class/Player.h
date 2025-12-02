#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject {
  protected:
    void renderFrames(float dt) override;

  private:
    float move_distance;
  
  public:
    Player(SDL_Renderer* renderer, float moveX);
    void update(float dt) override;
    void failed();
    void success();
};

#endif