#pragma once
#include "DxLib.h"
#include "../sprite.hpp"
#include "../map.hpp"

class ball {

private:

    int Type;
    sprite Sprite;

public:

    enum Type {
        TEST
    };

    bool Use;

    void Update(map Map) {
        if (this->Use) {
            // ˆÚ“®
            this->Sprite.Pos.AddPos(this->Sprite.Motion.GetX(), this->Sprite.Motion.GetY());
            // ŠO‚Éo‚Ä‚½‚çÁ‚·
            if (!Map.GetInMap(this->Sprite)) this->Use = false;
        }
    }

    void Draw(int Scroll) {
        if (this->Use) {
            DxLib::DrawCircle(
                -Scroll + this->Sprite.GetCenterPos().GetXInt(), 96 + this->Sprite.GetCenterPos().GetYInt(),
                4, 0xff0000, TRUE
            );
            //DxLib::DrawFormatString(16, 16, 0x000000, "%f, %f", this->Sprite.Pos.GetX(), this->Sprite.Pos.GetY());
        }
    }

    ball() {
        this->Use = false;
    }
    ball(int Type, pos Pos, double Direction) {

        this->Use = true;
        this->Type = Type;
        this->Sprite.Pos = Pos;
        this->Sprite.Direction = Direction;
        this->Sprite.Motion = this->Sprite.GetPosFromDirection(4);

    }

};
