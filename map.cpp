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
                    Sprite->Pos.GetY() + Sprite->Size.GetY() > 48 * i &&
                    Sprite->Pos.GetY() < 48 * i + 48 &&
                    Sprite->Pos.GetX() + Sprite->Size.GetX() > 48 * j &&
                    Sprite->Pos.GetX() < 48 * j + 48
                    ) {

                    // どこからあたったか

                    if (Sprite->Motion.GetX() == 0) {       // 真上下から

                        if (Sprite->Motion.GetY() >= 0) {   // 真上から

                            Sprite->Pos.SetY(48 * i - Sprite->Size.GetY());
                            //Sprite->Motion.SetY(0);
                            Where[this->UP] = true;

                        }
                        else {                            // 真下から

                            Sprite->Pos.SetY(48 * i + 48);
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
                        double y = a * (48 * j - Sprite->Size.GetX()) + b;

                        if (y <= 48 * i - Sprite->Size.GetY()) {    // 上から

                            if (i == 0 || !Block[this->Map[i - 1][j]]) {    // 上にブロックがないときのみ

                                Sprite->Pos.SetY(48 * i - Sprite->Size.GetY());
                                //Sprite->Motion.SetY(0);
                                Where[this->UP] = true;

                            }

                        }
                        else if (y > 48 * i + 48 + 1) {                // 下から

                            if (i + 1 == this->Map.size() || !Block[this->Map[i + 1][j]]) { // 下にブロックがないときのみ

                                Sprite->Pos.SetY(48 * i + 48);
                                //Sprite->Motion.SetY(0);
                                Where[this->DOWN] = true;

                            }

                        }
                        else {                                      // 左から

                            if (j == 0 || !Block[this->Map[i][j - 1]]) {    // 左にブロックがないときのみ

                                Sprite->Pos.SetX(48 * j - Sprite->Size.GetX());
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
                        double y = a * (48 * j + 48) + b;

                        if (y <= 48 * i - Sprite->Size.GetY()) {    // 上から

                            if (i == 0 || !Block[this->Map[i - 1][j]]) {    // 上にブロックがないときのみ

                                Sprite->Pos.SetY(48 * i - Sprite->Size.GetY());
                                //Sprite->Motion.SetY(0);
                                Where[this->UP] = true;

                            }

                        }
                        else if (y >= 48 * i + 48) {                // 下から

                            if (i + 1 == this->Map.size() || !Block[this->Map[i + 1][j]]) { // 下にブロックがないときのみ

                                Sprite->Pos.SetY(48 * i + 48);
                                //Sprite->Motion.SetY(0);
                                Where[this->DOWN] = true;

                            }

                        }
                        else {                                      // 右から

                            if (j + 1 == this->Map[0].size() || !Block[this->Map[i][j + 1]]) {  // 右にブロックがないときのみ

                                Sprite->Pos.SetX(48 * j + 48);
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
            switch (this->Map[i][j]) {
            case 0:
                DxLib::DrawBox(
                    -Scroll + 48 * j,      96 + 48 * i,
                    -Scroll + 48 * j + 48, 96 + 48 * i + 48,
                    0xFFFFFF, FALSE
                );
                break;
            case 1:
                DxLib::DrawBox(
                    -Scroll + 48 * j,      96 + 48 * i,
                    -Scroll + 48 * j + 48, 96 + 48 * i + 48,
                    0x808080, TRUE
                );
                break;
            }
        }
    }
}

map::map() {
    std::vector<std::vector<int>> Tmp = {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    };
    this->Map = Tmp;
}
