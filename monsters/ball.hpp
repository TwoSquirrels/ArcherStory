#pragma once
#include "DxLib.h"
#include "ExternalHeaderFiles/json.hpp"
#include "../sprite.hpp"
#include "../map.hpp"
#include "../player.hpp"

using json = nlohmann::json;

class ball {

public:

    enum type {
        TEST, JUMP
    };

private:

    json Config;

    enum type Type;
    int Attack;
    player *Player;
    sprite Sprite;
    pos PlayerCenterPos{};
    // ジャンプ系
    double Jump_a = 0;   // y=ax+h の a
    double Jump_CenterX = 0;
    sprite Shadow;

public:

    bool Use;

    void Update(map Map) {
        if (this->Use) {

            switch (this->Type) {
            case this->JUMP:
                // Shadow移動
                this->Shadow.Move();
                // Ball移動
                this->Sprite.Pos.SetY(
                    this->Shadow.Pos.GetY() - (this->Jump_a * pow(this->Jump_CenterX - this->Shadow.Pos.GetX(), 2) + this->Config["Jump"]["High"].get<double>())
                );
                // 半径がスピード/2以内なら消す
                if (Distance2d(this->Shadow.Pos, this->PlayerCenterPos) <= Config["Jump"]["Speed"].get<double>() / 2) this->Use = false;
                break;
            }
            // 移動
            this->Sprite.Move();
            // ダメージ
            if (this->Type == this->TEST || this->Type == this->JUMP) {
                if (this->Player->CheckHit(this->Sprite)) {
                    this->Player->Damage(this->Attack);
                    this->Use = false;
                }
            }
            // 外に出てたら消す
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
                    -Scroll + this->Shadow.GetCenterPos().GetXInt(), 96 + this->Shadow.GetCenterPos().GetYInt(),
                    4, 0x000000, TRUE
                );
                DxLib::DrawCircle(
                    -Scroll + this->Sprite.GetCenterPos().GetXInt(), 96 + this->Sprite.GetCenterPos().GetYInt(),
                    4, 0xff0000, TRUE
                );
                break;
            }

        }
    }

    ball() {
        this->Use = false;
    }
    // TEST, JUMP
    ball(enum type Type, int Attack, player *Player, pos Pos, pos PlayerCenterPos, json Config) {

        this->Use = true;
        this->Type = Type;
        this->Attack = Attack;
        this->Player = Player;
        this->Sprite.Pos = Pos;
        this->PlayerCenterPos = PlayerCenterPos;
        this->Config = Config;
        switch (Type) {
        case TEST:
            this->Sprite.SetDrectionFromPos(pos(
                this->PlayerCenterPos.GetX() - this->Sprite.Pos.GetX(),
                this->PlayerCenterPos.GetY() - this->Sprite.Pos.GetY()
            ));
            this->Sprite.Motion = this->Sprite.GetPosFromDirection(4.0);
            break;
        case JUMP:
            this->Jump_a = -4 * this->Config["Jump"]["High"].get<double>() / std::pow(this->PlayerCenterPos.GetX() - this->Sprite.Pos.GetX(), 2);
            this->Shadow.Pos = this->Sprite.Pos;
            this->Shadow.SetDrectionFromPos(pos(
                this->PlayerCenterPos.GetX() - this->Sprite.Pos.GetX(),
                this->PlayerCenterPos.GetY() - this->Sprite.Pos.GetY()
            ));
            this->Shadow.Motion = this->Shadow.GetPosFromDirection(Config["Jump"]["Speed"].get<double>());
            this->Sprite.Motion.SetX(Shadow.Motion.GetX());
            this->Jump_CenterX = (this->Sprite.Pos.GetX() + this->PlayerCenterPos.GetX()) / 2;
            break;
        }

    }

};
