#include "map.hpp"

void map::LoadFromFile(std::string FilePath) {

}

std::vector<bool> map::Collision(sprite *Sprite, std::vector<bool> Block) {

    // 処理軽減のため、範囲指定

    int Start_i = std::floor((Sprite->Pos.GetY() - 1) / 48);
    if (Start_i < 0) Start_i = 0;
    if (Start_i >= this->Map.size()) Start_i = this->Map.size() - 1;

    int Start_j = std::floor((Sprite->Pos.GetX() - 1) / 48);
    if (Start_j < 0) Start_j = 0;
    if (Start_j >= this->Map[0].size()) Start_j = this->Map[0].size() - 1;

    int End_i = std::ceil((Sprite->Pos.GetY() + Sprite->Size.GetY() + 1) / 48);
    if (End_i < 0) End_i = 0;
    if (End_i >= this->Map.size()) End_i = this->Map.size() - 1;

    int End_j = std::ceil((Sprite->Pos.GetX() + Sprite->Size.GetX() + 1) / 48);
    if (End_j < 0) End_j = 0;
    if (End_j >= this->Map[0].size()) End_j = this->Map[0].size() - 1;

    // return用
    std::vector<bool> Where(4);
    Where[this->UP   ] = false;
    Where[this->DOWN ] = false;
    Where[this->LEFT ] = false;
    Where[this->RIGHT] = false;

    // メイン

    for (int i = Start_i; i <= End_i; i++) {
        for (int j = Start_j; j <= End_j; j++) {

            // これが該当するブロックか
            if (Block[this->Map[i][j]]) {

                if (    // 当たってるか
                    Sprite->Pos.GetY() + Sprite->Size.GetY() > 48.0 * i &&
                    Sprite->Pos.GetY() < 48.0 * i + 48 &&
                    Sprite->Pos.GetX() + Sprite->Size.GetX() > 48.0 * j &&
                    Sprite->Pos.GetX() < 48.0 * j + 48
                    ) {

                    // どこからあたったか

                    if (Sprite->Motion.GetX() == 0) {       // 真上下から

                        if (Sprite->Motion.GetY() >= 0) {   // 真上から

                            Sprite->Pos.SetY(48.0 * i - Sprite->Size.GetY());
                            //Sprite->Motion.SetY(0);
                            Where[this->UP] = true;

                        }
                        else {                            // 真下から

                            Sprite->Pos.SetY(48.0 * i + 48);
                            //Sprite->Motion.SetY(0);
                            Where[this->DOWN] = true;

                        }

                    }
                    else if (Sprite->Motion.GetX() > 0) { // 左(上下)から

                        // y=ax+b と置いて、a(傾き)を求める
                        double a = Sprite->Motion.GetY() / Sprite->Motion.GetX();
                        // (x,y)にPosを代入してbを求める (b=y-ax)
                        double b = Sprite->Pos.GetY() - a * Sprite->Pos.GetX();

                        // y=ax+b に x=(Blockの左のx座標)-(Spriteのxのサイズ) を代入
                        double y = a * (48.0 * j - Sprite->Size.GetX()) + b;

                        if (y <= 48.0 * i - Sprite->Size.GetY()) {    // 上から

                            if (i == 0 || !Block[this->Map[i - 1][j]]) {    // 上にブロックがないときのみ

                                Sprite->Pos.SetY(48.0 * i - Sprite->Size.GetY());
                                //Sprite->Motion.SetY(0);
                                Where[this->UP] = true;

                            }

                        }
                        else if (y > 48.0 * i + 48 + 1) {                // 下から

                            if (i + 1 == this->Map.size() || !Block[this->Map[i + 1][j]]) { // 下にブロックがないときのみ

                                Sprite->Pos.SetY(48.0 * i + 48);
                                //Sprite->Motion.SetY(0);
                                Where[this->DOWN] = true;

                            }

                        }
                        else {                                      // 左から

                            if (j == 0 || !Block[this->Map[i][j - 1]]) {    // 左にブロックがないときのみ

                                Sprite->Pos.SetX(48.0 * j - Sprite->Size.GetX());
                                //Sprite->Motion.SetX(0);
                                Where[this->LEFT] = true;

                            }

                        }

                    }
                    else {                                // 右(上下)から

                        // y=ax+b と置いて、a(傾き)を求める
                        double a = Sprite->Motion.GetY() / Sprite->Motion.GetX();
                        // (x,y)にPosを代入してbを求める (b=y-ax)
                        double b = Sprite->Pos.GetY() - a * Sprite->Pos.GetX();

                        // y=ax+b に x=(Blockの右のx座標) を代入
                        double y = a * (48.0 * j + 48) + b;

                        if (y <= 48.0 * i - Sprite->Size.GetY()) {    // 上から

                            if (i == 0 || !Block[this->Map[i - 1][j]]) {    // 上にブロックがないときのみ

                                Sprite->Pos.SetY(48.0 * i - Sprite->Size.GetY());
                                //Sprite->Motion.SetY(0);
                                Where[this->UP] = true;

                            }

                        }
                        else if (y >= 48.0 * i + 48) {                // 下から

                            if (i + 1 == this->Map.size() || !Block[this->Map[i + 1][j]]) { // 下にブロックがないときのみ

                                Sprite->Pos.SetY(48.0 * i + 48);
                                //Sprite->Motion.SetY(0);
                                Where[this->DOWN] = true;

                            }

                        }
                        else {                                      // 右から

                            if (j + 1 == this->Map[0].size() || !Block[this->Map[i][j + 1]]) {  // 右にブロックがないときのみ

                                Sprite->Pos.SetX(48.0 * j + 48);
                                //Sprite->Motion.SetX(0);
                                Where[this->RIGHT] = true;

                            }

                        }

                    }

                }

            }

        }
    }

    return Where;

}

