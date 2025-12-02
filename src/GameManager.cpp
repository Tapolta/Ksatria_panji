#include "../include/class/GameManager.h"
#include <algorithm>

GameManager::GameManager(SDL_Renderer* renderer, WindowStruct* s_window) {
  this->s_window = s_window;
  this->renderer = renderer;
}

void GameManager::update(float dt) {
  for (auto i : go_list) i->update(dt);

  if (!ground_list.empty()) groundMove(dt);
  spawnObstacle(dt);
  removeObstacle();
}

void GameManager::addGameObject(GameObject* go) {
  go_list.push_back(go);
}

void GameManager::addGrounds(std::string path, SDL_FRect d_ground) {
  for (int i = 0; i < MAX_GROUND; ++i) {
    GameObject* go = new GameObject(renderer);
    addGameObject(go);
    go->setImgPaths({
      path
    });
    go->setObjectD({
      d_ground.x + (d_ground.w * i),
      d_ground.y,
      d_ground.w,
      d_ground.h,
    });
    ground_list.push_back(go);
  }
}

void GameManager::groundMove(float dt) {
  GameObject* rightMostGround = ground_list[0];

  for (auto g : ground_list) {
    if (g->getObjectD().x > rightMostGround->getObjectD().x) {
      rightMostGround = g;
    }
  }

  for (auto i : ground_list) {
    SDL_FRect d = i->getObjectD();
    d.x -= ground_speed * dt;
    i->setObjectD(d);

    if (d.x < -d.w) {
      SDL_FRect newD = d;
      newD.x = rightMostGround->getObjectD().x + rightMostGround->getObjectD().w - 1.0f;
      i->setObjectD(newD);

      rightMostGround = i;
    }
  }
}

void GameManager::destroyGrounds() {
  for (auto i : ground_list) {
    delete i;
  }

  ground_list.clear();
}

void GameManager::spawnObstacle(float dt) {
  timerObstacle += dt;

  if (timerObstacle >= nextObstacleTime) {
    timerObstacle = 0.0f;

    nextObstacleTime = randomRange(1.0f, 5.0f);

    Obstacle* obs = new Obstacle(renderer);
    obs->addBackground({0,0,0,255});

    SDL_FRect pos = { s_window->width, s_window->height - 350.0f, 100, 100 };
    obs->setObjectD(pos);

    obstacle_list.push_back(obs);
    go_list.push_back(obs);
  }
}

float GameManager::randomRange(float min, float max) {
  return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}


void GameManager::removeObstacle() {
  obstacle_list.erase(
    std::remove_if(obstacle_list.begin(), obstacle_list.end(),
    [&](Obstacle* obs) {
      if (obs->getObjectD().x < -obs->getObjectD().w) {
        go_list.erase(
          std::remove(go_list.begin(), go_list.end(), obs),
          go_list.end()
        );

        delete obs;
        return true;
      }
      return false;
    }),
    obstacle_list.end()
  );
}
