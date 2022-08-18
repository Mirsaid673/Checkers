#include "input.h"

int Input::last_keys_state[348];
int Input::last_mouse_butons_state[8];
GLFWwindow *Input::window;
glm::ivec2 Input::cursor_pos;

void Input::init(const Window &_window)
{
    window = _window;
    for (int i = 0; i < sizeof(last_keys_state) / sizeof(last_keys_state[0]); i++)
    {
        last_keys_state[i] = glfwGetKey(window, i);
    }

    for (int i = 0; i < sizeof(last_mouse_butons_state) / sizeof(last_mouse_butons_state[0]); i++)
    {
        last_mouse_butons_state[i] = glfwGetMouseButton(window, i);
    }

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    cursor_pos = glm::ivec2(x, y);
}

void Input::update()
{
    for (int i = 0; i < sizeof(last_keys_state) / sizeof(last_keys_state[0]); i++)
    {
        last_keys_state[i] = glfwGetKey(window, i);
    }

    for (int i = 0; i < sizeof(last_mouse_butons_state) / sizeof(last_mouse_butons_state[0]); i++)
    {
        last_mouse_butons_state[i] = glfwGetMouseButton(window, i);
    }

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    cursor_pos = glm::ivec2(x, y);
}