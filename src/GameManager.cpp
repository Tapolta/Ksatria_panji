#include "../include/class/GameManager.h"
#include <algorithm>
#include <iostream>

GameManager::GameManager(SDL_Renderer* renderer, WindowStruct* s_window) {
  this->s_window = s_window;
  this->renderer = renderer;

  phase = MAX_PHASE;

  success_sound = new Sound();
  failed_sound = new Sound();

  game_over = new GameObject(renderer);
  game_over->addText("Permainan Berakhir");
  game_over->setObjectD({0, 0, s_window->height, s_window->width});
  game_over->getText()->setFontSize(72);

  teks_jarak = new GameObject(renderer);
  teks_jarak->addText("Jarak: 0");
  teks_jarak->setObjectD({0,0,1.0f,1.0f});
  addGameObject(teks_jarak);
}

GameManager::~GameManager() {
  if (game_over) delete game_over;
  if (teks_jarak) delete teks_jarak;
  if (success_sound) delete success_sound;
  if (failed_sound) delete failed_sound;
  if(active_obstacle) delete active_obstacle;
}

void GameManager::update(float dt) 
{
  if (is_game_over) {
    game_over->update(dt);
    return;
  }

  for (GameObject* obj : go_list) if (obj) obj->update(dt);

  if (!ground_list.empty()) groundMove(dt);

  updateJarak(dt);
  spawnObstacle(dt);
  removeObstacle();
  spawnPohon(dt);
  removePohon();
  inputZone();
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
  for (auto i : ground_list) delete i;
  ground_list.clear();
}

void GameManager::spawnObstacle(float dt) {
  timer_obstacle += dt;

  if (timer_obstacle >= next_obstacle_time) {
    timer_obstacle = 0.0f;

    next_obstacle_time = randomRange(1.0f, 5.0f);

    Obstacle* obs = new Obstacle(renderer);

    SDL_FRect pos = {s_window->width, s_window->height - 350.0f, 100, 100};
    obs->setObjectD(pos);

    obstacle_list.push_back(obs);
    go_list.push_back(obs);
  }
}

float GameManager::randomRange(float min, float max) {
  return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

void GameManager::removeObstacle() {
  auto it = obstacle_list.begin();
  while (it != obstacle_list.end()) {
    Obstacle* obs = *it;

    if (obs->getObjectD().x < -obs->getObjectD().w ||
      obs->getObjectD().x < player->getObjectD().x)
    {
      go_list.erase(
        std::remove(go_list.begin(), go_list.end(), obs),
        go_list.end()
      );

      playerFailed();

      delete obs;
      it = obstacle_list.erase(it);
    } else {
      ++it;
    }
  }
}

void GameManager::setPlayer(Player* player) {
  this->player = player;
}

void GameManager::playerFailed() {
  if (player && phase > MIN_PHASE) {
    player->failed();
    phase--;
  }
  
  failed_sound->playSound("assets/sound/damage.wav");
  is_game_over = phase <= MIN_PHASE;
}

void GameManager::playerSuccess() {
  if (player && phase < MAX_PHASE) {
    player->success();
    phase++;
  }
  
  success_sound->playSound("assets/sound/success.wav");
}

void GameManager::GameOver(float dt) {
  game_over->update(dt);
}

void GameManager::updateJarak(float dt) {
  jarak_timer += dt;

  if (jarak_timer >= 0.5f) {
    jarak += 1; 
    jarak_timer = 0.0f;

    teks_jarak->addText("Jarak: " + std::to_string(jarak));
  }
}

void GameManager::handleInput(SDL_Event* event) {
  if (!active_obstacle) return;

  SDL_Keycode key = SDL_GetKeyFromScancode(event->key.scancode, SDL_KMOD_NONE, true);
  char c = (char)key;
  if (c >= 'A' && c <= 'Z') c = c + 32;
  if (c < 'a' || c > 'z') return;

  if (key == active_obstacle->getKey()) {
    playerSuccess();
  } else {
    playerFailed();
  }

  go_list.erase(
    std::remove(go_list.begin(), go_list.end(), active_obstacle),
    go_list.end()
  );

  obstacle_list.erase(
    std::remove(obstacle_list.begin(), obstacle_list.end(), active_obstacle),
    obstacle_list.end()
  );

  delete active_obstacle;
  active_obstacle = nullptr;
}

void GameManager::spawnPohon(float dt) {
  timer_pohon += dt;

  if (timer_pohon >= next_pohon_time) {
    timer_pohon = 0.0f;

    next_pohon_time = randomRange(3.0f, 7.0f);

    GameObject* pohon = new GameObject(renderer);

    pohon->setObjectD({
      s_window->width,             
      s_window->height - 600.0f,
      400.0f,
      600.0f
    });

    pohon->setImgPaths({
      "assets/pohon/pohon.png"
    });

    pohon_list.push_back(pohon);
    go_list.push_back(pohon);
  }

  for (auto pohon : pohon_list) {
    pohon->setObjectD({
      pohon->getObjectD().x - (ground_speed * dt),
      pohon->getObjectD().y,
      pohon->getObjectD().w,
      pohon->getObjectD().h,
    });
  }
}

void GameManager::removePohon() {
  auto it = pohon_list.begin();

  while (it != pohon_list.end()) {
    GameObject* pohon = *it;

    if (pohon->getObjectD().x < -pohon->getObjectD().w) {
      go_list.erase(
        std::remove(go_list.begin(), go_list.end(), pohon),
        go_list.end()
      );

      delete pohon;
      it = pohon_list.erase(it);
    } else {
      ++it;
    }
  }
}

void GameManager::inputZone() {
  for (Obstacle* obs : obstacle_list) {
    if (obs->getObjectD().x < (player->getObjectD().x + 300.0f)) {
      obs->readyInput();
      active_obstacle = obs;
      return;
    }
  }

  active_obstacle = nullptr;
}
