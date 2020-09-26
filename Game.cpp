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
    if (this->Config["WindowName"].empty())         this->Config["WindowName"] = "アーチャー物語";
    else this->Config["WindowName"] = utf8_to_sjis(this->Config["WindowName"].get<std::string>());
    if (this->Config["WindowExtendRate"].empty())   this->Config["WindowExtendRate"] = 1.0;
    if (this->Config["Player"]["JoystickSize"].empty()) this->Config["Player"]["JoystickSize"] = 64;
    if (this->Config["Player"]["Speed"].empty())        this->Config["Player"]["Speed"] = 7.0;
    if (this->Config["Player"]["DefaultMaxHP"].empty()) this->Config["Player"]["DefaultMaxHP"] = 500;
    if (this->Config["Player"]["GodTimeMax"].empty())   this->Config["Player"]["GodTimeMax"] = 60;
    if (this->Config["Monster"]["FlowerPlant"]["AttackSpeed"].empty())  this->Config["Monster"]["FlowerPlant"]["AttackSpeed"] = 120;
    if (this->Config["Balls"]["Jump"]["High"].empty())      this->Config["Balls"]["Jump"]["High"] = 64.0;
    if (this->Config["Balls"]["Jump"]["Speed"].empty())     this->Config["Balls"]["Jump"]["Speed"] = 8.0;

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
    this->Player = player(&(this->Input), &(this->Map), &(this->Death), this->Config["Player"]);

    for (int i = 0; i < 16; i++) {
        this->FlowerPlant.push_back(flower_plant(&(this->Ball), pos(48 + DxLib::GetRand(1072), 48 + DxLib::GetRand(496)), 100, 100, &(this->Player), this->Config));
    }

    return 0;

}

bool Game::Update() {
    if (this->Input.GetKey(KEY_INPUT_ESCAPE)) return false;
    switch (this->Seen) {
    case this->INTRO:
        return this->Intro();
    case this->STAGE:
        return this->Stage();
    case this->PAUSE:
        return this->Pause();
    case this->SKILL_SELECT:
        return this->SkillSelect();
    case this->DIE:
        return this->Die();
    }
}

void Game::Unload(bool Error) {

    DxLib::DxLib_End();

}

bool Game::Intro() {

    // 処理 //
    if (this->Input.GetKey(KEY_INPUT_SPACE)) this->Seen = this->STAGE;

    // 描画
    DxLib::ClearDrawScreen();
    DxLib::DrawBox(64, 64, 256, 256, 0x00ffff, TRUE);
    DxLib::ScreenFlip();

    return true;

}

bool Game::Stage() {

    // 処理 //
    this->Player.Update();
    for (int i = 0; i < FlowerPlant.size(); i++) this->FlowerPlant[i].Update(this->Map, this->Player.Sprite);
    for (int i = 0; i < this->Ball.size(); i++) this->Ball[i].Update(this->Map);
    // ボールが使われなくなってたら削除(1秒ごと)
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
    // 死んじゃった！
    if (this->Death) {
        this->Death = false;
        this->Seen = this->DIE;
    }

    // 描画 //
    DxLib::ClearDrawScreen();
    DxLib::DrawBox(0, 0, 1280, 720, 0x00FF00, TRUE);
    this->Map.Draw(this->Player.Sprite.Pos.GetX() - this->Player.StartPos.GetX());
    for (int i = 0; i < FlowerPlant.size(); i++) this->FlowerPlant[i].Draw(this->Player.Sprite.Pos.GetX() - this->Player.StartPos.GetX());
    this->Player.Draw();
    for (ball b : this->Ball) b.Draw(this->Player.Sprite.Pos.GetX() - this->Player.StartPos.GetX());
    this->Player.JoystickDraw();
    DxLib::ScreenFlip();

    return true;

}

bool Game::Pause() {

    // 処理 //


    // 描画
    DxLib::ClearDrawScreen();

    DxLib::ScreenFlip();

    return true;

}

bool Game::SkillSelect() {

    // 処理 //


    // 描画
    DxLib::ClearDrawScreen();

    DxLib::ScreenFlip();

    return true;

}

bool Game::Die() {

    // 処理 //


    // 描画
    DxLib::ClearDrawScreen();

    DxLib::ScreenFlip();

    return true;

}

std::string Game::FileToString(std::string FilePath) {

    std::ifstream File(FilePath);
    if (!File) return "";
    return std::string(
        (std::istreambuf_iterator<char>(File)),
        std::istreambuf_iterator<char>()
    );

}
