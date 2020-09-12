#include "player.hpp"

void player::Move() {

    // ‘€ì
    pos InputDirection{};
    bool NoDirection = false;
    if (this->Input->GetKey(KEY_INPUT_UP    )) InputDirection.AddY(-1);
    if (this->Input->GetKey(KEY_INPUT_DOWN  )) InputDirection.AddY( 1);
    if (this->Input->GetKey(KEY_INPUT_LEFT  )) InputDirection.AddX(-1);
    if (this->Input->GetKey(KEY_INPUT_RIGHT )) InputDirection.AddX( 1);

    // Direction <- InputDirection
    if (InputDirection.GetX() == 0.0 && InputDirection.GetY() == 0.0) NoDirection = true;
    else this->Sprite.SetDrectionFromPos(InputDirection);
    // Motion <- Direction
    this->Sprite.Motion.SetPos(0.0, 0.0);
    if (!NoDirection) {
        this->Sprite.Motion.AddPos(
            this->Sprite.GetPosFromDirection(4).GetX(),
            this->Sprite.GetPosFromDirection(4).GetY()
        );
    }
    // Pos += Motion
    this->Sprite.Pos.AddPos(this->Sprite.Motion.GetX(), this->Sprite.Motion.GetY());

}

void player::Draw() {

    DxLib::DrawGraph(this->Sprite.Pos.GetXInt(), this->Sprite.Pos.GetYInt(), this->Img["ring"], TRUE);
    DxLib::DrawGraph(this->Sprite.Pos.GetXInt(), this->Sprite.Pos.GetYInt(), this->Img["right"], TRUE);

    DxLib::DrawFormatString(16, 16, 0x000000, "Direction : %f (x:%f,y:%f)", this->Sprite.Direction, this->Sprite.GetPosFromDirection(4.0).GetX(), this->Sprite.GetPosFromDirection(4.0).GetY());

}

player::player(input *Input) {

    this->Input = Input;

    // ‰æ‘œ
    this->Img["ring"]   = DxLib::LoadGraph("data/stable/img/player/ring.png");
    this->Img["right"]  = DxLib::LoadGraph("data/stable/img/player/right.png");

}
