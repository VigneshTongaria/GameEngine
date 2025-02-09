#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "KeyBoard.h"

bool KeyBoard::keys[GLFW_KEY_LAST] = {0};
bool KeyBoard::keysChanged[GLFW_KEY_LAST] = {0};

void KeyBoard :: KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
      if(action != GLFW_RELEASE)
      {
        if(!keys[key])
         keys[key] = true; 
      }
      else keys[key] = false;

      keysChanged[key] = action != GLFW_REPEAT;
}
bool KeyBoard::Key(int key)
{
    return keys[key];
}
bool KeyBoard :: keyChanged(int key)
{
    bool ret = keysChanged[key];
    keysChanged[key] = false;
    return ret;
}
 bool KeyBoard :: KeyWentUp(int key)
 {
    return !keys[key]&&keyChanged(key);
 }
 bool KeyBoard :: KeyWentDown(int key)
 {
    return keys[key]&& keyChanged(key);
 }
