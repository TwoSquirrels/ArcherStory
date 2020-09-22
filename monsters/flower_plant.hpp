#pragma once
#include "DxLib.h"
#include "../monster.hpp"

class flower_plant {

private:

    monster Monster;

    map Map;
    std::vector<ball>* Ball;

public:

    void Update() {

        if (this->Monster.HP > 0) {

            Monster.Update(this->Map);

            // UŒ‚
            if (Monster.GetAttack()) {
                this->Ball->push_back(ball(0, this->Monster.Sprite.Pos), pos(1, 0));
            }

        }

    }

    void Draw(int Scroll) {

        if (this->Monster.HP > 0) {
            this->Monster.DrawRing(Scroll);
            DxLib::DrawBox(
                -Scroll + this->Monster.Sprite.Pos.GetXInt(), 96 + this->Monster.Sprite.Pos.GetYInt(),
                -Scroll + this->Monster.Sprite.Pos.GetXInt() + this->Monster.Sprite.Size.GetXInt(), 96 + this->Monster.Sprite.Pos.GetYInt() + this->Monster.Sprite.Size.GetXInt(),
                0xff00ff, TRUE
            );
        }

    }

    flower_plant() {}
    flower_plant(map Map, std::vector<ball> *Ball, pos StartPos, int HP, int StartAttackCount = 120) {
        
        this->Map = Map;
        this->Ball = Ball;
        this->Monster = monster(StartPos, HP, StartAttackCount);

    }

};
