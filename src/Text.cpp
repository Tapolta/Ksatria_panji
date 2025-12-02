#include "../include/class/Text.h"

void Text::render() {
  if (!textTexture) return;
  SDL_GetTextureSize(this->textTexture, &d_text.w, &d_text.h);
  SDL_RenderTexture(this->renderer, textTexture, nullptr, &d_text);
}

Text::Text(SDL_Renderer* renderer) {
  this->renderer = renderer;

  this->font = TTF_OpenFont(this->fontPath.c_str(), this->fontSize);
  if (!font) {
    SDL_Log( "Failed to Open Font: %s", SDL_GetError());
    return;
  }
}

Text::~Text() {
  if (textTexture) SDL_DestroyTexture(textTexture);
  TTF_CloseFont(this->font);
}

void Text::update() {
  render();
}

void Text::setText(std::string text) {
  this->mainText = text;

  if (this->textTexture) SDL_DestroyTexture(this->textTexture);

  SDL_Surface* textSurface = TTF_RenderText_Blended(
    this->font, 
    this->mainText.c_str(), 
    SDL_strlen(this->mainText.c_str()), 
    textColor
  );
  if (!textSurface) {
   SDL_Log("Render text error: %S", SDL_GetError());
    return;
  }

  this->textTexture = SDL_CreateTextureFromSurface(this->renderer, textSurface);
  SDL_DestroySurface(textSurface);
}

void Text::setFontSize(float size) {
  if (size <= 0) return;
  TTF_CloseFont(this->font);
  this->fontSize = size;
  this->font = TTF_OpenFont(this->fontPath.c_str(), this->fontSize);
  setText(this->mainText);
}

void Text::setTextColor(SDL_Color color) {
  this->textColor = color;
}

void Text::settextD(SDL_FRect d_rect) {
  d_text = d_rect;
}

SDL_FRect Text::getDText() {
 return d_text; 
}
