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
    if (this->Config["Player"]["JoystickSize"].empty())         this->Config["Player"]["JoystickSize"] = 64;
    if (this->Config["Player"]["Speed"].empty())                this->Config["Player"]["Speed"] = 7.0;
    if (this->Config["Player"]["DefaultMaxHP"].empty())         this->Config["Player"]["DefaultMaxHP"] = 500;
    if (this->Config["Player"]["GodTimeMax"].empty())           this->Config["Player"]["GodTimeMax"] = 60;
    if (this->Config["Player"]["AttackCooldownMax"].empty())    this->Config["Player"]["AttackCooldownMax"] = 36;
    if (this->Config["Player"]["DefaultAttack"].empty())        this->Config["Player"]["DefaultAttack"] = 40;
    if (this->Config["Player"]["Arrow"]["Speed"].empty())       this->Config["Player"]["Arrow"]["Speed"] = 8.0;
    if (this->Config["Monsters"]["FlowerPlant"]["AttackSpeed"].empty()) this->Config["Monsters"]["FlowerPlant"]["AttackSpeed"] = 120;
    if (this->Config["Monsters"]["Slime"]["MoveCooldown"].empty())      this->Config["Monsters"]["Slime"]["MoveCooldown"] = 60;
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

    return 0;

}

bool Game::Update() {
    if (this->Input.GetKey(KEY_INPUT_ESCAPE)) return false;
    bool Continue = false;
    switch (this->Seen) {
    case this->INTRO:
        Continue = this->Intro();
        break;
    case this->STAGE:
        Continue = this->Stage();
        break;
    case this->PAUSE:
        Continue = this->Pause();
        break;
    case this->SKILL_SELECT:
        Continue = this->SkillSelect();
        break;
    case this->DIE:
        Continue = this->Die();
        break;
    }
    return Continue;
}

void Game::Unload(bool Error) {

    //for (monster *m: this->Monster) free(m);
    DxLib::DxLib_End();

}

bool Game::Intro() {

    if (BeforeIntroFrame + 1 != Frame) {

        // 画像読み込み
        std::map<std::string, std::map<std::string, int>> Graph;
        Graph["Map"]["Ground"] = DxLib::LoadGraph("data/stable/img/map/ground.png");
        Graph["Map"]["Wall"] = DxLib::LoadGraph("data/stable/img/map/wall.png");
        Graph["Map"]["Stone"] = DxLib::LoadGraph("data/stable/img/map/stone.png");
        int PondGraph[3 * 3];
        DxLib::LoadDivGraph("data/stable/img/map/pond.png", 3 * 3, 3, 3, 48 / 2, 48 / 2, PondGraph);
        Graph["Map"]["Pond00"] = PondGraph[3 * 0 + 0];
        Graph["Map"]["Pond01"] = PondGraph[3 * 0 + 1];
        Graph["Map"]["Pond02"] = PondGraph[3 * 0 + 2];
        Graph["Map"]["Pond10"] = PondGraph[3 * 1 + 0];
        Graph["Map"]["Pond11"] = PondGraph[3 * 1 + 1];
        Graph["Map"]["Pond12"] = PondGraph[3 * 1 + 2];
        Graph["Map"]["Pond20"] = PondGraph[3 * 2 + 0];
        Graph["Map"]["Pond21"] = PondGraph[3 * 2 + 1];
        Graph["Map"]["Pond22"] = PondGraph[3 * 2 + 2];
        Graph["Map"]["Shutter"] = DxLib::LoadGraph("data/stable/img/map/shutter.png");
        Graph["Map"]["ShutterGate"] = DxLib::LoadGraph("data/stable/img/map/shutter_gate.png");

        // 初期化

        this->Input = input(false);
        this->Map = map(Graph["Map"]);
        this->Player = player(&this->Input, &this->Map, &this->Arrow, &this->Death, &this->Monster, this->Config["Player"]);

        for (int i = 0; i < 4; i++) {
            this->FlowerPlant.push_back(flower_plant(&this->Ball, pos(48.0 + DxLib::GetRand(1072), 48.0 + DxLib::GetRand(496)), 100, 100, &this->Player, this->Config));
        }
        for (int i = 0; i < 4; i++) {
            this->Slime.push_back(slime(pos(48.0 + DxLib::GetRand(1072), 48.0 + DxLib::GetRand(496)), 100, 2, &this->Player, this->Config));
        }

    }

    // 処理 //
    if (this->Input.GetKey(KEY_INPUT_SPACE)) this->Seen = this->STAGE;

    // 描画
    DxLib::ClearDrawScreen();
    DxLib::DrawBox(64, 64, 256, 256, 0x00ffff, TRUE);
    DxLib::ScreenFlip();

    this->BeforeIntroFrame = this->Frame;

    return true;

}

bool Game::Stage() {

    // 処理 //
    // Monster更新
    //for (monster *m : this->Monster) free(m);
    this->Monster.resize(0);
    for (int i = 0; i < this->FlowerPlant.size(); i++) this->Monster.push_back(&this->FlowerPlant[i].Monster);
    for (int i = 0; i < this->Slime.size(); i++) this->Monster.push_back(&this->Slime[i].Monster);
    // Update
    this->Player.Update();
    for (int j = 0; j < 4; j++) {   // スピードを上げるため二重
        for (int i = 0; i < this->Arrow.size(); i++) this->Arrow[i].Update(this->Map);
    }
    for (int i = 0; i < this->FlowerPlant.size(); i++) this->FlowerPlant[i].Update(this->Map);
    std::vector<slime> SlimeToAdd;
    for (int i = 0; i < this->Slime.size(); i++) {
        SlimeToAdd = this->Slime[i].Update(this->Map);
        for (int j = 0; j < SlimeToAdd.size(); j++) {
            this->Slime.push_back(SlimeToAdd[j]);
        }
    }
    for (int i = 0; i < this->Ball.size(); i++) this->Ball[i].Update(this->Map);
    // 使われてないものを削除(1秒ごと)
    if (this->Frame % 60 == 0) {
        int Size;
        // ボール
        Size = this->Ball.size();
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
        // 矢
        Size = this->Arrow.size();
        for (int i = 0; i < Size; i++) {
            if (!this->Arrow[i].Use) {
                // 終わりだったら消すだけ/途中なら最後を代入して最後を消す
                if (i == Size - 1) {
                    Arrow.pop_back();
                    break;
                }
                Arrow[i] = Arrow.back();
                Arrow.pop_back();
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
    // 全員倒した！
    bool IsClear = true;
    for (monster *m : this->Monster) if (m->Use) IsClear = false;
    if (IsClear) this->Map.Clear();

    // 描画 //
    DxLib::ClearDrawScreen();
    int Scroll = this->Player.Sprite.Pos.GetX() - this->Player.StartPos.GetX();
    // 背景
    this->Map.Draw(Scroll);
    // 矢
    for (arrow a : this->Arrow) a.Draw(Scroll);
    // 敵
    for (flower_plant f : this->FlowerPlant) f.Draw(Scroll);
    for (slime s : this->Slime) s.Draw(Scroll);
    // プレイヤー
    this->Player.Draw();
    // 敵の弾
    for (ball b : this->Ball) b.Draw(Scroll);
    // GUI
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
