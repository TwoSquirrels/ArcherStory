#pragma once
#include "DxLib.h"
#include "ExternalHeaderFiles/json.hpp"
#include "sprite.hpp"
#include "map.hpp"
#include "player.hpp"
#include "monster.hpp"

class monster;

using json = nlohmann::json;

class arrow {

private:

    json Config;

    sprite Sprite;
    std::vector<monster *> *Monster;
    
    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        false,  // pond
    };

    int Attack;

public:

    bool Use;

    void Update(map Map);

    void Draw(int Scroll);

    arrow();
    arrow(pos Pos, double Direction, std::vector<monster *> *Monster, int Attack, json Config);

};
