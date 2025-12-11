#pragma once
#include "DxLib.h"
#include "../ExternalHeaderFiles/json.hpp"
#include "../monster.hpp"
#include "../map.hpp"
#include "../player.hpp"

class map;
class monster;

using json = nlohmann::json;

class virus {

private:

    map *Map;
    player *Player;
    
    int Level;

    int MoveCount = 0;

    std::map<std::string, int> Graph;
    std::map<std::string, pos> GraphSize;

public:

    monster *Monster;

    std::vector<virus> Update();

    void Draw(int Scroll);

    virus();
    virus(pos Pos, int HP, int Level, map *Map, player *Player, std::map<std::string, int> Graph);

};
