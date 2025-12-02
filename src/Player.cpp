#include "../include/class/Player.h"

Player::Player(SDL_Renderer* renderer, float moveX) : GameObject(renderer) {
  this->renderer = renderer;
  move_distance= moveX;

  d_object.w = 0 + (moveX * 3);
}

void Player::success() {

}

void Player::failed() {
  
}

void Player::renderFrames(float dt) {
  GameObject::renderFrames(dt);
}

void Player::update(float dt) {
  GameObject::update(dt);
}