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
    Where[MAP_UP   ] = false;
    Where[MAP_DOWN ] = false;
    Where[MAP_LEFT ] = false;
    Where[MAP_RIGHT] = false;

    // メイン

    for (int i = Start_i; i <= End_i; i++) {
        for (int j = Start_j; j <= Start_j; j++) {

            // これが該当するブロックか
            for (int k = 0; k < Block.size(); k++) {
                if (this->Map[i][j] == Block[k]) {

                    /*if () { // 左上

                    }*/

                    break;
                }
            }

        }
    }

    return Where;

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
                    Scroll + 48 * j,      96 + 48 * i,
                    Scroll + 48 * j + 48, 96 + 48 * i + 48,
                    0x808080, TRUE
                );
                break;
            }
        }
    }
}

map::map() {
    std::vector<std::vector<int>> Tmp = {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    };
    this->Map = Tmp;
}
