#include "input.hpp"

void input::Update() {
    // Update keyboard state
    DxLib::GetHitKeyStateAll(this->Key);
    
    // Update mouse state
    int X = 0, Y = 0;
    DxLib::GetMousePoint(&X, &Y);
    this->MousePos.SetPos(static_cast<double>(X), static_cast<double>(Y));
    
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
    return (this->MouseInput & MouseCode) != 0;
}

bool input::GetMouseDown(short MouseCode) {
    // Check if button was pressed this frame (XOR detects change, AND with current selects press)
    return (((this->BeforeMouseInput ^ this->MouseInput) & this->MouseInput) & MouseCode) != 0;
}

bool input::GetMouseUp(short MouseCode) {
    // Check if button was released this frame (XOR detects change, AND with previous selects release)
    return (((this->BeforeMouseInput ^ this->MouseInput) & this->BeforeMouseInput) & MouseCode) != 0;
}

input::input(bool Init) {
    if (!Init) {
        this->Update();
    }
}
