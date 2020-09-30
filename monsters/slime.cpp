#include "slime.hpp"

std::vector<slime> slime::Update(map Map) {

    std::vector<slime> SlimeToAdd;

    if (this->Monster.HP > 0) {

        Monster.Update(Map);

        // ƒ‰ƒ“ƒ_ƒ€ˆÚ“®
        if (this->Monster.GetAttack()) {
            this->Monster.Sprite.Direction = DxLib::GetRand(360 - 1) * DX_PI / 180.0;
            this->Monster.Sprite.Motion = this->Monster.Sprite.GetPosFromDirection(16);
            this->MoveCount = 0;
        }
        if (this->MoveCount++ >= 10) this->Monster.Sprite.Motion.SetPos(0, 0);

    }

    // •ª—ô
    if (this->Monster.GetDeath() && this->Level > 1) {
        SlimeToAdd.push_back(slime(
            this->Monster.Sprite.Pos,
            this->Monster.MaxHP / 2,
            this->Level - 1,
            this->Player,
            this->Config
        ));
        SlimeToAdd.push_back(slime(
            this->Monster.Sprite.Pos,
            this->Monster.MaxHP / 2,
            this->Level - 1,
            this->Player,
            this->Config
        ));
    }

    return SlimeToAdd;

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
slime::slime(pos Pos, int HP, int Level, player *Player, json Config) {

    this->Monster = monster(Pos, HP, Config["Monsters"]["Slime"]["MoveCooldown"].get<int>(), Player);
    this->Level = Level;
    this->Player = Player;
    this->Config = Config;

}
