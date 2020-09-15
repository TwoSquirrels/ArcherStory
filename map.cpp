#include "map.hpp"

void map::LoadFromFile(std::string FilePath) {

}

std::vector<bool> Collision(sprite *Sprite, std::map<std::string, bool> Block) {



}

void map::Draw(int Scroll) {
    // 720 - (48*11=528) = 192
    for (int i = 0; i < this->Map.size(); i++) {
        for (int j = 0; j < this->Map[i].size(); j++) {
            switch (this->Map[i][j]) {
            case 0:
                DxLib::DrawBox(
                    -Scroll + 48 + 48 * j, 720 - 48 * (11 - i) - 48,
                    -Scroll + 48 + 48 * j + 48, 720 - 48 * (11 - i),
                    0xFFFFFF, FALSE
                );
                break;
            case 1:
                DxLib::DrawBox(
                    Scroll + 48 + 48 * j, 720 - 48 * (11 - i) - 48,
                    Scroll + 48 + 48 * j + 48, 720 - 48 * (11 - i),
                    0x808080, TRUE
                );
                break;
            }
        }
    }
}

map::map() {
    std::vector<std::vector<int>> Tmp = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    };
    this->Map = Tmp;
}
