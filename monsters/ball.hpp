#pragma once
#include "DxLib.h"
#include "../sprite.hpp"

class ball {

private:

    int Type;
    sprite Sprite;
    bool Use;

public:

    void Update() {
        if (this->Use) {
            // ˆÚ“®
            this->Sprite.Pos.AddPos(this->Sprite.Motion.GetX(), this->Sprite.Motion.GetY());
        }
    }

    void Draw(int Scroll) {
        if (this->Use) {
            DxLib::DrawCircle(
                -Scroll + this->Sprite.Pos.GetXInt() + this->Sprite.Size.GetXInt() / 2, 96 + this->Sprite.Pos.GetYInt() + this->Sprite.Size.GetYInt() / 2,
                4, 0xff0000, TRUE
            );
        }
    }

    ball() {
        this->Use = false;
    }
    ball(int Type, pos Pos, pos Motion) {

        this->Use = true;
        this->Type = Type;
        this->Sprite.Pos = Pos;
        this->Sprite.Motion = Motion;
        this->Sprite.Size.SetPos(8, 8);

    }

};
