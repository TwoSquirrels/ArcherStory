#pragma once
#include "DxLib.h"
#include "../ExternalHeaderFiles/json.hpp"
#include "../monster.hpp"
#include "../map.hpp"
#include "../player.hpp"
#include "ball.hpp"

class map;
class ball;
class monster;

using json = nlohmann::json;

class flower_plant {

private:

    int Attack;
    map *Map;
    player *Player;
    std::vector<ball>* Ball;
    json Config;

    std::map<std::string, int> Graph;
    std::map<std::string, pos> GraphSize;

public:

    monster *Monster;

    void Update();

    void Draw(int Scroll);

    flower_plant();
    flower_plant(std::vector<ball> *Ball, pos Pos, int HP, int Attack, map *Map, player *Player, std::map<std::string, int> Graph, json Config);

};