std::vector<int> map::GetSidePos() {
    std::vector<int> SidePos(4);
    SidePos[this->LEFT] = 0;
    SidePos[this->RIGHT] = 48 * this->Map[0].size();
    SidePos[this->UP] = 0;
    SidePos[this->DOWN] = 48 * this->Map.size();
    return SidePos;
}
bool map::GetInMap(sprite Sprite) {
    std::vector<int> SidePos = this->GetSidePos();
    if (Sprite.GetSidePos(Sprite.LEFT ) < SidePos[this->LEFT ]) return false;
    if (Sprite.GetSidePos(Sprite.RIGHT) > SidePos[this->RIGHT]) return false;
    if (Sprite.GetSidePos(Sprite.UP   ) < SidePos[this->UP   ]) return false;
    if (Sprite.GetSidePos(Sprite.DOWN ) > SidePos[this->DOWN ]) return false;
    return true;
}

void map::Draw(int Scroll) {

    // 720 - (48*11=528) = 192
    for (int i = 0; i < this->Map.size(); i++) {
        for (int j = 0; j < this->Map[i].size(); j++) {
            if (j % 2 == i % 2) DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i, this->Graph["map"]["ground"], TRUE);
            switch (this->Map[i][j]) {
            case this->STONE:
                DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i, this->Graph["map"]["stone"], TRUE);
                break;
            case this->POND:
                // 左上
                if (i - 1 >= 0 && j - 1 >= 0 && this->Map[i - 1][j] == 3 && this->Map[i][j - 1] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i, this->Graph["map"]["pond11"], TRUE);
                } else if (i - 1 >= 0 && this->Map[i - 1][j] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i, this->Graph["map"]["pond10"], TRUE);
                } else if (j - 1 >= 0 && this->Map[i][j - 1] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i, this->Graph["map"]["pond01"], TRUE);
                } else {
                    DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i, this->Graph["map"]["pond00"], TRUE);
                }
                // 右上
                if (i - 1 >= 0 && j + 1 <= this->Map[0].size() - 1 && this->Map[i - 1][j] == 3 && this->Map[i][j + 1] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j + 48 / 2, 96 + 48 * i, this->Graph["map"]["pond11"], TRUE);
                } else if (i - 1 >= 0 && this->Map[i - 1][j] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j + 48 / 2, 96 + 48 * i, this->Graph["map"]["pond12"], TRUE);
                } else if (j + 1 <= this->Map[0].size() - 1 && this->Map[i][j + 1] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j + 48 / 2, 96 + 48 * i, this->Graph["map"]["pond01"], TRUE);
                } else {
                    DxLib::DrawGraph(-Scroll + 48 * j + 48 / 2, 96 + 48 * i, this->Graph["map"]["pond02"], TRUE);
                }
                // 左下
                if (i + 1 <= this->Map.size() - 1 && j - 1 >= 0 && this->Map[i + 1][j] == 3 && this->Map[i][j - 1] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i + 48 / 2, this->Graph["map"]["pond11"], TRUE);
                } else if (i + 1 <= this->Map.size() - 1 && this->Map[i + 1][j] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i + 48 / 2, this->Graph["map"]["pond10"], TRUE);
                } else if (j - 1 >= 0 && this->Map[i][j - 1] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i + 48 / 2, this->Graph["map"]["pond21"], TRUE);
                } else {
                    DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i + 48 / 2, this->Graph["map"]["pond20"], TRUE);
                }
                // 右下
                if (i + 1 <= this->Map.size() - 1 && j + 1 <= this->Map[0].size() - 1 && this->Map[i + 1][j] == 3 && this->Map[i][j + 1] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j + 48 / 2, 96 + 48 * i + 48 / 2, this->Graph["map"]["pond11"], TRUE);
                } else if (i + 1 <= this->Map.size() - 1 && this->Map[i + 1][j] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j + 48 / 2, 96 + 48 * i + 48 / 2, this->Graph["map"]["pond12"], TRUE);
                } else if (j + 1 <= this->Map[0].size() - 1 && this->Map[i][j + 1] == 3) {
                    DxLib::DrawGraph(-Scroll + 48 * j + 48 / 2, 96 + 48 * i + 48 / 2, this->Graph["map"]["pond21"], TRUE);
                } else {
                    DxLib::DrawGraph(-Scroll + 48 * j + 48 / 2, 96 + 48 * i + 48 / 2, this->Graph["map"]["pond22"], TRUE);
                }
                break;
            }
        }
    }

    // wall
    for (int i = 0; 96 + 48 * i + 48 > 0; i--) {
        // 左上
        for (int j = 0; -Scroll + 48 * j + 48 > 0; j--) {
            DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i, this->Graph["map"]["wall"], TRUE);
        }
        // 右上
        for (int j = 0; -Scroll + 48 * j < 1280; j++) {
            DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i, this->Graph["map"]["wall"], TRUE);
        }
    }
    for (int i = 0; 96 + 48 * i < 720; i++) {
        // 左下
        for (int j = 0; -Scroll + 48 * j + 48 > 0; j--) {
            DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i, this->Graph["map"]["wall"], TRUE);
        }
        // 右下
        for (int j = 0; -Scroll + 48 * j < 1280; j++) {
            if (!(j < this->Map[0].size() - 1 && i < this->Map.size() - 1) && !(j == this->Map[0].size() - 1 && i >= 5 && i <= 7)) {
                DxLib::DrawGraph(-Scroll + 48 * j, 96 + 48 * i, this->Graph["map"]["wall"], TRUE);
            }
        }
    }
    // 枠
    for (int i = 1; i <= 4; i++) {
        //DxLib::DrawBox(-Scroll + 48 - i, 96 + 48 - i, -Scroll + 48 * this->Map[0].size() - 48 + i, 96 + 48 * this->Map.size() - 48 + i, 0x000000, FALSE);
        int Up = 96 + 48 - i, Down = 96 + 48 * this->Map.size() - 48 + i, Left = -Scroll + 48 - i, Right = -Scroll + 48 * this->Map[0].size() - 48 + i;
        DxLib::DrawLine(Left, Up, Right, Up, 0x000000);
        DxLib::DrawLine(Left, Up, Left, Down, 0x000000);
        DxLib::DrawLine(Left, Down, Right, Down, 0x000000);
        DxLib::DrawLine(Right, Up, Right, Up + 48 * 4, 0x000000);
        DxLib::DrawLine(Right, Down - 48 * 4, Right, Down, 0x000000);
    }
    // シャッター
    for (int i = 0; i < 3; i++) DxLib::DrawGraph(-Scroll + 48 * this->Map[0].size() - 48 + this->ClearCount, 96 + 48 * (5 + i), this->Graph["map"]["shutter"], TRUE);
    DxLib::DrawGraph(-Scroll + 48 * this->Map[0].size() - 48, 96 + 48 * 4, this->Graph["map"]["shutter_gate"], TRUE);
    if (this->ClearCount > 0 && this->ClearCount < 48) this->ClearCount++;

}

