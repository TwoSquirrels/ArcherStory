#pragma once
#include "DxLib.h"
#include <vector>
#include <string>
#include <fstream>
#include "ExternalHeaderFiles/strconv.h"
#include "ExternalHeaderFiles/json.hpp"
#include "input.hpp"
#include "pos.hpp"
#include "player.hpp"
#include "arrow.hpp"
#include "map.hpp"
// monsters
#include "monsters/ball.hpp"
#include "monsters/flower_plant.hpp"
#include "monsters/slime.hpp"
#include "monsters/golem.hpp"
#include "monsters/bat.hpp"
#include "monsters/tree.hpp"
#include "monsters/virus.hpp"

using json = nlohmann::json;

class Game {

private:

    enum scene {
        INTRO, STAGE, PAUSE, SKILL_SELECT, DIE  // ポーズは未使用
    };

    json Config;
    std::string FileToString(std::string FilePath);

    bool Debug;
    int Frame;
    std::string ConfigFilePath;
    input Input;

    scene Scene = INTRO;

    map Map;
    player Player;
    bool Death = false;
    bool Next = false;
    std::vector<arrow> Arrow;

    int ClearCount = 0;
    std::vector<monster *> Monster;
    std::vector<ball> Ball;

    std::vector<flower_plant> FlowerPlant;
    std::vector<slime> Slime;
    std::vector<golem> Golem;
    std::vector<bat> Bat;
    std::vector<tree> Tree;
    std::vector<virus> Virus;

    int Load();                 // 一番最初
    bool Update();              // マイフレーム return false:終了
    void Unload(bool Error);    // 最後 Error:エラーで終了したか

    int StartGraph;
    bool Intro();
    int BeforeIntroFrame = 0;
    bool Stage();
    int FadeOutCount = 0;
    bool Pause();
    bool SkillSelect();
    int EndGraph;
    bool Die();
    
public:

    Game(bool Debug = false, std::string ConfigFilePath = "config.json");

};
