#include "Game.hpp"

Game::Game(bool Debug = false, std::string ConfigFilePath = "config.ini") {

    this->Debug = Debug;
    this->ConfigFilePath = ConfigFilePath;
    this->Frame = 0;
    bool Error = false;

    input Input();

    this->Load();
    while (this->Update()) {
        Input.Update();
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

    DxLib::ClearDrawScreen();
    DxLib::DrawBox(0, 0, 100, 100, 0x00ff00, FALSE);

    if (DxLib::CheckHitKey(KEY_INPUT_ESCAPE)) return false;
    return true;

}

void Game::Unload(bool Error) {



}
