#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include "Text.h"

class GameObject {
  protected:
    SDL_FRect d_object;
    SDL_Renderer* renderer = nullptr;
    Text* text = nullptr;
    std::vector<SDL_Texture*> img_list;
    float frameTimer = 0.0f, frameDuration = 0.1f;
    int currentFrame = 0;

    virtual void renderFrames(float dt);

  private:
    SDL_Texture* texture_bg = nullptr;
    SDL_Color* bg_color = nullptr;
    float tw, th, texScale = 4.0f;

    void renderBg();
    void renderText(float dt);

  public:
    GameObject(SDL_Renderer* renderer);
    ~GameObject();
    virtual void update(float dt);
    void setObjectD(SDL_FRect new_d);
    SDL_FRect getObjectD();
    void setImgPaths(std::vector<std::string> imageFrames);
    void removeImgList();
    void addBackground(SDL_Color color);
    void removeBackground();
    void addText(std::string text);
    Text* getText();
};

#endif