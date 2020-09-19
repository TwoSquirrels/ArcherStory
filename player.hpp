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

    struct {
        bool UseFlag = false;
        pos Pos{};
        pos Stick{};
        int Size = 64;
    } Joystick;

    double Speed = 4.0;
    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        true,   // pond
    };

    void KeyInput(pos *InputDirection);
    void JoystickInput(pos *InputDirection);

public:

    void Move();

    void Draw();
    void JoystickDraw();

    player(input *Input, map *Map);

};
