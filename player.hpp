#pragma once
#include "DxLib.h"
#include <string>
#include <map>
#include <cmath>
#include "sprite.hpp"
#include "input.hpp"

class player {

private:

    sprite Sprite;
    std::map<std::string, int> Img;
    std::map<std::string, pos> ImgSize;

    input *Input;

public:

    void Move();

    void Draw();

    player(input *Input);

};
