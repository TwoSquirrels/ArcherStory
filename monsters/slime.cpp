#include "slime.hpp"

void slime::Update(map Map) {

    if (this->Monster.HP > 0) {

        Monster.Update(Map);

        // ƒ‰ƒ“ƒ_ƒ€ˆÚ“®
        if (this->Monster.GetAttack()) {
            this->Monster.Sprite.Direction = DxLib::GetRand(360 - 1) * DX_PI / 180.0;
            this->Monster.Sprite.Motion = this->Monster.Sprite.GetPosFromDirection(16);
            this->MoveCount = 0;
        }
        if (this->MoveCount++ >= 20) this->Monster.Sprite.Motion.SetPos(0, 0);

    }

    // •ª—ô
    if (this->Monster.GetDeath() && this->Level > 1) {
        this->Slime->push_back(slime(
            this->Monster.Sprite.Pos,
            this->Monster.MaxHP / 2,
            this->Level - 1,
            this->Player,
            this->Slime,
            this->Config
        ));
        this->Slime->push_back(slime(
            this->Monster.Sprite.Pos,
            this->Monster.MaxHP / 2,
            this->Level - 1,
            this->Player,
            this->Slime,
            this->Config
        ));
    }

}

void slime::Draw(int Scroll) {

    if (this->Monster.HP > 0) {
        this->Monster.DrawRing(Scroll);
        if (this->Level == 1) DxLib::DrawBox(
            -Scroll + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.LEFT), 96 + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.UP),
            -Scroll + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.RIGHT), 96 + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.DOWN),
            0x00ffff, TRUE
        );
        else DxLib::DrawBox(
            -Scroll + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.LEFT), 96 + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.UP),
            -Scroll + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.RIGHT), 96 + this->Monster.Sprite.GetSidePos(this->Monster.Sprite.DOWN),
            0xff8000, TRUE
        );
        this->Monster.DrawHP(Scroll);
    }

}

slime::slime() {};
slime::slime(pos Pos, int HP, int Level, player *Player, std::vector<slime> *Slime, json Config) {

    this->Monster = monster(Pos, HP, Config["Monsters"]["Slime"]["MoveCooldown"].get<int>());
    this->Level = Level;
    this->Player = Player;
    this->Slime = Slime;
    this->Config = Config;

}
