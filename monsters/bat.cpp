#include "bat.hpp"

void bat::Update() {

    if (this->Monster->HP > 0) {

        this->Monster->Update();

        // 攻撃
        if (this->Monster->GetAttack()) {
            double CenterDirection = std::atan2(
                this->Player->Sprite.GetCenterPos().GetY() - this->Monster->Sprite.GetCenterPos().GetY(),
                this->Player->Sprite.GetCenterPos().GetX() - this->Monster->Sprite.GetCenterPos().GetX()
            );
            try {
                this->Ball->push_back(ball(
                    ball().BEAM,
                    this->Attack,
                    this->Map,
                    this->Player,
                    this->Monster->Sprite.GetCenterPos(),
                    CenterDirection,
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
            this->MoveCount = 0;
        }

        // ランダム移動
        if (this->Monster->Sprite.Motion.GetX() == 0.0 && this->Monster->Sprite.Motion.GetY() == 0.0 && this->MoveCount >= Config["Monsters"]["Bat"]["AttackSpeed"].get<int>() / 3) {
            this->Monster->Sprite.Direction = DxLib::GetRand(360 - 1) * DX_PI / 180.0;
            this->Monster->Sprite.Motion = this->Monster->Sprite.GetPosFromDirection(2);
        }
        if (this->MoveCount >= Config["Monsters"]["Bat"]["AttackSpeed"].get<int>() * 2 / 3) this->Monster->Sprite.Motion.SetPos(0, 0);
        this->MoveCount++;

    }

}

void bat::Draw(int Scroll) {

    if (this->Monster->HP > 0) {
        this->Monster->DrawRing(Scroll);
        DxLib::DrawGraph(
            -Scroll + this->Monster->Sprite.GetCenterPos().GetXInt() - this->GraphSize["bat"].GetXInt() / 2,
            96 + this->Monster->Sprite.GetCenterPos().GetYInt() - this->GraphSize["bat"].GetYInt() / 2,
            this->Graph["bat"], TRUE
        );
        this->Monster->DrawHP(Scroll);
    }

}

bat::bat() {};
bat::bat(std::vector<ball> *Ball, pos Pos, int HP, int Attack, map *Map, player *Player, std::map<std::string, int> Graph, json Config) {

    this->Ball = Ball;
    this->Monster = new monster(Pos, HP, Config["Monsters"]["Bat"]["AttackSpeed"].get<int>(), Map, Player, Graph);
    this->Attack = Attack;
    this->Map = Map;
    this->Player = Player;
    this->Graph = Graph;
    this->Config = Config;
    this->Monster->BlockCol[map::STONE] = false;

    // 画像サイズ取得
    int X, Y;

    DxLib::GetGraphSize(this->Graph["bat"], &X, &Y);
    this->GraphSize["bat"].SetPos(X, Y);

}
