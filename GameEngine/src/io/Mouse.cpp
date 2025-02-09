#include "Mouse.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>

double Mouse::_x = 0;
double Mouse::_y = 0;
double Mouse::lastx = 0;
double Mouse::lasty = 0;
double Mouse::dx = 0;
double Mouse::dy = 0;
double Mouse::scrollDx = 0;
double Mouse::scrollDy= 0;
bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = {0};
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = {0};
bool Mouse::firstMouse = true;

void Mouse::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if(action != GLFW_RELEASE)
    {
        if(!buttons[button]) 
         buttons[button] = true;
    }
    else buttons[button] = false;

    buttonsChanged[button] = action != GLFW_REPEAT;
}
void Mouse::CursorButtonCallback(GLFWwindow *window, double x, double y)
{
    _x = x;
    _y = y;

    if(firstMouse)
    {
        lastx = x;
        lasty = y;
        firstMouse = false;
    }

    dx = x - lastx;
    dy = lasty - y; //inverted idk why

    lastx = x;
    lasty = y;

}
void Mouse::MouseScrollCallback(GLFWwindow *window, double dx, double dy)
{
    scrollDx = dx;
    scrollDy = dy;
}
double Mouse:: getMouseX()
{
    return _x;
}
double Mouse:: getMouseY()
{
    return _y;
}
double Mouse::getMouseDX()
{
    double _dx = dx;
    dx = 0;
    return _dx;
}
double Mouse::getMouseDY()
{
    double _dy = dy;
    dy = 0;
    return _dy;
}
double Mouse:: getMouseWheelX()
{
    double sdx = scrollDx;
    scrollDx = 0;
    return sdx;
}
double Mouse:: getMouseWheelY()
{
    double sdy = scrollDy;
    scrollDy = 0;
    return sdy;
}
bool Mouse::MouseButton(int button)
{
    return buttons[button];
}
bool Mouse::MouseButtonChanged(int button)
{
    bool ret = buttonsChanged[button];
    buttonsChanged[button] = false;
    return ret;
}
bool Mouse::MouseButtonWentUp(int button)
{
    return !buttons[button] && MouseButtonChanged(button);
}
bool Mouse::MosueButtonWentDown(int button)
{
    return buttons[button] && MouseButtonChanged(button);
}