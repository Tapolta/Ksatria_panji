#include "../include/class/Player.h"

Player::Player(SDL_Renderer* renderer, float moveX) : GameObject(renderer)
{
  this->renderer = renderer;
  move_distance = moveX;

  running = new Sound();

  running->playSoundLoop("assets/sound/running.wav");
}

Player::~Player() {
  if (running) {
    delete running;
  }
}

void Player::success() {
  d_object.x += move_distance;
  
}

void Player::failed() {
  d_object.x -= move_distance;
}

void Player::renderFrames(float dt) {
  GameObject::renderFrames(dt);
}

void Player::update(float dt) {
  GameObject::update(dt);
  if (running) running->updateLoop(dt);
}
