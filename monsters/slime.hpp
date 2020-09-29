#pragma once
#include "DxLib.h"
#include "../ExternalHeaderFiles/json.hpp"
#include "../monster.hpp"
#include "../player.hpp"

using json = nlohmann::json;

class slime {

private:

    player *Player;
    std::vector<slime> *Slime;
    json Config;
    
    int Level;

    int MoveCount = 0;

public:

    monster Monster;

    void Update(map Map);

    void Draw(int Scroll);

    slime();
    slime(pos Pos, int HP, int Level, player *Player, std::vector<slime> *Slime, json Config);

};
