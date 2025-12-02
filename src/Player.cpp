#include "../include/class/Player.h"

Player::Player(SDL_Renderer* renderer, float moveX) : GameObject(renderer)
{
  this->renderer = renderer;
  move_distance = moveX;

  running = new Sound();
  success_sound = new Sound();
  failed_sound = new Sound();

  running->playSoundLoop("assets/sound/running.wav");
}

Player::~Player() {
  if (running) {
    delete running;
  }

  if (success_sound) {
    delete success_sound;
  }

  if (failed_sound) {
    delete failed_sound;
  }
}

void Player::success() {
  d_object.x += move_distance;
  success_sound->playSound("assets/sound/success.wav");
}

void Player::failed() {
  d_object.x -= move_distance;
  failed_sound->playSound("assets/sound/damage.wav");
}

void Player::renderFrames(float dt) {
  GameObject::renderFrames(dt);
}

void Player::update(float dt) {
  GameObject::update(dt);
  if (running) running->updateLoop(dt);
}
