#pragma once
#include "DxLib.h"
#include <string>
#include "input.hpp"
#include "pos.hpp"
#include "player.hpp"
#include "map.hpp"

class Game {

private:

    bool Debug;
    unsigned int Frame;
    std::string ConfigFilePath;
    input Input{};

    map Map{};
    player Player{&Input, &Map};

    void Load();                // 一番最初
    bool Update();              // マイフレーム return false:終了
    void Unload(bool Error);    // 最後 Error:エラーで終了したか
    
public:

    Game(bool Debug = false, std::string ConfigFilePath = "config.ini");

};

