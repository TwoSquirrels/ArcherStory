#include "player.hpp"

std::vector<bool> Where;

void player::Update() {

    //if (this->Input->GetKey(KEY_INPUT_D)) this->GiveSkill(this->DIAGONAL_ARROW);
    //if (this->Input->GetKey(KEY_INPUT_F)) this->GiveSkill(this->FRONT_ARROW);
    //if (this->Input->GetKey(KEY_INPUT_S)) this->GiveSkill(this->SIDE_ARROW);
    //if (this->Input->GetKey(KEY_INPUT_B)) this->GiveSkill(this->BACK_ARROW);
    //if (this->Input->GetKey(KEY_INPUT_M)) this->GiveSkill(this->MULTI_SHOT);
    //if (this->Input->GetKey(KEY_INPUT_P)) this->GiveSkill(this->PENETRATION);
    //if (this->Input->GetKey(KEY_INPUT_B)) this->GiveSkill(this->BOUND);
    //if (this->Input->GetKey(KEY_INPUT_P)) this->GiveSkill(this->POISON);
    //if (this->Input->GetKey(KEY_INPUT_H)) this->GiveSkill(this->HEADSHOT);
    // 移動
    this->Move();
    // 外に出たら次のステージへ
    if (!this->Map->GetInMap(this->Sprite)) {
        *this->Next = true;
        this->GodTime = 0;
        this->AttackCooldown = this->AttackCooldownMax * 3 / 4;
    }
    // 無敵時間更新   
    if (this->GodTime > 0) this->GodTime--;
    // 攻撃
    if (this->Sprite.Motion.GetX() != 0.0 || this->Sprite.Motion.GetY() != 0.0) {
        this->AttackCooldown = this->AttackCooldownMax * 3 / 4;
    }
    if (--this->AttackCooldown <= 0) {
        if (!this->Monster->empty()) {
            this->AttackCooldown = this->AttackCooldownMax;

            // 一番近くの敵を調べる
            int Nearest = -1;
            double NearestDistance = -1;
            for (int i = 0; i < this->Monster->size(); i++) {
                if ((*this->Monster)[i]->Use) {
                    double Distance = Distance2d((*this->Monster)[i]->Sprite.Pos, this->Sprite.Pos);
                    if (Nearest < 0 || Distance < NearestDistance) {
                        NearestDistance = Distance;
                        Nearest = i;
                    }
                }
            }

            // 撃つ
            if (Nearest >= 0) {
                this->Sprite.SetDrectionFromPos(pos(
                    (*this->Monster)[Nearest]->Sprite.GetCenterPos().GetX() - this->Sprite.GetCenterPos().GetX(),
                    (*this->Monster)[Nearest]->Sprite.GetCenterPos().GetY() - this->Sprite.GetCenterPos().GetY()
                ));
                // 斜め
                int DiagonalNum = this->GetSkill()[this->DIAGONAL_ARROW];
                for (int i = 0; i < DiagonalNum; i++) {
                    this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction + 0.5 + (-(DiagonalNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 2, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
                    this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction - 0.5 + (-(DiagonalNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 2, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
                }
                // 前
                int FrontNum = this->GetSkill()[this->FRONT_ARROW] + 1;
                for (int i = 0; i < FrontNum; i++) {
                    this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction + (-(FrontNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, (DxLib::GetRand(20 - 1) > 0) ? this->Attack : (this->Attack * 4096), this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
                }
                // 横
                int SideNum = this->GetSkill()[this->SIDE_ARROW];
                for (int i = 0; i < SideNum; i++) {
                    this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction + DX_PI * 0.5 + (-(SideNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 2, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
                    this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction - DX_PI * 0.5 + (-(SideNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 2, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
                }
                // 後ろ
                int BackNum = this->GetSkill()[this->BACK_ARROW];
                for (int i = 0; i < BackNum; i++) {
                    this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction + DX_PI + (-(BackNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 2, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
                }
                this->MultiShotCount = 0;
            }

        }
    }
    // マルチショット
    for (int i = 0; i < this->GetSkill()[this->MULTI_SHOT]; i++) {
        if (this->MultiShotCount == 4 + 4 * i) {
            // 斜め
            int DiagonalNum = this->GetSkill()[this->DIAGONAL_ARROW];
            for (int i = 0; i < DiagonalNum; i++) {
                this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction + 0.5 + (-(DiagonalNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 4, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
                this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction - 0.5 + (-(DiagonalNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 4, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
            }
            // 前
            int FrontNum = this->GetSkill()[this->FRONT_ARROW] + 1;
            for (int i = 0; i < FrontNum; i++) {
                this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction + (-(FrontNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 2, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
            }
            // 横
            int SideNum = this->GetSkill()[this->SIDE_ARROW];
            for (int i = 0; i < SideNum; i++) {
                this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction + DX_PI * 0.5 + (-(SideNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 4, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
                this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction - DX_PI * 0.5 + (-(SideNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 4, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
            }
            // 後ろ
            int BackNum = this->GetSkill()[this->BACK_ARROW];
            for (int i = 0; i < BackNum; i++) {
                this->Arrow->push_back(arrow(this->Sprite.GetCenterPos(), this->Sprite.Direction + DX_PI + (-(BackNum - 1) / 2.0 + i) * 0.05, this->Map, this->Monster, this->Attack / 4, this->GetSkill()[this->PENETRATION], this->GetSkill()[this->BOUND], this->GetSkill()[this->POISON] > 0, this->Graph, this->Config["Arrow"]));
            }
        }
    }
    this->MultiShotCount++;

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
    this->Sprite.Move();

    // 当たり判定
    Where = this->Map->Collision(&(this->Sprite), this->BlockCol);

}

void player::Draw() {

    DxLib::DrawGraph(this->StartPos.GetXInt(), 96 + this->Sprite.Pos.GetYInt(), this->Graph["ring"], TRUE);
    DxLib::DrawRotaGraph(
        this->StartPos.GetXInt() + this->GraphSize["right"].GetX() / 2, 96 + this->Sprite.Pos.GetYInt() + this->GraphSize["right"].GetY() / 2,
        1.0, (this->Sprite.GetPosFromDirection().GetX() >= 0.0) ? this->Sprite.Direction : this->Sprite.Direction + DX_PI,
        this->Graph["right"], TRUE, (this->Sprite.GetPosFromDirection().GetX() < 0.0)
    );
    // HPバー
    DxLib::DrawBox(
        this->StartPos.GetXInt() - 16, 96 + this->Sprite.Pos.GetYInt() - 16,
        this->StartPos.GetXInt() + 48, 96 + this->Sprite.Pos.GetYInt() - 8,
        0x000000, TRUE
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
    // ステータス
    DxLib::DrawFormatStringToHandle(16, 16 + 32 * 1, 0x000000, this->Font["Parameters"], "HP: %3d / %3d", this->HP, this->MaxHP);
    DxLib::DrawFormatStringToHandle(16, 16 + 32 * 2, 0x000000, this->Font["Parameters"], "攻撃力: %3d", this->Attack);
    DxLib::DrawFormatStringToHandle(16, 16 + 32 * 3, 0x000000, this->Font["Parameters"], "防御力: %3d", this->Defense);
    DxLib::DrawFormatStringToHandle(200, 16 + 32 * 1, 0x000000, this->Font["Parameters"], "移動速度: %2d", (int)(this->Speed * 4));
    DxLib::DrawFormatStringToHandle(200, 16 + 32 * 2, 0x000000, this->Font["Parameters"], "攻撃速度: %2d", this->AttackCooldownMax);
    DxLib::DrawFormatStringToHandle(200, 16 + 32 * 3, 0x000000, this->Font["Parameters"], "無敵時間: %2d", this->GodTimeMax);
    // スキルメッセージ
    if (this->SkillMessageCount > 0) {
        DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, (this->SkillMessageCount > 60) ? 191 : (this->SkillMessageCount * 191 / 60));
        DxLib::DrawStringToHandle(640 - 1.5 * DxLib::GetDrawFormatStringWidth(this->SkillMessage.c_str(), 0), 360 - 144, this->SkillMessage.c_str(), 0x0000ff, this->Font["NewSkill"]);
        DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
        this->SkillMessageCount--;
    }

}

void player::JoystickDraw() {

    if (this->Joystick.UseFlag) {
        DxLib::DrawExtendGraph(
            this->Joystick.Pos.GetXInt() - this->Joystick.Size, this->Joystick.Pos.GetYInt() - this->Joystick.Size,
            this->Joystick.Pos.GetXInt() + this->Joystick.Size, this->Joystick.Pos.GetYInt() + this->Joystick.Size,
            this->Graph["joystick_base"], TRUE
        );
        DxLib::DrawExtendGraph(
            this->Joystick.Stick.GetXInt() - this->Joystick.Size / 2, this->Joystick.Stick.GetYInt() - this->Joystick.Size / 2,
            this->Joystick.Stick.GetXInt() + this->Joystick.Size / 2, this->Joystick.Stick.GetYInt() + this->Joystick.Size / 2,
            this->Graph["joystick_stick"], TRUE
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

int player::GetMaxHP() {
    return this->MaxHP;
}

void player::Heal(int AddHP) {
    this->HP += AddHP;
    if (this->HP > this->MaxHP) this->HP = this->MaxHP;
}

void player::Damage(int Damage) {
    if (this->GodTime <= 0) {
        this->HP -= Damage - this->Defense;
        this->GodTime = this->GodTimeMax;
        if (this->HP <= 0) {
            // 死んじゃった！
            this->HP = 0;
            *this->Death = true;
        }
    }
}

std::map<player::skill, int> player::GetSkill() {
    std::map<skill, int> ForReturn;
    for (auto s : this->SkillLeft) ForReturn[s.first] = this->SkillMax[s.first] - s.second;
    return ForReturn;
}

std::map<player::skill, int> player::GetSkillLeft() {
    return this->SkillLeft;
}

bool player::GiveSkill(skill Skill) {

    if (this->SkillLeft[Skill] == 0) return false;
    this->SkillLeft[Skill]--;
    switch (Skill) {
    case this->HEAL:
        this->Heal(this->MaxHP * 0.5);
        this->SkillMessage = "HPが最大の半分回復した！";
        break;
    case this->HP_MAX_UP:
        this->MaxHP *= 1.3;
        this->HP *= 1.3;
        this->SkillMessage = "スキル「最大HP上昇」を習得した！";
        break;
    case this->ATTACK_SPEED_UP:
        this->AttackCooldownMax *= 0.8;
        this->SkillMessage = "スキル「攻撃速度上昇」を習得した！";
        break;
    case this->ATTACK_UP:
        this->Attack *= 1.3;
        this->SkillMessage = "スキル「攻撃力上昇」を習得した！";
        break;
    case this->DEFENSE_UP:
        this->Defense += 20;
        this->SkillMessage = "スキル「防御力上昇」を習得した！";
        break;
    case this->GOD_TIME_EXTENSION:
        this->GodTimeMax *= 1.5;
        this->SkillMessage = "スキル「無敵時間延長」を習得した！";
        break;
    case this->WALK_SPEED_UP:
        this->Speed *= 1.3;
        this->SkillMessage = "スキル「移動速度上昇」を習得した！";
        break;
    case this->DIAGONAL_ARROW:
        this->SkillMessage = "スキル「斜め矢+1」を習得した！";
        break;
    case this->FRONT_ARROW:
        this->SkillMessage = "スキル「前矢+1」を習得した！";
        break;
    case this->SIDE_ARROW:
        this->SkillMessage = "スキル「横矢+1」を習得した！";
        break;
    case this->BACK_ARROW:
        this->SkillMessage = "スキル「後ろ矢+1」を習得した！";
        break;
    case this->MULTI_SHOT:
        this->SkillMessage = "スキル「マルチショット」を習得した！";
        break;
    case this->PENETRATION:
        this->SkillMessage = "スキル「貫通ショット」を習得した！";
        break;
    case this->BOUND:
        this->SkillMessage = "スキル「バウンド壁」を習得した！";
        break;
    case this->POISON:
        this->SkillMessage = "スキル「毒タッチ」を習得した！";
        break;
    case this->HEADSHOT:
        this->SkillMessage = "スキル「ヘッドショット」を習得した！";
        break;
    }
    this->SkillMessageCount = 120;
    return true;

}

bool player::CheckHit(sprite Sprite, enum shape Shape) {

    switch (Shape) {
    case this->SQUARE:
        if (this->Sprite.GetSidePos(sprite::LEFT) >= Sprite.GetSidePos(sprite::RIGHT)) return false;
        if (this->Sprite.GetSidePos(sprite::RIGHT) <= Sprite.GetSidePos(sprite::LEFT)) return false;
        if (this->Sprite.GetSidePos(sprite::UP) >= Sprite.GetSidePos(sprite::DOWN)) return false;
        if (this->Sprite.GetSidePos(sprite::DOWN) <= Sprite.GetSidePos(sprite::UP)) return false;
        return true;
    case this->CIRCLE:
        if (Distance2d(
            this->Sprite.GetCenterPos(), Sprite.GetCenterPos()
        ) < (
            min(this->Sprite.Size.GetX(), this->Sprite.Size.GetY()) / 2 + min(Sprite.Size.GetX(), Sprite.Size.GetY()) / 2
        )) return true;
        return false;
    }
    
}

player::player() {}
player::player(input *Input, map *Map, std::vector<arrow> *Arrow, bool *Death, bool *Next, std::vector<monster *> *Monster, std::map<std::string, int> Graph, std::map<std::string, int> Font, json Config) {

    this->Input = Input;
    this->Map = Map;
    this->Arrow = Arrow;
    this->Death = Death;
    this->Next = Next;
    this->Monster = Monster;
    this->Graph = Graph;
    this->Font = Font;
    this->Config = Config;

    this->Joystick.Size = Config["JoystickSize"].get<int>();
    this->Speed = Config["Speed"].get<double>();
    this->Sprite.Pos = this->StartPos;
    this->Sprite.Size.SetPos(32, 32);
    this->MaxHP = Config["DefaultMaxHP"].get<int>();
    this->HP = this->MaxHP;
    this->GodTimeMax = Config["GodTimeMax"].get<int>();
    this->AttackCooldownMax = Config["AttackCooldownMax"].get<int>();
    this->AttackCooldown = this->AttackCooldownMax;
    this->Attack = Config["DefaultAttack"].get<int>();

    this->SkillLeft = this->SkillMax;

    // 画像サイズ取得
    int X, Y;

    DxLib::GetGraphSize(this->Graph["ring"], &X, &Y);
    this->GraphSize["ring"].SetPos(X, Y);

    DxLib::GetGraphSize(this->Graph["right"], &X, &Y);
    this->GraphSize["right"].SetPos(X, Y);

    DxLib::GetGraphSize(this->Graph["joystick_base"], &X, &Y);
    this->GraphSize["joystick_base"].SetPos(X, Y);

    DxLib::GetGraphSize(this->Graph["joystick_stick"], &X, &Y);
    this->GraphSize["joystick_stick"].SetPos(X, Y);

}
