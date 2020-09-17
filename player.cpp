#include "player.hpp"

std::vector<bool> Where;

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

    // “–‚½‚è”»’è
    Where = this->Map->Collision(&(this->Sprite), this->BlockCol);

}

void player::Draw() {

    DxLib::DrawGraph(this->Sprite.Pos.GetXInt(), 96 + this->Sprite.Pos.GetYInt(), this->Img["ring"], TRUE);
    DxLib::DrawRotaGraph(
        this->Sprite.Pos.GetXInt() + this->ImgSize["right"].GetX() / 2, 96 + this->Sprite.Pos.GetYInt() + this->ImgSize["right"].GetY() / 2,
        1.0, (this->Sprite.GetPosFromDirection().GetX() >= 0.0) ? this->Sprite.Direction : this->Sprite.Direction + DX_PI,
        this->Img["right"], TRUE, (this->Sprite.GetPosFromDirection().GetX() < 0.0)
    );

    DxLib::DrawFormatString(16, 16, 0x000000, "Up:%s, Down:%s, Left:%s, Right:%s, Col:%s", (Where[MAP_UP] ? "true" : "false"), (Where[MAP_DOWN] ? "true" : "false"), (Where[MAP_LEFT] ? "true" : "false"), (Where[MAP_RIGHT] ? "true" : "false"), (Where[4] ? "true" : "false"));

}

player::player(input *Input, map *Map) {

    this->Input = Input;
    this->Map = Map;

    this->Sprite.Pos.SetPos(48 * 2 + 8, 48 * 6 + 8);
    this->Sprite.Size.SetPos(32, 32);

    // ‰æ‘œ
    int X, Y;

    this->Img["ring"]   = DxLib::LoadGraph("data/stable/img/player/ring.png");
    DxLib::GetGraphSize(this->Img["ring"], &X, &Y);
    this->ImgSize["ring"].SetPos(X, Y);

    this->Img["right"]  = DxLib::LoadGraph("data/stable/img/player/right.png");
    DxLib::GetGraphSize(this->Img["right"], &X, &Y);
    this->ImgSize["right"].SetPos(X, Y);

}
