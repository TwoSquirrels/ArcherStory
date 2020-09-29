#include "monster.hpp"

void monster::Update(map Map) {

    // もしHPがカンストしてたら直す
    if (this->HP > this->MaxHP) this->HP = this->MaxHP;
    // AttackCountを更新
    if (this->AttackCount > 0) this->AttackCount--;

    // Motionの分だけ移動
    this->Sprite.Move();
    // ブロックとの当たり判定
    Map.Collision(&(this->Sprite), BlockCol);

}

void monster::DrawRing(int Scroll) {
    if (!this->Hide) {

        DxLib::DrawCircle(
            -Scroll + this->Sprite.GetCenterPos().GetXInt(), 96 + this->Sprite.GetCenterPos().GetYInt(),
            min(this->Sprite.Size.GetXInt(), this->Sprite.Size.GetYInt()) / 2,
            0xFF0000, FALSE, 4
        );

    }
}

void monster::DrawHP(int Scroll) {
    if (!this->Hide) {

        DxLib::DrawBox(
            -Scroll + this->Sprite.GetCenterPos().GetXInt() - 32, 96 + this->Sprite.Pos.GetYInt() - 16,
            -Scroll + this->Sprite.GetCenterPos().GetXInt() + 32, 96 + this->Sprite.Pos.GetYInt() - 8,
            0x000000, TRUE
        );
        DxLib::DrawBox(
            -Scroll + this->Sprite.GetCenterPos().GetXInt() - 32, 96 + this->Sprite.Pos.GetYInt() - 16,
            -Scroll + this->Sprite.GetCenterPos().GetXInt() - 32 + 1 + (64 - 1) * this->HP / this->MaxHP, 96 + this->Sprite.Pos.GetYInt() - 8,
            0xff0000, TRUE
        );
        DxLib::DrawBox(
            -Scroll + this->Sprite.GetCenterPos().GetXInt() - 32, 96 + this->Sprite.Pos.GetYInt() - 16,
            -Scroll + this->Sprite.GetCenterPos().GetXInt() + 32, 96 + this->Sprite.Pos.GetYInt() - 8,
            0x000000, FALSE
        );
        DxLib::DrawFormatString(
            -Scroll + this->Sprite.GetCenterPos().GetXInt() - 32, 96 + this->Sprite.Pos.GetYInt() - 32,
            0x000000, "%d", this->HP
        );

    }
}

bool monster::GetAttack(int ResetAttackCount) {
    if (this->AttackCount == 0) {
        this->AttackCount = ResetAttackCount;
        if (this->AttackCount == 0) this->AttackCount = this->MaxAttackCount;
        return true;
    }
    else return false;
}

bool monster::GetDeath() {
    bool Death = (this->BeforeHP > 0 && HP <= 0);
    this->BeforeHP = this->HP;
    return Death;
}

int monster::GetHP() {
    return this->HP;
}

void monster::Heal(int AddHP) {
    this->HP += AddHP;
    if (this->HP > this->MaxHP) this->HP = this->MaxHP;
}

void monster::Damage(int Damage) {
    this->HP -= Damage;
    this->AttackCount += 10;
    if (this->HP <= 0) {
        // 死んじゃった！
        this->HP = 0;
        this->Use = false;
    }
}

bool monster::CheckHit(sprite Sprite, enum shape Shape) {

    switch (Shape) {
    case this->SQUARE:
        if (this->Sprite.GetSidePos(sprite().LEFT) >= Sprite.GetSidePos(sprite().RIGHT)) return false;
        if (this->Sprite.GetSidePos(sprite().RIGHT) <= Sprite.GetSidePos(sprite().LEFT)) return false;
        if (this->Sprite.GetSidePos(sprite().UP) >= Sprite.GetSidePos(sprite().DOWN)) return false;
        if (this->Sprite.GetSidePos(sprite().DOWN) <= Sprite.GetSidePos(sprite().UP)) return false;
        return true;
    case this->CIRCLE:
        if (Distance2d(
            this->Sprite.GetCenterPos(), Sprite.GetCenterPos()
        ) < (
            min(this->Sprite.Size.GetX(), this->Sprite.Size.GetY()) + min(Sprite.Size.GetX(), Sprite.Size.GetY())
            )) return true;
        return false;
    }

}

monster::monster() {
    this->Use = false;
}
monster::monster(pos Pos, int HP, int StartAttackCount, pos Size) {

    this->Use = true;

    this->Sprite.Pos = Pos;
    this->MaxHP = HP;
    this->HP = HP;
    this->AttackCount = StartAttackCount;
    this->MaxAttackCount = StartAttackCount;
    this->Sprite.Size = Size;

}
