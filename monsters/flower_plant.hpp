#pragma once
#include "DxLib.h"
#include "ExternalHeaderFiles/json.hpp"
#include "../monster.hpp"
#include "../player.hpp"

using json = nlohmann::json;

class flower_plant {

private:

    monster Monster;

    int Attack;
    player *Player;
    std::vector<ball>* Ball;
    json BallConfig;

public:

    void Update(map Map, sprite Player) {

        if (this->Monster.HP > 0) {

            Monster.Update(Map);

            // UŒ‚
            if (Monster.GetAttack()) {
                this->Ball->push_back(ball(
                    ball().JUMP,
                    this->Attack,
                    this->Player,
                    this->Monster.Sprite.GetCenterPos(),
                    Player.GetCenterPos(),
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
    flower_plant(std::vector<ball> *Ball, pos StartPos, int HP, int Attack, player *Player, json Config) {
        
        this->Ball = Ball;
        this->Monster = monster(StartPos, HP, Config["Monster"]["FlowerPlant"]["AttackSpeed"].get<int>());
        this->Attack = Attack;
        this->Player = Player;
        this->BallConfig = Config["Balls"]["Jump"];

    }

};
