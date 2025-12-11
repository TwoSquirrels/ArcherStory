#include "virus.hpp"

std::vector<virus> virus::Update() {

    std::vector<virus> VirusToAdd;

    if (this->Monster->HP > 0) {

        this->Monster->Update();

        // 追尾
        this->Monster->Sprite.SetDrectionFromPos(pos(
            this->Player->Sprite.GetCenterPos().GetX() - this->Monster->Sprite.GetCenterPos().GetX(),
            this->Player->Sprite.GetCenterPos().GetY() - this->Monster->Sprite.GetCenterPos().GetY()
        ));
        this->Monster->Sprite.Motion = this->Monster->Sprite.GetPosFromDirection(1.5);

    }

    // 分裂
    if (this->Monster->GetDeath() && this->Level > 1) {
        VirusToAdd.push_back(virus(
            this->Monster->Sprite.Pos,
            this->Monster->MaxHP / 2,
            this->Level - 1,
            this->Map,
            this->Player,
            this->Graph
        ));
        VirusToAdd.push_back(virus(
            this->Monster->Sprite.Pos,
            this->Monster->MaxHP / 2,
            this->Level - 1,
            this->Map,
            this->Player,
            this->Graph
        ));
    }

    return VirusToAdd;

}

void virus::Draw(int Scroll) {

    if (this->Monster->HP > 0) {
        this->Monster->DrawRing(Scroll);
        if (this->Level == 1) {
            DxLib::DrawGraph(
                -Scroll + this->Monster->Sprite.GetCenterPos().GetXInt() - this->GraphSize["virus"].GetXInt() / 2,
                96 + this->Monster->Sprite.GetCenterPos().GetYInt() - this->GraphSize["virus"].GetYInt() / 2,
                this->Graph["virus"], TRUE
            );
        } else {
            DxLib::DrawGraph(
                -Scroll + this->Monster->Sprite.GetCenterPos().GetXInt() - this->GraphSize["virus_lvlup"].GetXInt() / 2,
                96 + this->Monster->Sprite.GetCenterPos().GetYInt() - this->GraphSize["virus_lvlup"].GetYInt() / 2,
                this->Graph["virus_lvlup"], TRUE
            );
        }
        this->Monster->DrawHP(Scroll);
    }

}

virus::virus() {};
virus::virus(pos Pos, int HP, int Level, map *Map, player *Player, std::map<std::string, int> Graph) {

    this->Monster = new monster(Pos, HP, 0, Map, Player, Graph);
    this->Level = Level;
    this->Map = Map;
    this->Player = Player;
    this->Graph = Graph;

    // 画像サイズ取得
    int X, Y;

    DxLib::GetGraphSize(this->Graph["virus"], &X, &Y);
    this->GraphSize["virus"].SetPos(X, Y);

    DxLib::GetGraphSize(this->Graph["virus_lvlup"], &X, &Y);
    this->GraphSize["virus_lvlup"].SetPos(X, Y);

}
