#pragma once
#include "DxLib.h"
#include <string>
#include <map>
#include <cmath>
#include "sprite.hpp"
#include "input.hpp"
#include "map.hpp"

class player {

private:

    sprite Sprite;
    std::map<std::string, int> Img;
    std::map<std::string, pos> ImgSize;

    input *Input;
    map *Map;

    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        true,   // pond
    };

public:

    void Move();

    void Draw();

    player(input *Input, map *Map);

};
