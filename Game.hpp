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
// monster
#include "monsters/ball.hpp"
#include "monsters/flower_plant.hpp"
#include "monsters/slime.hpp"

using json = nlohmann::json;

class Game {

private:

    enum seen {
        INTRO, STAGE, PAUSE, SKILL_SELECT, DIE
    };

    json Config;
    std::string FileToString(std::string FilePath);

    bool Debug;
    unsigned int Frame;
    std::string ConfigFilePath;
    input Input;

    seen Seen = INTRO, BeforeSeen = INTRO;

    map Map;
    player Player;
    bool Death = false;
    std::vector<arrow> Arrow;

    std::vector<monster *> Monster;
    std::vector<ball> Ball;

    std::vector<flower_plant> FlowerPlant;
    std::vector<slime> Slime;

    int Load();                 // 一番最初
    bool Update();              // マイフレーム return false:終了
    void Unload(bool Error);    // 最後 Error:エラーで終了したか

    bool Intro();
    bool Stage();
    bool Pause();
    bool SkillSelect();
    bool Die();
    
public:

    Game(bool Debug = false, std::string ConfigFilePath = "config.json");

};
