#pragma once
#include "DxLib.h"
#include <vector>
#include "pos.hpp"

class input {

private:

    // キーボード //
    char Key[256];

    // マウス //
    pos MousePos{};
    int MouseInput;
    int BeforeMouseInput;
    // int MouseScroll

    // タッチパネル//


    // ジョイパッド //


public:

    void Update();

    bool GetKey(unsigned char KeyCode);
    pos GetMousePos();
    bool GetMouseInput(short MouseCode);
    bool GetMouseDown(short MouseCode);
    bool GetMouseUp(short MouseCode);

    input(bool Init = true);

};

