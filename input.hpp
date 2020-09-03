#pragma once
#include "DxLib.h"
#include <vector>
#include "pos.hpp"

class input {

private:

    // キーボード //
    char Key[256];

    // マウス //
    pos MousePos;
    int MouseInput;

    // タッチパネル//


    // ジョイパッド //


public:

    void Update();

    bool GetKey(char KeyCode);
    pos GetMousePos();
    bool GetMouseInput(short MouseCode);

    input();

};

