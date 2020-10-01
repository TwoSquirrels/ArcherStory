#pragma once
#include "DxLib.h"
#include "../ExternalHeaderFiles/json.hpp"
#include "../monster.hpp"
#include "../map.hpp"
#include "../player.hpp"

class map;
class monster;

using json = nlohmann::json;

class slime {

private:

    map *Map;
    player *Player;
    json Config;
    
    int Level;

    int MoveCount = 0;

    std::map<std::string, int> Graph;
    std::map<std::string, pos> GraphSize;

public:

    monster *Monster;

    std::vector<slime> Update();

    void Draw(int Scroll);

    slime();
    slime(pos Pos, int HP, int Level, map *Map, player *Player, std::map<std::string, int> Graph, json Config);

};
