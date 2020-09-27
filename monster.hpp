#pragma once
#include "sprite.hpp"
#include "map.hpp"
#include "monsters/ball.hpp"

class monster {

private:

    int MaxAttackCount;

public:

    bool Use;

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
        this->Sprite.Move();
        // ブロックとの当たり判定
        Map.Collision(&(this->Sprite), BlockCol);

    }

    void DrawRing(int Scroll) {

        DxLib::DrawCircle(
            -Scroll + this->Sprite.GetCenterPos().GetXInt(), 96 + this->Sprite.GetCenterPos().GetYInt(),
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

    monster() {
        this->Use = false;
    }
    monster(pos Pos, int HP, int StartAttackCount, pos Size = pos(32, 32)) {

        this->Use = true;

        this->Sprite.Pos = Pos;
        this->MaxHP = HP;
        this->HP = HP;
        this->AttackCount = StartAttackCount;
        this->MaxAttackCount = StartAttackCount;
        this->Sprite.Size = Size;

    }

};
