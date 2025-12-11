#pragma once
#include "DxLib.h"
#include "../ExternalHeaderFiles/json.hpp"
#include "../monster.hpp"
#include "../map.hpp"
#include "../player.hpp"

class map;
class monster;

using json = nlohmann::json;

class tree {

private:

    std::vector<ball> *Ball;
    map *Map;
    player *Player;
    json Config;
    
    int Attack;

    int MoveCount = 0;

    std::map<std::string, int> Graph;
    std::map<std::string, pos> GraphSize;

public:

    monster *Monster;

    void Update();

    void Draw(int Scroll);

    tree();
    tree(std::vector<ball> *Ball, pos Pos, int HP, int Attack, map *Map, player *Player, std::map<std::string, int> Graph, json Config);

};
