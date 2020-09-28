#pragma once
#include "sprite.hpp"
#include "map.hpp"
#include "monsters/ball.hpp"

class monster {

private:

    int MaxAttackCount;

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

    void Update(map Map);

    void DrawRing(int Scroll);
    void DrawHP(int Scroll);

    bool GetAttack(int ResetAttackCount = 0);

    int GetHP();
    void Heel(int AddHP);
    void Damage(int Damage);

    enum shape {
        SQUARE, CIRCLE
    };
    bool CheckHit(sprite Sprite, enum shape Shape = SQUARE);

    monster();
    monster(pos Pos, int HP, int StartAttackCount, pos Size = pos(32, 32));

};
