#ifndef KEYBOARD_H
#define KEYBOARD_H
#include<glad/glad.h>
#include<GLFW/glfw3.h>
class KeyBoard
{
private:
    static bool keys[];
    static bool keysChanged[];
    /* data */
public:

    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static bool Key(int key);
    static bool keyChanged(int key);
    static bool KeyWentUp(int key);
    static bool KeyWentDown(int key);
};


#endif