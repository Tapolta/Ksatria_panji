#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "GameObject.h"

class Obstacle : public GameObject {
  private:
    float speed = 300.0f;
    char key;

    void movement(float dt);
    void setBackground();
    char randomChar();
  
  public:
    Obstacle(SDL_Renderer* renderer);
    void update(float dt) override;
    void setTextPos();
    void readyInput();
    char getKey();
};

#endif