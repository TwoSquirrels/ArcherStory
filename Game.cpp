#include "Game.hpp"

Game::Game(bool Debug, std::string ConfigFilePath) {

    this->Debug = Debug;
    this->ConfigFilePath = ConfigFilePath;
    this->Frame = 0;

    bool Error = false;

    if (this->Load() == -1) return;
    while (this->Update()) {
        this->Input.Update();
        if (DxLib::ProcessMessage()) {
            Error = true;
            break;
        }
        this->Frame++;
    }
    this->Unload(Error);

}

int Game::Load() {

    // json読み込み
    std::string JsonString = this->FileToString(this->ConfigFilePath);
    if (JsonString == "") JsonString = "{}";
    this->Config = json::parse(JsonString);

    // ない項目は補足 文字列はあれば文字コード変換
    if (this->Config["WindowName"].empty())             this->Config["WindowName"] = "アーチャー物語";
    else this->Config["WindowName"] = utf8_to_sjis(this->Config["WindowName"].get<std::string>());
    if (this->Config["WindowExtendRate"].empty())       this->Config["WindowExtendRate"] = 1.0;
    if (this->Config["Player"]["JoystickSize"].empty()) this->Config["Player"]["JoystickSize"] = 64;
    if (this->Config["Player"]["Speed"].empty())        this->Config["Player"]["Speed"] = 7.0;

    // 初期設定
    SetOutApplicationLogValidFlag(this->Debug);
    SetGraphMode(1280, 720, 16);
    SetDrawScreen(DX_SCREEN_BACK);
    ChangeWindowMode(TRUE);
    SetMainWindowText(this->Config["WindowName"].get<std::string>().c_str());
    //SetWindowIconID(IDI_ICON1);
    SetWindowSizeChangeEnableFlag(TRUE);
    SetWindowSizeExtendRate(this->Config["WindowExtendRate"].get<double>());
    if (DxLib_Init() == -1) return -1;

    this->Input = input();
    this->Map = map();
    this->Player = player(&(this->Input), &(this->Map), this->Config["Player"]);

    this->FlowerPlant = flower_plant(&(this->Ball), pos(100, 100), 100);

    return 0;

}

bool Game::Update() {

    // 処理 //
    this->Player.Move();
    this->FlowerPlant.Update(this->Map, this->Player.Sprite.Pos);
    for (int i = 0; i < this->Ball.size(); i++) this->Ball[i].Update(this->Map);
    // ボールがマップ外に出てたら削除(1秒ごと)
    if (this->Frame % 60 == 0) {
        int Size = this->Ball.size();
        for (int i = 0; i < Size; i++) {
            if (!this->Ball[i].Use) {
                // 終わりだったら消すだけ/途中なら最後を代入して最後を消す
                if (i == Size - 1) {
                    Ball.pop_back();
                    break;
                }
                Ball[i] = Ball.back();
                Ball.pop_back();
                Size--;
                i--;
            }
        }
    }

    // 描画 //
    DxLib::ClearDrawScreen();
    DxLib::DrawBox(0, 0, 1280, 720, 0x00FF00, TRUE);
    this->Map.Draw(this->Player.Sprite.Pos.GetX() - this->Player.StartPos.GetX());
    this->FlowerPlant.Draw(this->Player.Sprite.Pos.GetX() - this->Player.StartPos.GetX());
    this->Player.Draw();
    for (ball b : this->Ball) b.Draw(this->Player.Sprite.Pos.GetX() - this->Player.StartPos.GetX());
    this->Player.JoystickDraw();
    DxLib::ScreenFlip();

    if (this->Input.GetKey(KEY_INPUT_ESCAPE)) return false;
    return true;

}

void Game::Unload(bool Error) {

    DxLib::DxLib_End();

}

std::string Game::FileToString(std::string FilePath) {

    std::ifstream File(FilePath);
    if (!File) return "";
    return std::string(
        (std::istreambuf_iterator<char>(File)),
        std::istreambuf_iterator<char>()
    );

}
