#include "arrow.hpp"

void arrow::Update() {
    if (this->Use) {

        // 移動
        this->Sprite.Move();
        // マップとの当たり判定
        std::vector<bool> Col = this->Map->Collision(&this->Sprite, this->BlockCol);
        for (bool b : Col) if (b == true) this->Use = false;
        // 攻撃Hit
        for (int i = 0; i < this->Monster->size(); i++) {
            if ((*this->Monster)[i]->Use && (*this->Monster)[i]->CheckHit(this->Sprite, monster().CIRCLE)) {
                (*this->Monster)[i]->Damage(this->Attack);
                this->Use = false;
                break;
            }
        }
        // 外に出てたら消す
        if (!this->Map->GetInMap(this->Sprite)) this->Use = false;

    }
}

void arrow::Draw(int Scroll) {
    if (this->Use) {

        DxLib::DrawRotaGraph(
            -Scroll + this->Sprite.GetCenterPos().GetXInt(), 96 + this->Sprite.GetCenterPos().GetYInt(),
            1.0, (this->Sprite.GetPosFromDirection().GetX() >= 0.0) ? this->Sprite.Direction : this->Sprite.Direction + DX_PI,
            this->Graph["arrow_arrow"], TRUE, (this->Sprite.GetPosFromDirection().GetX() < 0.0)
        );

    }
}

arrow::arrow() {
    this->Use = false;
}
arrow::arrow(pos Pos, double Direction, map *Map, std::vector<monster *> *Monster, int Attack, std::map<std::string, int> Graph, json Config) {

    this->Use = true;
    this->Sprite.Pos = Pos;
    this->Sprite.Direction = Direction;
    this->Map = Map;
    this->Monster = Monster;
    this->Attack = Attack;
    this->Graph = Graph;
    this->Config = Config;

    this->Sprite.Motion = this->Sprite.GetPosFromDirection(this->Config["Speed"]);

    // 画像サイズ取得
    int X, Y;

    DxLib::GetGraphSize(this->Graph["arrow_arrow"], &X, &Y);
    this->GraphSize["arrow_arrow"].SetPos(X, Y);

}
