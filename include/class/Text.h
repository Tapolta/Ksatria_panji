#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Text {
  private:
    const std::string fontPath = "assets/font/LoveDays-2v7Oe.ttf";
    std::string mainText;
    int fontSize = 24;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_FRect d_text;
    SDL_Texture* textTexture;
    TTF_Font* font;
    SDL_Renderer* renderer;
  
    void render();
  
  public:
    Text(SDL_Renderer* renderer);
    ~Text();
    void update();
    void setText(std::string text);
    void setTextColor(SDL_Color color);
    void setFontSize(float textSize);
    void settextD(SDL_FRect d_text);
    SDL_FRect getDText();
};

#endif