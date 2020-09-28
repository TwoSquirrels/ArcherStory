#pragma once
#include "DxLib.h"
#include "../ExternalHeaderFiles/json.hpp"
#include "../monster.hpp"
#include "../player.hpp"

using json = nlohmann::json;

class flower_plant {

private:

    int Attack;
    player *Player;
    std::vector<ball>* Ball;
    json BallConfig;

public:

    monster Monster;

    void Update(map Map);

    void Draw(int Scroll);

    flower_plant();
    flower_plant(std::vector<ball> *Ball, pos Pos, int HP, int Attack, player *Player, json Config);

};
