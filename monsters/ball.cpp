#include "ball.hpp"

void ball::Update() {
    if (this->Use) {

        switch (this->Type) {
        case this->JUMP:
            // Shadow移動
            this->Shadow.Move();
            // Ball移動
            this->Sprite.Pos.SetY(
                this->Shadow.Pos.GetY() - (this->Jump_a * pow(this->Jump_CenterX - this->Shadow.Pos.GetX(), 2) + this->Config["Jump"]["High"].get<double>())
            );
            // 半径がスピード/2以内なら消す
            if (Distance2d(this->Shadow.Pos, this->PlayerCenterPos) <= Config["Jump"]["Speed"].get<double>() / 2) this->Use = false;
            break;
        }
        // 移動
        this->Sprite.Move();
        // マップとの当たり判定
        switch (this->Type) {
        case this->JUMP:
            std::vector<bool> Col = this->Map->Collision(&this->Sprite, {
                false,  // air
                false,  // wall
                false,  // stone
                false,  // pond
            });
            for (bool b : Col) if (b == true) this->Use = false;
            break;
        }
        // ダメージ
        if (this->Type == this->TEST || this->Type == this->JUMP) {
            if (this->Player->CheckHit(this->Sprite, player().CIRCLE)) {
                this->Player->Damage(this->Attack);
                this->Use = false;
            }
        }
        // 外に出てたら消す
        if (!this->Map->GetInMap(this->Sprite)) this->Use = false;

    }
}

void ball::Draw(int Scroll) {
    if (this->Use) {

        switch (this->Type) {
        case this->TEST:
            DxLib::DrawCircle(
                -Scroll + this->Sprite.GetCenterPos().GetXInt(), 96 + this->Sprite.GetCenterPos().GetYInt(),
                4, 0xff0000, TRUE
            );
            break;
        case this->JUMP:
            DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
            DxLib::DrawGraph(
                -Scroll + this->Shadow.GetCenterPos().GetXInt() - this->GraphSize["ball_shadow"].GetXInt() / 2,
                96 + this->Shadow.GetCenterPos().GetYInt() - this->GraphSize["ball_shadow"].GetYInt() / 2,
                this->Graph["ball_shadow"], TRUE
            );
            DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
            DxLib::DrawGraph(
                -Scroll + this->Sprite.GetCenterPos().GetXInt() - this->GraphSize["ball_jumping"].GetXInt() / 2,
                96 + this->Sprite.GetCenterPos().GetYInt() - this->GraphSize["ball_jumping"].GetYInt() / 2,
                this->Graph["ball_jumping"], TRUE
            );
            break;
        }

    }
}

ball::ball() {
    this->Use = false;
}
// TEST, JUMP
ball::ball(enum type Type, int Attack, map *Map, player *Player, pos Pos, pos PlayerCenterPos, std::map<std::string, int> Graph, json Config) {

    this->Use = true;
    this->Type = Type;
    this->Attack = Attack;
    this->Map = Map;
    this->Player = Player;
    this->Sprite.Pos = Pos;
    this->PlayerCenterPos = PlayerCenterPos;
    this->Graph = Graph;
    this->Config = Config;
    switch (Type) {
    case TEST:
        this->Sprite.SetDrectionFromPos(pos(
            this->PlayerCenterPos.GetX() - this->Sprite.Pos.GetX(),
            this->PlayerCenterPos.GetY() - this->Sprite.Pos.GetY()
        ));
        this->Sprite.Motion = this->Sprite.GetPosFromDirection(4.0);
        break;
    case JUMP:
        this->Jump_a = -4 * this->Config["Jump"]["High"].get<double>() / std::pow(this->PlayerCenterPos.GetX() - this->Sprite.Pos.GetX(), 2);
        this->Shadow.Pos = this->Sprite.Pos;
        this->Shadow.SetDrectionFromPos(pos(
            this->PlayerCenterPos.GetX() - this->Sprite.Pos.GetX(),
            this->PlayerCenterPos.GetY() - this->Sprite.Pos.GetY()
        ));
        this->Shadow.Motion = this->Shadow.GetPosFromDirection(Config["Jump"]["Speed"].get<double>());
        this->Sprite.Motion.SetX(Shadow.Motion.GetX());
        this->Jump_CenterX = (this->Sprite.Pos.GetX() + this->PlayerCenterPos.GetX()) / 2;
        break;
    }

    // 画像サイズ取得
    int X, Y;

    DxLib::GetGraphSize(this->Graph["ball_jumping"], &X, &Y);
    this->GraphSize["ball_jumping"].SetPos(X, Y);

    DxLib::GetGraphSize(this->Graph["ball_shadow"], &X, &Y);
    this->GraphSize["ball_shadow"].SetPos(X, Y);

}
