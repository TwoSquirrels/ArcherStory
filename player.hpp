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
class map;

using json = nlohmann::json;

class player {

public:

    enum skill {
        HEAL, HP_MAX_UP, ATTACK_SPEED_UP, ATTACK_UP, DEFENSE_UP, GOD_TIME_EXTENSION, WALK_SPEED_UP,
        DIAGONAL_ARROW, FRONT_ARROW, SIDE_ARROW, BACK_ARROW, MULTI_SHOT, PENETRATION, BOUND, POISON, HEADSHOT
    };

private:

    json Config;

    std::map<std::string, int> Graph;
    std::map<std::string, pos> GraphSize;
    std::map<std::string, int> Font;

    input *Input;
    map *Map;
    std::vector<arrow> *Arrow;
    bool *Death, *Next;
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
    int Defense = 0;
    double Speed;
    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        true,   // pond
    };

    std::map<skill, int> SkillLeft;
    std::map<skill, int> SkillMax = {
        { HEAL, -1 }, { HP_MAX_UP, -1 }, { ATTACK_SPEED_UP, 2 }, { ATTACK_UP, -1 }, { DEFENSE_UP, -1 }, { GOD_TIME_EXTENSION, 3 }, { WALK_SPEED_UP, 1 },
        { DIAGONAL_ARROW, 3 }, { FRONT_ARROW, 3 }, { SIDE_ARROW, 2 }, { BACK_ARROW, 2 }, { MULTI_SHOT, 2 }, { PENETRATION, 2 }, { BOUND, 2 }, { POISON, 1 }, { HEADSHOT, 2 }
    };
    int MultiShotCount = 0;

    void Move();
    void KeyInput(pos *InputDirection);

    std::map<skill, int> GetSkill();
    std::string SkillMessage = "";
    int SkillMessageCount = 0;
    std::string ErrorMessage = "";
    int ErrorMessageCount = 0;

public:

    pos StartPos{ 48.0 * 12 + 8.0, 48.0 * 6 + 8.0 };
    sprite Sprite;

    void JoystickInput(pos *InputDirection);
    void Update();

    void Draw();
    void JoystickDraw();

    int GetHP();
    int GetMaxHP();
    void Heal(int AddHP);
    void Damage(int Damage);

    std::map<skill, int> GetSkillLeft();
    bool GiveSkill(skill Skill);

    enum shape {
        SQUARE, CIRCLE
    };
    bool CheckHit(sprite Sprite, enum shape Shape = SQUARE);

    player();
    player(input *Input, map *Map, std::vector<arrow> *Arrow, bool *Death, bool *Next, std::vector<monster *> *Monster, std::map<std::string, int> Graph, std::map<std::string, int> Font, json Config);

};
