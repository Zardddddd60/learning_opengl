#pragma once

#include <GLFW/glfw3.h>

class SelfImgui
{
public:
    SelfImgui(GLFWwindow* window);
    ~SelfImgui();

    void beforeRender() const;
    void afterRender() const;
};
