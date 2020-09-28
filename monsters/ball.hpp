#pragma once
#include "DxLib.h"
#include "../ExternalHeaderFiles/json.hpp"
#include "../sprite.hpp"
#include "../map.hpp"
#include "../player.hpp"

class player;

using json = nlohmann::json;

class ball {

public:

    enum type {
        TEST, JUMP
    };

private:

    json Config;

    enum type Type;
    int Attack;
    player *Player;
    sprite Sprite;
    pos PlayerCenterPos{};
    // ƒWƒƒƒ“ƒvŒn
    double Jump_a = 0;   // y=ax+h ‚Ì a
    double Jump_CenterX = 0;
    sprite Shadow;

public:

    bool Use;

    void Update(map Map);

    void Draw(int Scroll);

    ball();
    // TEST, JUMP
    ball(enum type Type, int Attack, player *Player, pos Pos, pos PlayerCenterPos, json Config);

};
