#pragma once
#include "DxLib.h"
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include "ExternalHeaderFiles/json.hpp"
#include "sprite.hpp"
#include "input.hpp"
#include "map.hpp"
#include "arrow.hpp"

class arrow;
class monster;

using json = nlohmann::json;

class player {

private:

    json Config;

    std::map<std::string, int> Img;
    std::map<std::string, pos> ImgSize;

    input *Input;
    map *Map;
    std::vector<arrow> *Arrow;
    bool *Death;
    std::vector<monster *> *Monster;

    struct {
        bool UseFlag = false;
        pos Pos{};
        pos Stick{};
        int Size = 64;
    } Joystick;

    int HP, MaxHP;
    int GodTime = 0, GodTimeMax;
    int AttackCooldown, AttackCooldownMax;
    int Attack;
    double Speed;
    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        true,   // pond
    };

    enum skill {
        HEAL, HP_MAX_UP, ATTACK_SPEED_UP, ATTACK_UP, DEFENSE_UP, GOD_TIME_EXTENSION, WALK_SPEED_UP,
        HEART_DROPRATE_UP, DIAGONAL_ARROW, FRONT_ARROW, SIDE_ARROW, BACK_ARROW, MULTI_SHOT, BOUNCE, BOUND_WALL, POISON, HEADSHOT,
    SKILL_NUM };
    std::vector<int> SkillCounter{SKILL_NUM};
    std::vector<int> SkillCounterMax = {

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

    enum shape {
        SQUARE, CIRCLE
    };
    bool CheckHit(sprite Sprite, enum shape Shape = SQUARE);

    player();
    player(input *Input, map *Map, std::vector<arrow> *Arrow, bool *Death, std::vector<monster *> *Monster, json Config);

};
