#include "player.hpp"

std::vector<bool> Where;

void player::Move() {

    // 操作
    pos InputDirection{};
    this->KeyInput(&InputDirection);
    this->JoystickInput(&InputDirection);

    // Direction <- InputDirection
    bool NoDirection = false;
    if (InputDirection.GetX() == 0.0 && InputDirection.GetY() == 0.0) NoDirection = true;
    else this->Sprite.SetDrectionFromPos(InputDirection);
    // Motion <- Direction
    this->Sprite.Motion.SetPos(0.0, 0.0);
    if (!NoDirection) {
        this->Sprite.Motion.AddPos(
            this->Sprite.GetPosFromDirection(this->Speed).GetX(),
            this->Sprite.GetPosFromDirection(this->Speed).GetY()
        );
    }
    // Pos += Motion
    this->Sprite.Pos.AddPos(this->Sprite.Motion.GetX(), this->Sprite.Motion.GetY());

    // 当たり判定
    Where = this->Map->Collision(&(this->Sprite), this->BlockCol);

}

void player::Draw() {

    DxLib::DrawGraph(this->Sprite.Pos.GetXInt(), 96 + this->Sprite.Pos.GetYInt(), this->Img["ring"], TRUE);
    DxLib::DrawRotaGraph(
        this->Sprite.Pos.GetXInt() + this->ImgSize["right"].GetX() / 2, 96 + this->Sprite.Pos.GetYInt() + this->ImgSize["right"].GetY() / 2,
        1.0, (this->Sprite.GetPosFromDirection().GetX() >= 0.0) ? this->Sprite.Direction : this->Sprite.Direction + DX_PI,
        this->Img["right"], TRUE, (this->Sprite.GetPosFromDirection().GetX() < 0.0)
    );

}

void player::JoystickDraw() {

    if (this->Joystick.UseFlag) {
        DxLib::DrawGraph(
            this->Joystick.Pos.GetXInt() - this->ImgSize["joystick_base"].GetXInt() / 2,
            this->Joystick.Pos.GetYInt() - this->ImgSize["joystick_base"].GetYInt() / 2,
            this->Img["joystick_base"], TRUE
        );
        DxLib::DrawGraph(
            this->Joystick.Stick.GetXInt() - this->ImgSize["joystick_stick"].GetXInt() / 2,
            this->Joystick.Stick.GetYInt() - this->ImgSize["joystick_stick"].GetYInt() / 2,
            this->Img["joystick_stick"], TRUE
        );
    }

}

void player::KeyInput(pos *InputDirection) {

    if (this->Input->GetKey(KEY_INPUT_UP   )) InputDirection->AddY(-1);
    if (this->Input->GetKey(KEY_INPUT_DOWN )) InputDirection->AddY( 1);
    if (this->Input->GetKey(KEY_INPUT_LEFT )) InputDirection->AddX(-1);
    if (this->Input->GetKey(KEY_INPUT_RIGHT)) InputDirection->AddX( 1);

}

void player::JoystickInput(pos *InputDirection) {

    // ジョイスティック生成
    if (this->Input->GetMouseDown(MOUSE_INPUT_LEFT)) {
        this->Joystick.UseFlag = true;
        this->Joystick.Pos = this->Input->GetMousePos();
    }

    // ジョイスティック削除
    if (this->Input->GetMouseUp(MOUSE_INPUT_LEFT)) {
        this->Joystick.UseFlag = false;
    }

    // 反映
    this->Joystick.Stick.SetPos(0, 0);
    if (this->Joystick.UseFlag) {

        double Direction = std::atan2(
            this->Input->GetMousePos().GetY() - this->Joystick.Pos.GetY(),
            this->Input->GetMousePos().GetX() - this->Joystick.Pos.GetX()
        );
        if (Distance2d(this->Joystick.Pos, this->Input->GetMousePos()) > this->Joystick.Size / 16) {
            InputDirection->AddPos(
                this->Speed * std::cos(Direction) / this->Joystick.Size,
                this->Speed * std::sin(Direction) / this->Joystick.Size
            );
        }

        if (Distance2d(this->Joystick.Pos, this->Input->GetMousePos()) > this->Joystick.Size) {
            this->Joystick.Stick.SetPos(
                this->Joystick.Pos.GetX() + this->Joystick.Size * std::cos(Direction),
                this->Joystick.Pos.GetY() + this->Joystick.Size * std::sin(Direction)
            );
        }
        else {
            this->Joystick.Stick = this->Input->GetMousePos();
        }

    }

}

player::player(input *Input, map *Map) {

    this->Input = Input;
    this->Map = Map;

    this->Sprite.Pos.SetPos(48 * 2 + 8, 48 * 6 + 8);
    this->Sprite.Size.SetPos(32, 32);

    // 画像
    int X, Y;

    this->Img["ring"]   = DxLib::LoadGraph("data/stable/img/player/ring.png");
    DxLib::GetGraphSize(this->Img["ring"], &X, &Y);
    this->ImgSize["ring"].SetPos(X, Y);

    this->Img["right"]  = DxLib::LoadGraph("data/stable/img/player/right.png");
    DxLib::GetGraphSize(this->Img["right"], &X, &Y);
    this->ImgSize["right"].SetPos(X, Y);

    this->Img["joystick_base"] = DxLib::LoadGraph("data/stable/img/player/joystick_base.png");
    DxLib::GetGraphSize(this->Img["joystick_base"], &X, &Y);
    this->ImgSize["joystick_base"].SetPos(X, Y);

    this->Img["joystick_stick"] = DxLib::LoadGraph("data/stable/img/player/joystick_stick.png");
    DxLib::GetGraphSize(this->Img["joystick_stick"], &X, &Y);
    this->ImgSize["joystick_stick"].SetPos(X, Y);

}
