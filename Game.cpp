#include "Game.hpp"

Game::Game(bool Debug, std::string ConfigFilePaths) {

    this->Debug = Debug;
    this->ConfigFilePath = ConfigFilePath;
    this->Frame = 0;

    bool Error = false;

    this->Load();
    while (this->Update()) {
        this->Input.Update();
        if (DxLib::ProcessMessage()) {
            Error = true;
            break;
        }
        this->Frame++;
    }
    this->Unload(Error);

    DxLib::DxLib_End();

}

void Game::Load() {



}

bool Game::Update() {

    // ˆ— //
    this->Player.Move();

    // •`‰æ //
    DxLib::ClearDrawScreen();
    DxLib::DrawBox(0, 0, 1280, 720, 0x00FF00, TRUE);
    this->Player.Draw();
    DxLib::ScreenFlip();

    if (this->Input.GetKey(KEY_INPUT_ESCAPE)) return false;
    return true;

}

void Game::Unload(bool Error) {



}
