#pragma once
#include "DxLib.h"
#include "../sprite.hpp"
#include "../map.hpp"

class ball {

private:

    int Type;
    sprite Sprite;

public:

    bool Use;

    void Update(map Map) {
        if (this->Use) {
            // ˆÚ“®
            this->Sprite.Pos.AddPos(this->Sprite.Motion.GetX(), this->Sprite.Motion.GetY());
            // ŠO‚Éo‚Ä‚½‚çÁ‚·
            //if (this->Sprite.Pos.GetX() > )
        }
    }

    void Draw(int Scroll) {
        if (this->Use) {
            DxLib::DrawCircle(
                -Scroll + this->Sprite.Pos.GetXInt() + this->Sprite.Size.GetXInt() / 2, 96 + this->Sprite.Pos.GetYInt() + this->Sprite.Size.GetYInt() / 2,
                4, 0xff0000, TRUE
            );
            //DxLib::DrawFormatString(16, 16, 0x000000, "%f, %f", this->Sprite.Pos.GetX(), this->Sprite.Pos.GetY());
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
