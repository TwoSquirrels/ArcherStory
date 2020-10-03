#include "monster.hpp"

void monster::Update() {

    // もしHPがカンストしてたら直す
    if (this->HP > this->MaxHP) this->HP = this->MaxHP;
    // 毒
    if (PoisonCount > 0) {
        PoisonCount--;
        if (PoisonCount % 60 == 0) this->Damage(20, pos(0, 0));
    }
    // AttackCountを更新
    if (this->AttackCount > 0) this->AttackCount--;

    if (this->KnockBackCount > 0) {
        // ノックバック
        this->KnockBackCount--;
        this->Sprite.Pos.AddPos(this->KnockBack.GetX(), this->KnockBack.GetY());
        // ブロックとの当たり判定
        pos Motion = this->Sprite.Motion;
        this->Sprite.Motion = this->KnockBack;
        this->Map->Collision(&(this->Sprite), BlockCol);
        this->Sprite.Motion = Motion;
    } else {
        // Motionの分だけ移動
        this->Sprite.Move();
        // ブロックとの当たり判定
        this->Map->Collision(&(this->Sprite), BlockCol);
    }
    // 外には出させない
    if (!this->Map->GetInMap(this->Sprite)) this->Sprite.Pos = this->SpawnPoint;
    // 接触ダメージ
    if (!this->Hide && this->Player->CheckHit(this->Sprite, player::CIRCLE)) this->Player->Damage(this->Player->GetMaxHP() / 64);

}

void monster::DrawRing(int Scroll) {
    if (!this->Hide) {

        DxLib::DrawExtendGraph(
            -Scroll + this->Sprite.GetCenterPos().GetXInt() - min(this->Sprite.Size.GetXInt(), this->Sprite.Size.GetYInt()) / 2,
            96 + this->Sprite.GetCenterPos().GetYInt() - min(this->Sprite.Size.GetXInt(), this->Sprite.Size.GetYInt()) / 2,
            -Scroll + this->Sprite.GetCenterPos().GetXInt() + min(this->Sprite.Size.GetXInt(), this->Sprite.Size.GetYInt()) / 2,
            96 + this->Sprite.GetCenterPos().GetYInt() + min(this->Sprite.Size.GetXInt(), this->Sprite.Size.GetYInt()) / 2,
            this->Graph["ring"], TRUE
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

void monster::Damage(int Damage, pos Motion, bool Poison) {
    this->HP -= Damage;
    this->AttackCount += 10;
    this->KnockBack = pos(Motion.GetX() / 4, Motion.GetY() / 4);
    this->KnockBackCount = 4;
    if (Poison) this->PoisonCount = 480;
    if (this->HP <= 0) {
        // 死んじゃった！
        this->HP = 0;
        this->Use = false;
    }
}

bool monster::CheckHit(sprite Sprite, enum shape Shape) {

    switch (Shape) {
    case this->SQUARE:
        if (this->Sprite.GetSidePos(sprite::LEFT) >= Sprite.GetSidePos(sprite::RIGHT)) return false;
        if (this->Sprite.GetSidePos(sprite::RIGHT) <= Sprite.GetSidePos(sprite::LEFT)) return false;
        if (this->Sprite.GetSidePos(sprite::UP) >= Sprite.GetSidePos(sprite::DOWN)) return false;
        if (this->Sprite.GetSidePos(sprite::DOWN) <= Sprite.GetSidePos(sprite::UP)) return false;
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
monster::monster(pos Pos, int HP, int StartAttackCount, map *Map, player *Player, std::map<std::string, int> Graph, pos Size) {

    this->Use = true;

    this->Sprite.Pos = Pos;
    this->SpawnPoint = this->Sprite.Pos;
    this->MaxHP = HP;
    this->HP = HP;
    this->AttackCount = StartAttackCount;
    this->MaxAttackCount = StartAttackCount;
    this->Map = Map;
    this->Player = Player;
    this->Graph = Graph;
    this->Sprite.Size = Size;
    
    // 画像サイズ取得
    int X, Y;

    DxLib::GetGraphSize(this->Graph["ring"], &X, &Y);
    this->GraphSize["ring"].SetPos(X, Y);

}
