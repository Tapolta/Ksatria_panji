#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include "include/struct/WindowStruct.h"
#include "include/class/GameManager.h"
#include "include/class/Player.h"
#include "include/class/Obstacle.h"

bool libraryReady();
SDL_Window* createWindow(const WindowStruct& s_window);
SDL_Renderer* createRenderer(SDL_Window* window);
GameManager* game_manager;
void loop(SDL_Renderer* renderer);
void cleanup(SDL_Window* window, SDL_Renderer* renderer);
bool* pause;
bool* running;

int main(int argc, char** argv) {
  if (!libraryReady()) return 1;

  srand(static_cast<unsigned>(time(nullptr)));

  WindowStruct s_window = {"Screen1", 800.0f, 600.0f};

  SDL_Window* window = createWindow(s_window);
  if (!window) return 1;

  SDL_Renderer* renderer = createRenderer(window);
  if (!renderer) {
    SDL_DestroyWindow(window);
    return 1;
  }

  game_manager = new GameManager(renderer, &s_window);

  Player* player = new Player(renderer, s_window.width / 8);
  game_manager->addGameObject(player);
  player->setImgPaths({
    "assets/panji/RUN1.png",
    "assets/panji/RUN2.png",
    "assets/panji/RUN3.png",
    "assets/panji/RUN4.png",
    "assets/panji/RUN5.png",
    "assets/panji/RUN6.png",
    "assets/panji/RUN7.png",
  });
  player->setObjectD({100.0f, s_window.height - 320.0f, 300.0f, 300.0f});

  GameObject* enemy = new GameObject(renderer);
  game_manager->addGameObject(enemy);
  enemy->setImgPaths({
    "assets/musuh/Warrior_Run_1.png",
    "assets/musuh/Warrior_Run_2.png",
    "assets/musuh/Warrior_Run_3.png",
    "assets/musuh/Warrior_Run_4.png",
    "assets/musuh/Warrior_Run_5.png",
    "assets/musuh/Warrior_Run_6.png",
    "assets/musuh/Warrior_Run_7.png",
    "assets/musuh/Warrior_Run_8.png",
  });
  enemy->setObjectD({-55.0f, s_window.height - 300.0f, 220.0f, 220.0f});

  game_manager->addGrounds("assets/ground/grass.png", {
    (s_window.width / 2) - (s_window.width / 2),
    s_window.height - 150.0f,
    s_window.width,
    150.0f
  });

  loop(renderer);

  cleanup(window, renderer);

  return 0;
}

bool libraryReady() {
  bool isReady = SDL_Init(SDL_INIT_VIDEO) && TTF_Init();
  
  if (!isReady) {
    SDL_SetError("Failed to perfom SDL: Reason %s", SDL_GetError());
    return false;
  }

  return true;
}

SDL_Window* createWindow(const WindowStruct& s_window) {
  SDL_Window* window = SDL_CreateWindow(
    s_window.window_name.c_str(),
    s_window.width,
    s_window.height,
    SDL_WINDOW_BORDERLESS
  );

  if (!window) SDL_Log("Cannot create window: %s", SDL_GetError());
  return window;
}

SDL_Renderer* createRenderer(SDL_Window* window) {
  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

  if (!renderer) SDL_Log("Cannot create renderer: %s", SDL_GetError());
  return renderer;
}

void loop(SDL_Renderer* renderer) {
  uint64_t lastTime = SDL_GetTicksNS();
  float dt;
  running = new bool(true);
  pause = new bool(false);
  SDL_Event event;

  while (*running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        *running = false;
      }

      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          *running = false;
        }
      }
    }

    if (!*pause) {
      uint64_t currentTime = SDL_GetTicksNS();
      dt = (currentTime - lastTime) / 1e9f;
      lastTime = currentTime;
    } else {
      dt = 0;
    }

    SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
    SDL_RenderClear(renderer);

    game_manager->update(dt);

    SDL_RenderPresent(renderer);
  }

  delete running;
  delete pause;
}

void cleanup(SDL_Window* window, SDL_Renderer* renderer) {
  TTF_Quit();
  if (renderer) SDL_DestroyRenderer(renderer);
  if (window) SDL_DestroyWindow(window);
  SDL_Quit();
}