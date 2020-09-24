#pragma once
#include "DxLib.h"
#include "../monster.hpp"

class flower_plant {

private:

    monster Monster;

    std::vector<ball>* Ball;

public:

    void Update(map Map, pos PlayerPos) {

        if (this->Monster.HP > 0) {

            Monster.Update(Map);

            // UŒ‚
            if (Monster.GetAttack()) {
                this->Ball->push_back(ball(
                    0,
                    this->Monster.Sprite.GetCenterPos(),
                    sprite().SetDrectionFromPos(pos(
                        PlayerPos.GetX() - this->Monster.Sprite.Pos.GetX(),
                        PlayerPos.GetY() - this->Monster.Sprite.Pos.GetY()
                    ))
                ));
            }

        }

    }

    void Draw(int Scroll) {

        if (this->Monster.HP > 0) {
            this->Monster.DrawRing(Scroll);
            DxLib::DrawBox(
                -Scroll + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.LEFT ), 96 + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.UP  ),
                -Scroll + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.RIGHT), 96 + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.DOWN),
                0xff00ff, TRUE
            );
        }

    }

    flower_plant() {}
    flower_plant(std::vector<ball> *Ball, pos StartPos, int HP, int StartAttackCount = 120) {
        
        this->Ball = Ball;
        this->Monster = monster(StartPos, HP, StartAttackCount);

    }

};
