#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameObject.h"
#include "Obstacle.h"
#include <vector>
#include "../struct/WindowStruct.h"
#include "Player.h"
#include "Sound.h"

class GameManager {
  private:
    WindowStruct* s_window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Player* player = nullptr;
    GameObject* game_over;
    GameObject* teks_jarak;
    Obstacle* active_obstacle = nullptr;
    Sound* failed_sound = nullptr;
    Sound* success_sound = nullptr;
    std::vector<GameObject*> go_list;
    std::vector<GameObject*> ground_list;
    std::vector<Obstacle*> obstacle_list;
    std::vector<GameObject*> pohon_list;
    const int MAX_GROUND = 3, MAX_PHASE = 3, MIN_PHASE = 0;
    const float ground_speed = 300.0f;
    float timer_obstacle  = 0.0f, next_obstacle_time = 0.0f, jarak_timer = 0.0f, timer_pohon = 0.0f, 
      next_pohon_time = 3.0f;
    int phase, jarak = 0;
    bool is_game_over = false;

    void groundMove(float dt);
    void spawnObstacle(float dt);
    void removeObstacle();
    float randomRange(float min, float max);
    void updateJarak(float dt);

  public:
    GameManager(SDL_Renderer* renderer, WindowStruct* s_window);
    ~GameManager();
    void playerFailed();
    void update(float dt);
    void addGameObject(GameObject* go);
    void addGrounds(std::string path, SDL_FRect d_ground);
    void destroyGrounds();
    void setPlayer(Player* player);
    void playerSuccess();
    void GameOver(float dt);
    void handleInput(SDL_Event* event);
    void spawnPohon(float dt);
    void removePohon();
    void inputZone();
};

#endif