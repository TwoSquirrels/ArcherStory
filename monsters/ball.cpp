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
        std::vector<bool> Col;
        switch (this->Type) {
        case this->STONE:
            Col = this->Map->Collision(&this->Sprite, {
                    false,  // air
                    true,   // wall
                    true,   // stone
                    false,  // pond
                });
            for (bool b : Col) if (b == true) this->Use = false;
            break;
        case this->JUMP:
            Col = this->Map->Collision(&this->Sprite, {
                    false,  // air
                    false,  // wall
                    false,  // stone
                    false,  // pond
                });
            for (bool b : Col) if (b == true) this->Use = false;
            break;
        case this->BEAM:
            Col = this->Map->Collision(&this->Sprite, {
                    false,  // air
                    true,   // wall
                    false,  // stone
                    false,  // pond
                });
            for (bool b : Col) if (b == true) this->Use = false;
            break;
        }
        // ダメージ
        if (this->Type == this->STONE || this->Type == this->JUMP || this->Type == this->BEAM) {
            if (this->Player->CheckHit(this->Sprite, player().CIRCLE)) {
                this->Player->Damage(this->Attack);
                this->Use = false;
            }
        }
        // 外に出てたら消す
        if (!this->Map->GetInMap(this->Sprite)) this->Use = false;
        this->Count++;

    }
}

void ball::Draw(int Scroll) {
    if (this->Use) {

        switch (this->Type) {
        case this->STONE:
            DxLib::DrawRotaGraph(
                -Scroll + this->Sprite.GetCenterPos().GetXInt(),
                96 + this->Sprite.GetCenterPos().GetYInt(),
                2.0, this->Count / 4.0, this->Graph["ball_stone"], TRUE
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
        case this->BEAM:
            DxLib::DrawRotaGraph(
                -Scroll + this->Sprite.GetCenterPos().GetXInt(),
                96 + this->Sprite.GetCenterPos().GetYInt(),
                2.0, 0.0, this->Graph["ball_beam"], TRUE
            );
            break;
        }

    }
}

ball::ball() {
    this->Use = false;
}
// STONE, BEAM
ball::ball(enum type Type, int Attack, map *Map, player *Player, pos Pos, double Direction, std::map<std::string, int> Graph, json Config) {

    this->Use = true;
    this->Type = Type;
    this->Attack = Attack;
    this->Map = Map;
    this->Player = Player;
    this->Sprite.Pos = Pos;
    this->Sprite.Direction = Direction;
    this->Graph = Graph;
    this->Config = Config;

    // 画像サイズ取得用
    int X, Y;

    switch (Type) {
        case this->STONE:

            this->Sprite.Motion = this->Sprite.GetPosFromDirection(Config["Stone"]["Speed"].get<double>());

            // 画像サイズ取得
            DxLib::GetGraphSize(this->Graph["ball_stone"], &X, &Y);
            this->GraphSize["ball_stone"].SetPos(X, Y);

            break;
        case this->BEAM:

            this->Sprite.Motion = this->Sprite.GetPosFromDirection(Config["Beam"]["Speed"].get<double>());

            // 画像サイズ取得
            DxLib::GetGraphSize(this->Graph["ball_beam"], &X, &Y);
            this->GraphSize["ball_beam"].SetPos(X, Y);

            break;
        default: throw this->WRONG_CONSTRUCTOR;
    }

}
// JUMP
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

    // 画像サイズ取得用
    int X, Y;

    switch (Type) {
    case this->JUMP:

        this->Jump_a = -4 * this->Config["Jump"]["High"].get<double>() / std::pow(this->PlayerCenterPos.GetX() - this->Sprite.Pos.GetX(), 2);
        this->Shadow.Pos = this->Sprite.Pos;
        this->Shadow.SetDirectionFromPos(pos(
            this->PlayerCenterPos.GetX() - this->Sprite.Pos.GetX(),
            this->PlayerCenterPos.GetY() - this->Sprite.Pos.GetY()
        ));
        this->Shadow.Motion = this->Shadow.GetPosFromDirection(Config["Jump"]["Speed"].get<double>());
        this->Sprite.Motion.SetX(Shadow.Motion.GetX());
        this->Jump_CenterX = (this->Sprite.Pos.GetX() + this->PlayerCenterPos.GetX()) / 2;

        // 画像サイズ取得
        DxLib::GetGraphSize(this->Graph["ball_jumping"], &X, &Y);
        this->GraphSize["ball_jumping"].SetPos(X, Y);
        DxLib::GetGraphSize(this->Graph["ball_shadow"], &X, &Y);
        this->GraphSize["ball_shadow"].SetPos(X, Y);

        break;
    default: throw this->WRONG_CONSTRUCTOR;
    }

}
