#include "../include/class/Obstacle.h"

Obstacle::Obstacle(SDL_Renderer* renderer) : GameObject(renderer) {
  this->renderer = renderer;
}

void Obstacle::update(float dt) {
  GameObject::update(dt);
  movement(dt);
}

void Obstacle::movement(float dt) {
  d_object.x -= speed * dt;
}