#include "player.hpp"

std::vector<bool> Where;

void player::Update() {

    // 移動
    this->Move();
    // 無敵時間更新   
    if (this->GodTime > 0) this->GodTime--;

}

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

    DxLib::DrawGraph(this->StartPos.GetXInt(), 96 + this->Sprite.Pos.GetYInt(), this->Img["ring"], TRUE);
    DxLib::DrawRotaGraph(
        this->StartPos.GetXInt() + this->ImgSize["right"].GetX() / 2, 96 + this->Sprite.Pos.GetYInt() + this->ImgSize["right"].GetY() / 2,
        1.0, (this->Sprite.GetPosFromDirection().GetX() >= 0.0) ? this->Sprite.Direction : this->Sprite.Direction + DX_PI,
        this->Img["right"], TRUE, (this->Sprite.GetPosFromDirection().GetX() < 0.0)
    );
    // HPバー
    DxLib::DrawBox(
        this->StartPos.GetXInt() - 16, 96 + this->Sprite.Pos.GetYInt() - 16,
        this->StartPos.GetXInt() + 48, 96 + this->Sprite.Pos.GetYInt() - 8,
        0xff0000, TRUE
    );
    DxLib::DrawBox(
        this->StartPos.GetXInt() - 16, 96 + this->Sprite.Pos.GetYInt() - 16,
        this->StartPos.GetXInt() - 16 + 1 + (64 - 1) * this->HP / this->MaxHP, 96 + this->Sprite.Pos.GetYInt() - 8,
        0x00ff00, TRUE
    );
    DxLib::DrawBox(
        this->StartPos.GetXInt() - 16, 96 + this->Sprite.Pos.GetYInt() - 16,
        this->StartPos.GetXInt() + 48, 96 + this->Sprite.Pos.GetYInt() - 8,
        0x000000, FALSE
    );
    DxLib::DrawFormatString(
        this->StartPos.GetXInt() - 16, 96 + this->Sprite.Pos.GetYInt() - 32,
        0x000000, "%d", this->HP
    );

}

void player::JoystickDraw() {

    if (this->Joystick.UseFlag) {
        DxLib::DrawExtendGraph(
            this->Joystick.Pos.GetXInt() - this->Joystick.Size, this->Joystick.Pos.GetYInt() - this->Joystick.Size,
            this->Joystick.Pos.GetXInt() + this->Joystick.Size, this->Joystick.Pos.GetYInt() + this->Joystick.Size,
            this->Img["joystick_base"], TRUE
        );
        DxLib::DrawExtendGraph(
            this->Joystick.Stick.GetXInt() - this->Joystick.Size / 2, this->Joystick.Stick.GetYInt() - this->Joystick.Size / 2,
            this->Joystick.Stick.GetXInt() + this->Joystick.Size / 2, this->Joystick.Stick.GetYInt() + this->Joystick.Size / 2,
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

int player::GetHP() {
    return this->HP;
}

void player::Heel(int AddHP) {
    this->HP += AddHP;
    if (this->HP > this->MaxHP) this->HP = this->MaxHP;
}

void player::Damage(int Damage) {
    if (this->GodTime <= 0) {
        this->HP -= Damage;
        this->GodTime = this->GodTimeMax;
        if (this->HP <= 0) {
            // 死んじゃった！
            this->HP = 0;
            *this->Death = true;
        }
    }
}

bool player::CheckHit(sprite Sprite) {
    if (this->Sprite.GetSidePos(sprite().LEFT) >= Sprite.GetSidePos(sprite().RIGHT)) return false;
    if (this->Sprite.GetSidePos(sprite().RIGHT) <= Sprite.GetSidePos(sprite().LEFT)) return false;
    if (this->Sprite.GetSidePos(sprite().UP) >= Sprite.GetSidePos(sprite().DOWN)) return false;
    if (this->Sprite.GetSidePos(sprite().DOWN) <= Sprite.GetSidePos(sprite().UP)) return false;
    return true;
}

player::player() {}
player::player(input *Input, map *Map, bool *Death, json Config) {

    this->Input = Input;
    this->Map = Map;
    this->Death = Death;

    this->Joystick.Size = Config["JoystickSize"].get<int>();
    this->Speed = Config["Speed"].get<double>();
    this->Sprite.Pos = this->StartPos;
    this->Sprite.Size.SetPos(32, 32);
    this->MaxHP = Config["DefaultMaxHP"].get<int>();
    this->HP = this->MaxHP;
    this->GodTimeMax = Config["GodTimeMax"].get<int>();

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
