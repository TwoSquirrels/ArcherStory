#include "flower_plant.hpp"

void flower_plant::Update() {

    if (this->Monster->HP > 0) {

        Monster->Update();

        // 攻撃
        if (Monster->GetAttack()) {
            this->Monster->Sprite.SetDirectionFromPos(pos(
                this->Player->Sprite.GetCenterPos().GetX() - this->Monster->Sprite.GetCenterPos().GetX(),
                this->Player->Sprite.GetCenterPos().GetY() - this->Monster->Sprite.GetCenterPos().GetY()
            ));
            try {
                this->Ball->push_back(ball(
                    ball().JUMP,
                    this->Attack,
                    this->Map,
                    this->Player,
                    this->Monster->Sprite.GetCenterPos(),
                    this->Player->Sprite.GetCenterPos(),
                    this->Graph,
                    this->Config["Balls"]
                ));
            } catch (ball::error e) {
                switch (e) {
                case ball::WRONG_CONSTRUCTOR:
                    MessageBox(NULL, "間違ったballのコンストラクタを使用しています。", "プログラム内エラー", MB_OK | MB_ICONERROR);
                    break;
                }
            }
        }

    }

}

void flower_plant::Draw(int Scroll) {

    if (this->Monster->HP > 0) {
        this->Monster->DrawRing(Scroll);
        if (this->Monster->AttackCount > this->Config["Monsters"]["FlowerPlant"]["AttackSpeed"].get<int>() - 15) {
            DxLib::DrawRotaGraph(
                -Scroll + this->Monster->Sprite.GetCenterPos().GetXInt(), 96 + this->Monster->Sprite.GetCenterPos().GetYInt(),
                1.0, (this->Monster->Sprite.GetPosFromDirection().GetX() >= 0.0) ? this->Monster->Sprite.Direction + DX_PI / 2 : this->Monster->Sprite.Direction + DX_PI / 2,
                this->Graph["flower_plant_attack"], TRUE, (this->Monster->Sprite.GetPosFromDirection().GetX() < 0.0)
            );
        } else {
            DxLib::DrawRotaGraph(
                -Scroll + this->Monster->Sprite.GetCenterPos().GetXInt(), 96 + this->Monster->Sprite.GetCenterPos().GetYInt(),
                1.0, (this->Monster->Sprite.GetPosFromDirection().GetX() >= 0.0) ? this->Monster->Sprite.Direction + DX_PI / 2 : this->Monster->Sprite.Direction + DX_PI / 2,
                this->Graph["flower_plant_normal"], TRUE, (this->Monster->Sprite.GetPosFromDirection().GetX() < 0.0)
            );
        }
        this->Monster->DrawHP(Scroll);
    }

}

flower_plant::flower_plant() {};
flower_plant::flower_plant(std::vector<ball> *Ball, pos Pos, int HP, int Attack, map *Map, player *Player, std::map<std::string, int> Graph, json Config) {

    this->Ball = Ball;
    this->Monster = new monster(Pos, HP, Config["Monsters"]["FlowerPlant"]["AttackSpeed"].get<int>(), Map, Player, Graph);
    this->Monster->Sprite.Direction = DX_PI * 3 / 2;
    this->Attack = Attack;
    this->Map = Map;
    this->Player = Player;
    this->Graph = Graph;
    this->Config = Config;

    // 画像サイズ取得
    int X, Y;

    DxLib::GetGraphSize(this->Graph["flower_plant_normal"], &X, &Y);
    this->GraphSize["flower_plant_normal"].SetPos(X, Y);

    DxLib::GetGraphSize(this->Graph["flower_plant_attack"], &X, &Y);
    this->GraphSize["flower_plant_attack"].SetPos(X, Y);

}
