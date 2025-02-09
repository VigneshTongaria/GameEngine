#ifndef MOUSE_H
#define MOUSE_H
#include<glad/glad.h>
#include<GLFW/glfw3.h>
class Mouse
{
    
private :
static double _x;
static double _y;
static double lastx;
static double lasty;
static double dx;
static double dy;
static double scrollDx;
static double scrollDy;
static bool firstMouse;
static bool buttons[];
static bool buttonsChanged[];

public:

    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void CursorButtonCallback(GLFWwindow *window, double x, double y);
    static void MouseScrollCallback(GLFWwindow *window, double dx, double dy);
    static double getMouseX();
    static double getMouseY();
    static double getMouseDX();
    static double getMouseDY();
    static double getMouseWheelX();
    static double getMouseWheelY();
    static bool MouseButton(int button);
    static bool MouseButtonChanged(int button);
    static bool MouseButtonWentUp(int button);
    static bool MosueButtonWentDown(int button);
};



#endif