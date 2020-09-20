#pragma once
#include "DxLib.h"
#include <string>
#include <fstream>
#include "strconv.h"
#include "ExternalHeaderFiles/json.hpp"
#include "input.hpp"
#include "pos.hpp"
#include "player.hpp"
#include "map.hpp"

using json = nlohmann::json;

class Game {

private:

    json Config;
    std::string FileToString(std::string FilePath);

    bool Debug;
    unsigned int Frame;
    std::string ConfigFilePath;
    input Input;

    map Map;
    player Player;

    int Load();                 // 一番最初
    bool Update();              // マイフレーム return false:終了
    void Unload(bool Error);    // 最後 Error:エラーで終了したか
    
public:

    Game(bool Debug = false, std::string ConfigFilePath = "config.json");

};

