#pragma once
#include "DxLib.h"
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include "sprite.hpp"

class map {

private:

    std::vector<std::vector<int>> Map{13};

    std::map<std::string, int> Graph;

public:

    enum block {
        AIR, WALL, STONE, POND
    };

    enum direction {
        UP, DOWN, LEFT, RIGHT
    };

    void LoadFromFile(std::string FIlePath);

    std::vector<bool> Collision(sprite *Sprite, std::vector<bool> Block);
    std::vector<int> GetSidePos();
    bool GetInMap(sprite Sprite);

    void Draw(int Scroll);

    map();
    map(std::map<std::string, int> Graph);

};
