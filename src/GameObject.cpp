#include "../include/class/GameObject.h"
#include <SDL3_image/SDL_image.h>

GameObject::GameObject(SDL_Renderer* renderer) {
  this->renderer = renderer;
}

// GameObject::~GameObject() {
//   if (!img_list.empty()) removeImgList();
//   if (texture_bg) removeBackground();
//   delete bg_color;
// }

void GameObject::update(float dt) {
  if (bg_color) renderBg();
  if (!img_list.empty()) renderFrames(dt);
}

void GameObject::setObjectD(SDL_FRect new_d) {
  d_object = new_d;
}

SDL_FRect GameObject::getObjectD() {
  return d_object;
}

void GameObject::addBackground(SDL_Color color) {
  if (!this->bg_color) this->bg_color = new SDL_Color();
  *this->bg_color = color;
}

void GameObject::renderBg() {
  const SDL_FRect rect = d_object;

  SDL_SetRenderDrawColor(
    renderer, 
    bg_color->r,
    bg_color->g,
    bg_color->b,
    bg_color->a
  );

  SDL_RenderFillRect(renderer, &rect);
}

void GameObject::removeBackground() {
  if (!bg_color) return; 
  delete bg_color;
  bg_color = nullptr;
}

void GameObject::setImgPaths(std::vector<std::string> imageFrames) {
  for (auto i : imageFrames) {
    SDL_Texture* texture = IMG_LoadTexture(this->renderer, i.c_str());
    if (!texture) {
      SDL_Log("Cannot get image: %s", SDL_GetError());
    } else {
      SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
      img_list.push_back(texture);
    }
  }

  if (!img_list.empty()) {
    SDL_GetTextureSize(img_list[0], &tw, &th);
    this->d_object.w = tw;
    this->d_object.h = th;
  }
}

void GameObject::renderFrames(float dt) {
  if (img_list.size() > 1) {
    frameTimer += dt;
    if (frameTimer >= frameDuration) {
      frameTimer = 0.0f;
      currentFrame++;
      if (currentFrame >= img_list.size()) currentFrame = 0;
    }
  }

  const SDL_FRect rect = d_object;
  SDL_RenderTexture(renderer, img_list[currentFrame], NULL, &rect);
}

void GameObject::removeImgList() {
  for (auto texture : img_list) {
    if (texture) SDL_DestroyTexture(texture);
  }

  img_list.clear();
}