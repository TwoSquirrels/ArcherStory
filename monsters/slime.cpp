#include "slime.hpp"

std::vector<slime> slime::Update() {

    std::vector<slime> SlimeToAdd;

    if (this->Monster->HP > 0) {

        this->Monster->Update();

        // ランダム移動
        if (this->Monster->GetAttack()) {
            this->Monster->Sprite.Direction = DxLib::GetRand(360 - 1) * DX_PI / 180.0;
            this->Monster->Sprite.Motion = this->Monster->Sprite.GetPosFromDirection(16);
            this->MoveCount = 0;
        }
        if (this->MoveCount++ >= 10) this->Monster->Sprite.Motion.SetPos(0, 0);

    }

    // 分裂
    if (this->Monster->GetDeath() && this->Level > 1) {
        SlimeToAdd.push_back(slime(
            this->Monster->Sprite.Pos,
            this->Monster->MaxHP / 2,
            this->Level - 1,
            this->Map,
            this->Player,
            this->Graph,
            this->Config
        ));
        SlimeToAdd.push_back(slime(
            this->Monster->Sprite.Pos,
            this->Monster->MaxHP / 2,
            this->Level - 1,
            this->Map,
            this->Player,
            this->Graph,
            this->Config
        ));
    }

    return SlimeToAdd;

}

void slime::Draw(int Scroll) {

    if (this->Monster->HP > 0) {
        this->Monster->DrawRing(Scroll);
        if (this->Level == 1) {
            DxLib::DrawGraph(
                -Scroll + this->Monster->Sprite.GetCenterPos().GetXInt() - this->GraphSize["slime"].GetXInt() / 2,
                96 + this->Monster->Sprite.GetCenterPos().GetYInt() - this->GraphSize["slime"].GetYInt() / 2,
                this->Graph["slime"], TRUE
            );
        } else {
            DxLib::DrawGraph(
                -Scroll + this->Monster->Sprite.GetCenterPos().GetXInt() - this->GraphSize["slime_lvlup"].GetXInt() / 2,
                96 + this->Monster->Sprite.GetCenterPos().GetYInt() - this->GraphSize["slime_lvlup"].GetYInt() / 2,
                this->Graph["slime_lvlup"], TRUE
            );
        }
        this->Monster->DrawHP(Scroll);
    }

}

slime::slime() {};
slime::slime(pos Pos, int HP, int Level, map *Map, player *Player, std::map<std::string, int> Graph, json Config) {

    this->Monster = new monster(Pos, HP, Config["Monsters"]["Slime"]["MoveCooldown"].get<int>(), Map, Player, Graph);
    this->Level = Level;
    this->Map = Map;
    this->Player = Player;
    this->Graph = Graph;
    this->Config = Config;

    // 画像サイズ取得
    int X, Y;

    DxLib::GetGraphSize(this->Graph["slime"], &X, &Y);
    this->GraphSize["slime"].SetPos(X, Y);

    DxLib::GetGraphSize(this->Graph["slime_lvlup"], &X, &Y);
    this->GraphSize["slime_lvlup"].SetPos(X, Y);

}
