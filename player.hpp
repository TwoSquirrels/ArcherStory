#pragma once
#include "DxLib.h"
#include <string>
#include <map>
#include "sprite.hpp"
#include "input.hpp"

class player {

private:

    sprite Sprite;
    std::map<std::string, int> Img;

    input *Input;

public:

    void Move();

    void Draw();

    player(input *Input);

};
