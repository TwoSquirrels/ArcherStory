#include "input.hpp"

void input::Update() {
    // キーボード //
    DxLib::GetHitKeyStateAll(this->Key);
    // マウス //
    int X = 0, Y = 0;
    DxLib::GetMousePoint(&X, &Y);
    this->MousePos.SetPos((double)X, (double)Y);
    this->BeforeMouseInput = this->MouseInput;
    this->MouseInput = DxLib::GetMouseInput();
}

bool input::GetKey(unsigned char KeyCode) {
    return this->Key[KeyCode] == 1;
}

pos input::GetMousePos() {
    return this->MousePos;
}

bool input::GetMouseInput(short MouseCode) {
    return this->MouseInput & MouseCode;
}

bool input::GetMouseDown(short MouseCode) {
    return ((this->BeforeMouseInput ^ this->MouseInput) & this->MouseInput) & MouseCode;
}

bool input::GetMouseUp(short MouseCode) {
    return ((this->BeforeMouseInput ^ this->MouseInput) & this->BeforeMouseInput) & MouseCode;
}

input::input() {
    this->Update();
}
