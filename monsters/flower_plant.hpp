#pragma once
#include "DxLib.h"
#include "ExternalHeaderFiles/json.hpp"
#include "../monster.hpp"
#include "../player.hpp"

using json = nlohmann::json;

class flower_plant {

private:

    int Attack;
    player *Player;
    std::vector<ball>* Ball;
    json BallConfig;

public:

    monster Monster;

    void Update(map Map) {

        if (this->Monster.HP > 0) {

            Monster.Update(Map);

            // UŒ‚
            if (Monster.GetAttack()) {
                this->Monster.Sprite.SetDrectionFromPos(pos(
                    this->Player->Sprite.GetCenterPos().GetX() - this->Monster.Sprite.GetCenterPos().GetX(),
                    this->Player->Sprite.GetCenterPos().GetY() - this->Monster.Sprite.GetCenterPos().GetY()
                ));
                this->Ball->push_back(ball(
                    ball().JUMP,
                    this->Attack,
                    this->Player,
                    this->Monster.Sprite.GetCenterPos(),
                    this->Player->Sprite.GetCenterPos(),
                    this->BallConfig
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
    flower_plant(std::vector<ball> *Ball, pos Pos, int HP, int Attack, player *Player, json Config) {
        
        this->Ball = Ball;
        this->Monster = monster(Pos, HP, Config["Monsters"]["FlowerPlant"]["AttackSpeed"].get<int>());
        this->Attack = Attack;
        this->Player = Player;
        this->BallConfig = Config["Balls"];

    }

};
