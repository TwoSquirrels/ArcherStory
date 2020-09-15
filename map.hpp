#pragma once
#include "DxLib.h"
#include <vector>
#include <string>
#include <map>
#include "sprite.hpp"

#define MAP_AIR     "air"
#define MAP_WALL    "wall"
#define MAP_STONE   "stone"
#define MAP_POND    "pond"

#define MAP_UP      0
#define MAP_DOWN    1
#define MAP_LEFT    2
#define MAP_RIGHT   3

class map {

private:

    std::vector<std::vector<int>> Map{11};

public:

    void LoadFromFile(std::string FIlePath);

    std::vector<bool> Collision(sprite *Sprite, std::map<std::string, bool> Block);

    void Draw(int Scroll);

    map();

};
