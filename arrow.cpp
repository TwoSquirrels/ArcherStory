#include "arrow.hpp"

void arrow::Update(map Map) {
    if (this->Use) {

        // 移動
        this->Sprite.Move();
        // マップとの当たり判定
        std::vector<bool> Col = Map.Collision(&this->Sprite, this->BlockCol);
        for (bool b : Col) if (b == true) this->Use = false;
        // 攻撃Hit
        for (int i = 0; i < this->Monster->size(); i++) {
            if ((*this->Monster)[i]->Use && (*this->Monster)[i]->CheckHit(this->Sprite, monster().CIRCLE)) {
                (*this->Monster)[i]->Damage(40);
                this->Use = false;
            }
        }
        // 外に出てたら消す
        if (!Map.GetInMap(this->Sprite)) this->Use = false;

    }
}

void arrow::Draw(int Scroll) {
    if (this->Use) {

        DxLib::DrawCircle(-Scroll + this->Sprite.Pos.GetXInt(), 96 + this->Sprite.Pos.GetYInt(), 8, 0x00ffff, TRUE);
        DxLib::DrawCircle(-Scroll + this->Sprite.Pos.GetXInt(), 96 + this->Sprite.Pos.GetYInt(), 8, 0x0000ff, FALSE, 3);

    }
}

arrow::arrow() {
    this->Use = false;
}
arrow::arrow(pos Pos, double Direction, std::vector<monster *> *Monster, json Config) {

    this->Use = true;
    this->Sprite.Pos = Pos;
    this->Sprite.Direction = Direction;
    this->Monster = Monster;
    this->Config = Config;

    this->Sprite.Motion = this->Sprite.GetPosFromDirection(this->Config["Speed"]);

}
