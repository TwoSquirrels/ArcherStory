#pragma once
#include "DxLib.h"
#include "ExternalHeaderFiles/json.hpp"
#include "sprite.hpp"
#include "map.hpp"
#include "player.hpp"
#include "monster.hpp"

class map;
class monster;

using json = nlohmann::json;

class arrow {

private:

    json Config;

    sprite Sprite;

    map *Map;
    std::vector<monster *> *Monster;
    
    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        false,  // pond
    };

    int Attack;

    std::map<std::string, int> Graph;
    std::map<std::string, pos> GraphSize;

public:

    bool Use;

    void Update();

    void Draw(int Scroll);

    arrow();
    arrow(pos Pos, double Direction, map *Map, std::vector<monster *> *Monster, int Attack, std::map<std::string, int> Graph, json Config);

};
