#ifdef _DEBUG
#define DebugMode true
#else
#define DebugMode false
#endif // !_DEBUG

#include "DxLib.h"
//#include "resource.h"

#include "Game.hpp"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    SetOutApplicationLogValidFlag(DebugMode);
    SetGraphMode(1280, 720, 16);
    SetDrawScreen(DX_SCREEN_BACK);
    ChangeWindowMode(TRUE);
    SetMainWindowText("DxLib");
    //SetWindowIconID(IDI_ICON1);
    SetWindowSizeChangeEnableFlag(TRUE);
    SetWindowSizeExtendRate(0.8);
    if (DxLib_Init() == -1) return -1;

    Game(DebugMode);

    DxLib_End();
    return 0;
}
