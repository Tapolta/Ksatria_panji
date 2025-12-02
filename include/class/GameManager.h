#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameObject.h"
#include "Obstacle.h"
#include <vector>
#include "../struct/WindowStruct.h"

class GameManager {
  private:
    WindowStruct* s_window;
    SDL_Renderer* renderer;
    std::vector<GameObject*> go_list;
    std::vector<GameObject*> ground_list;
    std::vector<Obstacle*> obstacle_list;
    const int MAX_GROUND = 3;
    const float ground_speed = 300.0f;
    float timerObstacle  = 0.0f, nextObstacleTime = 0.0f;

    void groundMove(float dt);
    void spawnObstacle(float dt);
    void removeObstacle();
    float randomRange(float min, float max);

  public:
    GameManager(SDL_Renderer* renderer, WindowStruct* s_window);
    void update(float dt);
    void addGameObject(GameObject* go);
    void addGrounds(std::string path, SDL_FRect d_ground);
    void destroyGrounds();
};

#endif