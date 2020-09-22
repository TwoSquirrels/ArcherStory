#pragma once
#include "sprite.hpp"
#include "map.hpp"
#include "monsters/ball.hpp"

class monster {

private:

    int MaxAttackCount;

public:

    sprite Sprite;
    int HP, MaxHP;
    std::vector<bool> BlockCol = {
        false,  // air
        true,   // wall
        true,   // stone
        true,   // pond
    };

    int AttackCount;

    void Update(map Map) {

        // もしHPがカンストしてたら直す
        if (this->HP > this->MaxHP) this->HP = this->MaxHP;
        // AttackCountを更新
        if (this->AttackCount > 0) this->AttackCount--;

        // Motionの分だけ移動
        this->Sprite.Pos.AddPos(this->Sprite.Motion.GetX(), this->Sprite.Motion.GetY());
        // ブロックとの当たり判定
        Map.Collision(&(this->Sprite), BlockCol);

    }

    void DrawRing(int Scroll) {

        DxLib::DrawCircle(
            -Scroll + this->Sprite.Pos.GetXInt() + this->Sprite.Size.GetXInt() / 2, 96 + this->Sprite.Pos.GetYInt() + this->Sprite.Size.GetYInt() / 2,
            min(this->Sprite.Size.GetXInt(), this->Sprite.Size.GetYInt()) / 2,
            0xFF0000, FALSE, 4
        );

    }

    bool GetAttack(int ResetAttackCount = 0) {
        if (this->AttackCount == 0) {
            this->AttackCount = ResetAttackCount;
            if (this->AttackCount == 0) this->AttackCount = this->MaxAttackCount;
            return true;
        }
        else return false;
    }

    monster() {}
    monster(pos StartPos, int HP, int StartAttackCount, pos Size = pos(32, 32)) {

        this->Sprite.Pos = StartPos;
        this->MaxHP = HP;
        this->HP = HP;
        this->AttackCount = StartAttackCount;
        this->MaxAttackCount = StartAttackCount;
        this->Sprite.Size = Size;

    }

};
