#pragma once
#include "DxLib.h"
#include "../ExternalHeaderFiles/json.hpp"
#include "../monster.hpp"
#include "../map.hpp"
#include "../player.hpp"
#include "ball.hpp"

class map;
class monster;
class ball;

using json = nlohmann::json;

class bat {

private:

    std::vector<ball> *Ball;
    map *Map;
    player *Player;
    json Config;
    
    int MoveCount = 0;

    int Attack;

    std::map<std::string, int> Graph;
    std::map<std::string, pos> GraphSize;

public:

    monster *Monster;

    void Update();

    void Draw(int Scroll);

    bat();
    bat(std::vector<ball> *Ball, pos Pos, int HP, int Attack, map *Map, player *Player, std::map<std::string, int> Graph, json Config);

};
