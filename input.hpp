#pragma once
#include "DxLib.h"
#include <vector>
#include "pos.hpp"

/**
 * @brief Input manager for keyboard and mouse input
 */
class input {

private:

    // Keyboard state
    char Key[256];

    // Mouse state
    pos MousePos{};
    int MouseInput;
    int BeforeMouseInput;
    // TODO: Add mouse scroll support
    // int MouseScroll;

    // TODO: Add touch input support

    // TODO: Add gamepad support

public:

    /**
     * @brief Update input state (call once per frame)
     */
    void Update();

    /**
     * @brief Check if key is currently pressed
     * @param KeyCode DxLib key code (e.g., KEY_INPUT_SPACE)
     * @return true if key is pressed
     */
    bool GetKey(unsigned char KeyCode);
    
    /**
     * @brief Get current mouse position
     * @return Mouse position
     */
    pos GetMousePos();
    
    /**
     * @brief Check if mouse button is currently pressed
     * @param MouseCode Mouse button code (e.g., MOUSE_INPUT_LEFT)
     * @return true if button is pressed
     */
    bool GetMouseInput(short MouseCode);
    
    /**
     * @brief Check if mouse button was just pressed this frame
     * @param MouseCode Mouse button code
     * @return true if button was just pressed
     */
    bool GetMouseDown(short MouseCode);
    
    /**
     * @brief Check if mouse button was just released this frame
     * @param MouseCode Mouse button code
     * @return true if button was just released
     */
    bool GetMouseUp(short MouseCode);

    /**
     * @brief Construct input manager
     * @param Init Whether to initialize (default: true)
     */
    input(bool Init = true);

};

