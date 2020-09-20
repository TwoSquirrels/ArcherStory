#pragma once
#include "DxLib.h"
#include <string>
#include <map>
#include <cmath>
#include "ExternalHeaderFiles/json.hpp"
#include "sprite.hpp"
#include "input.hpp"
#include "map.hpp"

using json = nlohmann::json;

class player {

private:

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

    double Speed = 7.0;
    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        true,   // pond
    };

    void KeyInput(pos *InputDirection);
    void JoystickInput(pos *InputDirection);

public:

    pos StartPos{ 48.0 * 12 + 8.0, 48.0 * 6 + 8.0 };
    sprite Sprite;

    void Move();

    void Draw();
    void JoystickDraw();

    player();
    player(input *Input, map *Map, json Config);

};
