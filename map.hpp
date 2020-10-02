#pragma once
#include "DxLib.h"
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include "ExternalHeaderFiles/json.hpp"
#include "ExternalHeaderFiles/strconv.h"
#include "sprite.hpp"
// monsters
#include "monsters/flower_plant.hpp"
#include "monsters/slime.hpp"
#include "monsters/golem.hpp"
#include "monsters/bat.hpp"
#include "monsters/tree.hpp"
#include "monsters/virus.hpp"

class ball;
class flower_plant;
class slime;
class golem;
class bat;
class tree;
class virus;

using json = nlohmann::json;

class map {

private:

    json Config;
    std::vector<ball> *Ball;
    player *Player;

    json Maps;
    std::vector<std::vector<int>> Map{13};
    std::string Text;

    std::vector<flower_plant> *FlowerPlant;
    std::vector<slime> *Slime;
    std::vector<golem> *Golem;
    std::vector<bat> *Bat;
    std::vector<tree> *Tree;
    std::vector<virus> *Virus;
    enum monsters {
        FLOWER_PLANT, SLIME, GOLEM, BAT, TREE, VIRUS,
    MONSTERS_NUM };

    int Stage = 0;

    std::map<std::string, std::map<std::string, int>> Graph;
    std::map<std::string, int> Font;

    int ClearCount = 0;

public:

    enum block {
        AIR, WALL, STONE, POND
    };

    enum direction {
        UP, DOWN, LEFT, RIGHT
    };

    std::vector<bool> Collision(sprite *Sprite, std::vector<bool> Block);
    std::vector<int> GetSidePos();
    bool GetInMap(sprite Sprite);

    void Draw(int Scroll);

    int GetStage();

    void Clear();
    void ClearCancel();
    void NextStage();

    map();
    map(json Maps, std::map<std::string, std::map<std::string, int>> Graph, std::map<std::string, int> Font, std::vector<flower_plant> *FlowerPlant, std::vector<slime> *Slime, std::vector<golem> *Golem, std::vector<bat> *Bat, std::vector<tree> *Tree, std::vector<virus> *Virus, std::vector<ball> *Ball, player *Player, json Config);

};
