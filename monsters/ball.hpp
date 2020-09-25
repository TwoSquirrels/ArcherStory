#pragma once
#include "DxLib.h"
#include "../sprite.hpp"
#include "../map.hpp"

class ball {

public:

    enum type {
        TEST, JUMP
    };

private:

    enum type Type;
    sprite Sprite;
    pos PlayerCenterPos{};
    // ƒWƒƒƒ“ƒvŒn
    //const double JUMP_h = 32.0;
    //double JUMP_a = 0;   // y=ax+h ‚Ì a

public:

    bool Use;

    void Update(map Map) {
        if (this->Use) {
            switch (this->Type) {
            case this->JUMP:
                // ”¼Œa‚ª2ˆÈ“à‚È‚çÁ‚·
                if (Distance2d(this->Sprite.Pos, this->PlayerCenterPos) <= 2.0) this->Use = false;
                break;
            }
            // ˆÚ“®
            this->Sprite.Pos.AddPos(this->Sprite.Motion.GetX(), this->Sprite.Motion.GetY());
            // ŠO‚Éo‚Ä‚½‚çÁ‚·
            if (!Map.GetInMap(this->Sprite)) this->Use = false;
        }
    }

    void Draw(int Scroll) {
        if (this->Use) {
            switch (this->Type) {
            case this->TEST:
                DxLib::DrawCircle(
                    -Scroll + this->Sprite.GetCenterPos().GetXInt(), 96 + this->Sprite.GetCenterPos().GetYInt(),
                    4, 0xff0000, TRUE
                );
                break;
            case this->JUMP:
                DxLib::DrawCircle(
                    -Scroll + this->Sprite.GetCenterPos().GetXInt(), 96 + this->Sprite.GetCenterPos().GetYInt(),
                    4, 0xff0000, TRUE
                );
                break;
            }
            //DxLib::DrawFormatString(16, 16, 0x000000, "%f, %f", this->Sprite.Pos.GetX(), this->Sprite.Pos.GetY());
        }
    }

    ball() {
        this->Use = false;
    }
    // TEST, JUMP
    ball(enum type Type, pos Pos, pos PlayerCenterPos) {

        this->Use = true;
        this->Type = Type;
        this->Sprite.Pos = Pos;
        this->PlayerCenterPos = PlayerCenterPos;
        this->Sprite.SetDrectionFromPos(pos(
            this->PlayerCenterPos.GetX() - this->Sprite.Pos.GetX(),
            this->PlayerCenterPos.GetY() - this->Sprite.Pos.GetY()
        ));
        this->Sprite.Motion = this->Sprite.GetPosFromDirection(4.0);
        if (Type == JUMP) {

        }

    }

};
