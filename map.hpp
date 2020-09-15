#pragma once
#include "DxLib.h"
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include "sprite.hpp"

#define MAP_AIR     0
#define MAP_WALL    1
#define MAP_STONE   2
#define MAP_POND    3

#define MAP_UP      0
#define MAP_DOWN    1
#define MAP_LEFT    2
#define MAP_RIGHT   3

class map {

private:

    std::vector<std::vector<int>> Map{13};

public:

    void LoadFromFile(std::string FIlePath);

    std::vector<bool> Collision(sprite *Sprite, std::vector<bool> Block);

    void Draw(int Scroll);

    map();

};
