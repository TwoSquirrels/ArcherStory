#pragma once
#include "DxLib.h"
#include "ExternalHeaderFiles/json.hpp"
#include "sprite.hpp"
#include "map.hpp"
#include "player.hpp"
#include "monster.hpp"

using json = nlohmann::json;

class arrow {

private:

    json Config;

    sprite Sprite;
    std::vector<monster *> *Monster;

public:

    bool Use;

    void Update(map Map) {
        if (this->Use) {

            // ˆÚ“®
            this->Sprite.Move();
            // UŒ‚Hit

            // ŠO‚Éo‚Ä‚½‚çÁ‚·
            if (!Map.GetInMap(this->Sprite)) this->Use = false;

        }
    }

    void Draw(int Scroll) {
        if (this->Use) {

            DxLib::DrawCircle(-Scroll + this->Sprite.Pos.GetXInt(), 96 + this->Sprite.Pos.GetYInt(), 8, 0x00ffff, TRUE);
            DxLib::DrawCircle(-Scroll + this->Sprite.Pos.GetXInt(), 96 + this->Sprite.Pos.GetYInt(), 8, 0x0000ff, FALSE, 3);

        }
    }

    arrow() {
        this->Use = false;
    }
    arrow(pos Pos, double Direction, std::vector<monster *> *Monster, json Config) {

        this->Use = true;
        this->Sprite.Pos = Pos;
        this->Sprite.Direction = Direction;
        this->Monster = Monster;
        this->Config = Config;

        this->Sprite.Motion = this->Sprite.GetPosFromDirection(this->Config["Speed"]);

    }

};
