#pragma once
#include "DxLib.h"
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include "ExternalHeaderFiles/json.hpp"
#include "sprite.hpp"
#include "monsters/flower_plant.hpp"
#include "monsters/slime.hpp"

class flower_plant;
class slime;
class ball;

using json = nlohmann::json;

class map {

private:

    json Config;
    std::vector<ball> *Ball;
    player *Player;

    json Maps;
    std::vector<std::vector<int>> Map{13};

    std::vector<flower_plant> *FlowerPlant;
    std::vector<slime> *Slime;
    enum monsters {
        FLOWER_PLANT, SLIME,
    MONSTERS_NUM };

    int Stage = 0;

    std::map<std::string, std::map<std::string, int>> Graph;

    int ClearCount = 0;

public:

    enum block {
        AIR, WALL, STONE, POND
    };

    enum direction {
        UP, DOWN, LEFT, RIGHT
    };

    void LoadFromFile(std::string FIlePath);

    std::vector<bool> Collision(sprite *Sprite, std::vector<bool> Block);
    std::vector<int> GetSidePos();
    bool GetInMap(sprite Sprite);

    void Draw(int Scroll);

    int GetStage();

    void Clear();
    void NextStage();

    map();
    map(json Maps, std::map<std::string, std::map<std::string, int>> Graph, std::vector<flower_plant> *FlowerPlant, std::vector<slime> *Slime, std::vector<ball> *Ball, player *Player, json Config);

};