int map::GetStage() {

    return this->Stage;

}

void map::Clear() {
    if (this->ClearCount == 0) {

        this->Map[5][this->Map[0].size() - 1] = this->AIR;
        this->Map[6][this->Map[0].size() - 1] = this->AIR;
        this->Map[7][this->Map[0].size() - 1] = this->AIR;
        this->ClearCount = 1;

    }
}

void map::NextStage() {

    this->Stage++;
    this->ClearCount = 0;

    // マップ選択
    std::vector<int> CanStage;
    for (int i = 0; i < this->Maps["Maps"].size(); i++) {
        int Min = this->Maps["Maps"][i]["StageMin"].get<int>();
        int Max = this->Maps["Maps"][i]["StageMax"].get<int>();
        if (
            (this->Maps["Maps"][i]["CanDuplicate"].get<bool>() || !this->Maps["Maps"][i]["Done"].get<bool>()) &&
            (Min == 0 || Min <= this->Stage) && (Max == 0 || Max >= this->Stage)
            ) {
            CanStage.push_back(i);
        }
    }
    if (CanStage.empty()) {
        this->NextStage();
        return;
    }
    int FixedStage = CanStage[DxLib::GetRand(CanStage.size() - 1)];
    this->Maps["Maps"][FixedStage]["Done"] = true;

    // マップ変換
    int YSize = this->Maps["Maps"][FixedStage]["Blocks"].size();
    int XSize = this->Maps["Maps"][FixedStage]["Blocks"][0].size();
    this->Map.resize(YSize + 2);
    for (int i = 0; i < YSize + 2; i++) {
        this->Map[i].resize(XSize + 2);
        for (int j = 0; j < XSize + 2; j++) this->Map[i][j] = this->WALL;
    }
    for (int i = 0; i < YSize; i++) for (int j = 0; j < XSize; j++) {
        switch (this->Maps["Maps"][FixedStage]["Blocks"][i][j].get<int>()) {
        case 0:
            this->Map[i + 1][j + 1] = this->AIR;
            break;
        case 1:
            this->Map[i + 1][j + 1] = this->STONE;
            break;
        case 2:
            this->Map[i + 1][j + 1] = this->POND;
            break;
        }
    }

    // モンスター
    FlowerPlant->clear();
    Slime->clear();
    for (int i = 0; i < this->Maps["Maps"][FixedStage]["Monsters"].size(); i++) {
        int Monster = DxLib::GetRand(this->MONSTERS_NUM - 1);
        std::string MonsterName = this->Maps["Maps"][FixedStage]["Monsters"][i]["Type"].get<std::string>();
        if (MonsterName == "FlowerPlant") Monster = this->FLOWER_PLANT;
        if (MonsterName == "Slime") Monster = this->SLIME;
        switch (Monster) {
        case this->FLOWER_PLANT:
            FlowerPlant->push_back(flower_plant(
                this->Ball,
                pos(48.0 + DxLib::GetRand(1072), 48.0 + DxLib::GetRand(496)),
                100,
                100,
                this,
                this->Player,
                this->Graph["monsters"],
                this->Config
            ));
            break;
        case this->SLIME:
            Slime->push_back(slime(
                pos(48.0 + DxLib::GetRand(1072), 48.0 + DxLib::GetRand(496)),
                100,
                2,
                this,
                this->Player,
                this->Graph["monsters"],
                this->Config
            ));
            break;
        }
    }

    // テキスト


}

map::map() {}
map::map(json Maps, std::map<std::string, std::map<std::string, int>> Graph, std::vector<flower_plant> *FlowerPlant, std::vector<slime> *Slime, std::vector<ball> *Ball, player *Player, json Config) {

    this->Maps = Maps;
    this->Graph = Graph;

    this->FlowerPlant = FlowerPlant;
    this->Slime = Slime;

    this->Ball = Ball;
    this->Player = Player;
    this->Config = Config;

    for (int i = 0; i < this->Maps["Maps"].size(); i++) this->Maps["Maps"][i]["Done"] = false;

    //std::vector<std::vector<int>> Tmp = {
    //    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    //    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    //};
    //this->Map = Tmp;
    this->NextStage();

}
