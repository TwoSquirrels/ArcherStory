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
    bool *Death;

    struct {
        bool UseFlag = false;
        pos Pos{};
        pos Stick{};
        int Size = 64;
    } Joystick;

    int HP, MaxHP;
    int GodTime = 0, GodTimeMax;    // –³“GŽžŠÔ

    double Speed = 7.0;
    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        true,   // pond
    };

    void Move();
    void KeyInput(pos *InputDirection);
    void JoystickInput(pos *InputDirection);

public:

    pos StartPos{ 48.0 * 12 + 8.0, 48.0 * 6 + 8.0 };
    sprite Sprite;

    void Update();

    void Draw();
    void JoystickDraw();

    int GetHP();
    void Heel(int AddHP);
    void Damage(int Damage);

    bool CheckHit(sprite Sprite);

    player();
    player(input *Input, map *Map, bool *Death, json Config);

};
