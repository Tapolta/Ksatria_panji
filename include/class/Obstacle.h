#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "GameObject.h"

class Obstacle : public GameObject {
  private:
    float speed = 300.0f;
    char main_char;

    void movement(float dt);
  
  public:
    Obstacle(SDL_Renderer* renderer);
    void update(float dt) override;
};

#endif