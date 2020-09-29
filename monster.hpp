#pragma once
#include "sprite.hpp"
#include "map.hpp"
#include "player.hpp"
#include "monsters/ball.hpp"

class player;

class monster {

private:

    int MaxAttackCount;
    int BeforeHP = this->MaxHP;

public:

    bool Use;
    bool Hide = false;

    sprite Sprite;
    int HP, MaxHP;
    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        true,   // pond
    };

    int AttackCount;

    player *Player;

    void Update(map Map);

    void DrawRing(int Scroll);
    void DrawHP(int Scroll);

    bool GetAttack(int ResetAttackCount = 0);
    bool GetDeath();

    int GetHP();
    void Heal(int AddHP);
    void Damage(int Damage);

    enum shape {
        SQUARE, CIRCLE
    };
    bool CheckHit(sprite Sprite, enum shape Shape = SQUARE);

    monster();
    monster(pos Pos, int HP, int StartAttackCount, player *Player, pos Size = pos(32, 32));

};
