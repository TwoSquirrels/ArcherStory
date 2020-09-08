#include "player.hpp"

void player::Move() {

    // ‘€ì
    if (this->Input->GetKey(KEY_INPUT_UP)) this->Sprite.Motion.SetY(-4);
    else if (this->Input->GetKey(KEY_INPUT_DOWN)) this->Sprite.Motion.SetY(4);
    else this->Sprite.Motion.SetY(0);
    if (this->Input->GetKey(KEY_INPUT_LEFT)) this->Sprite.Motion.SetX(-4);
    else if (this->Input->GetKey(KEY_INPUT_RIGHT)) this->Sprite.Motion.SetX(4);
    else this->Sprite.Motion.SetX(0);

    // Pos += Motion
    this->Sprite.Pos.AddPos(this->Sprite.Motion.GetX(), this->Sprite.Motion.GetY());

}

void player::Draw() {

    DxLib::DrawGraph(this->Sprite.Pos.GetXInt(), this->Sprite.Pos.GetYInt(), this->Img["ring"], TRUE);
    DxLib::DrawGraph(this->Sprite.Pos.GetXInt(), this->Sprite.Pos.GetYInt(), this->Img["right"], TRUE);

}

player::player(input *Input) {

    this->Input = Input;

    // ‰æ‘œ
    this->Img["ring"]   = DxLib::LoadGraph("data/stable/img/player/ring.png");
    this->Img["right"]  = DxLib::LoadGraph("data/stable/img/player/right.png");

}
