#ifdef _DEBUG
#define DebugMode true
#else
#define DebugMode false
#endif // !_DEBUG

#include "DxLib.h"
//#include "resource.h"

#include "Game.hpp"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    Game(DebugMode, "config.json");

    return 0;
}
