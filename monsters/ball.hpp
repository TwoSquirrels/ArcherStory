#pragma once
#include "DxLib.h"
#include "../ExternalHeaderFiles/json.hpp"
#include "../sprite.hpp"
#include "../map.hpp"
#include "../player.hpp"

class map;
class player;

using json = nlohmann::json;

class ball {

public:

    enum type {
        STONE, JUMP, BEAM
    };

    enum error {
        WRONG_CONSTRUCTOR
    };

private:

    json Config;

    enum type Type;
    int Attack;
    map *Map;
    player *Player;
    sprite Sprite;
    pos PlayerCenterPos{};

    int Count = 0;
    // ジャンプ系
    double Jump_a = 0;   // y=ax+h の a
    double Jump_CenterX = 0;
    sprite Shadow;

    std::map<std::string, int> Graph;
    std::map<std::string, pos> GraphSize;

public:

    bool Use;

    void Update();

    void Draw(int Scroll);

    ball();
    // STONE, BEAM
    ball(enum type Type, int Attack, map *Map, player *Player, pos Pos, double Direction, std::map<std::string, int> Graph, json Config);
    // JUMP
    ball(enum type Type, int Attack, map *Map, player *Player, pos Pos, pos PlayerCenterPos, std::map<std::string, int> Graph, json Config);

};
