#pragma once

#include "window.h"

#include <glm/glm.hpp>

class Input
{
public:
    enum State
    {
        RELEASE = GLFW_RELEASE,
        PRESS = GLFW_PRESS,
    };

private:
    static int last_keys_state[348];
    static int last_mouse_butons_state[8];
    static GLFWwindow *window;
    static glm::ivec2 cursor_pos;

public:
    static void init(const Window &_window);
    static void update();

    static int getKey(int key) { return glfwGetKey(window, key); }
    static bool getKeyDown(int key) { return getKey(key) == PRESS; }
    static bool getKeyUp(int key) { return getKey(key) == RELEASE; }
    static bool getKeyPress(int key) { return getKey(key) == PRESS && last_keys_state[key] == RELEASE; }
    static bool getKeyRelease(int key) { return getKey(key) == RELEASE && last_keys_state[key] == PRESS; }

    static int getMouseButton(int mouse_button) { return glfwGetMouseButton(window, mouse_button); }
    static bool getMouseButtonDown(int mouse_button) { return getMouseButton(mouse_button) == PRESS; }
    static bool getMouseButtonUp(int mouse_button) { return getMouseButton(mouse_button) == RELEASE; }
    static bool getMouseButtonPress(int mouse_button) { return getMouseButton(mouse_button) == PRESS && last_mouse_butons_state[mouse_button] == RELEASE; }
    static bool getMouseButtonRelease(int mouse_button) { return getMouseButton(mouse_button) == RELEASE && last_mouse_butons_state[mouse_button] == PRESS; }

    static glm::ivec2 getCursorPos() {return cursor_pos;}
};