#include <iostream>
#include "glfw.h"

Glfw::Glfw(unsigned int initWidth, unsigned int initHeight)
{
    // 初始化GLFW
    glfwInit();
    // 配置GLFW，告诉GLFW使用的OpenGL的版本是3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    m_Window = glfwCreateWindow(initWidth, initHeight, "LearnOpenGL", NULL, NULL);
    if (m_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    // 将窗口上下文，设置成现成的主上下文
    glfwMakeContextCurrent(m_Window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        m_Window = NULL;
    }
    // 设置OpenGL渲染窗口的尺寸
    // 前两个参数设置左下角位置，第3/4设置宽高
    // OpenGL会自动把坐标(-0.5, 0.5)，根据设置的窗口大小，映射到(200, 450)
    // 好像视口viewport要这么设置才可以
    glfwGetFramebufferSize(m_Window, &m_CalculatedWidth, &m_CalculatedHeight);
    // std::cout << display_w << ", " << display_h << std::endl;
    glViewport(0, 0, m_CalculatedWidth, m_CalculatedHeight);
}

Glfw::~Glfw()
{
    glfwTerminate();
}

WindowSize Glfw::getActucalSize() const
{
    return {
        m_CalculatedWidth,
        m_CalculatedHeight
    };
}