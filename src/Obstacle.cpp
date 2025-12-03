#include "../include/class/Obstacle.h"

Obstacle::Obstacle(SDL_Renderer* renderer) : GameObject(renderer) {
  this->addText("");
  key = randomChar();
  this->text->setText(std::string(1, key));
  this->text->setFontSize(60);
  speed = 400.0f;

  setBackground();
}

void Obstacle::update(float dt) {
  GameObject::update(dt);
  movement(dt);
  setTextPos();
}

void Obstacle::movement(float dt) {
  d_object.x -= speed * dt;
}

void Obstacle::setBackground() {
  addBackground({0, 0, 0, 255});
}

void Obstacle::setTextPos() {
  this->text->settextD({
    d_object.x + (d_object.w / 2) - (this->text->getDText().w / 2),
    d_object.y + (d_object.h / 2) - (this->text->getDText().h / 2),
    d_object.w,
    d_object.h
  });
}

char Obstacle::randomChar() {
  return 'a' + (rand() % 26);
}

void Obstacle::readyInput() {
  addBackground({178, 222, 39, 255});
}

char Obstacle::getKey() {
  return key;
}