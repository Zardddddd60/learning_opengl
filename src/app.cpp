#include <iostream>
#include <cmath>
// 管理OpenGL的函数指针
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

void framebuffer_size_cb(GLFWwindow* window, int width, int height)
{
    std::cout << width << ", " << height << std::endl;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 将窗口上下文，设置成现成的主上下文
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 设置OpenGL渲染窗口的尺寸
    // 前两个参数设置左下角位置，第3/4设置宽高
    // OpenGL会自动把坐标(-0.5, 0.5)，根据设置的窗口大小，映射到(200, 450)
    // 好像视口viewport要这么设置才可以
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    // std::cout << display_w << ", " << display_h << std::endl;
    glViewport(0, 0, display_w, display_h);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);

    // ----------------------------- shader & program ---------------------------------
    // 创建shader
    Shader shader1("src/res/shaders/vs1.vs", "src/res/shaders/fs1.fs");
    Shader shader2("src/res/shaders/vs2.vs", "src/res/shaders/fs2.fs");

    // ----------------------------- buffers ---------------------------------
    // 顶点坐标 
    float vertices1[] = {
        // 调整顶点组成
        -1.0f, -1.0f, 0.0f,
        -0.5f, 1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
    };

    float vertices2[] = {
        0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    // 调整indices顺序
    // unsigned int indices[] = {
    //     0, 1, 2,
    //     2, 3, 4
    // };

    unsigned int VBO[2], VAO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    // 绑定第一个VAO和VBO
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unsigned int EBO;
    // glGenBuffers(1, &EBO);
    // // 这里绑定的EBO会存在bind的VAO里，之后切换不同VAO，则它的EBO也会切换
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // EBO不能解绑，因为它与VAO是绑定的
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 线框模式绘制
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染循环
    while(!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 指定要清空的缓冲的哪一个，通过缓冲位（Buffer Bit）来指定
        glClear(GL_COLOR_BUFFER_BIT);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        shader1.bind();
        shader1.setUniform4f("color", 0.0f, greenValue, 0.0f, 1.0f);

        // 这个VAO绑定的EBO
        glBindVertexArray(VAO[0]);
        // 从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取其索引
        glDrawArrays(GL_TRIANGLES, 0, 3);

        shader2.bind();
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 解绑
        glBindVertexArray(0);

        // 交换窗口每一个像素颜色值的缓冲，被绘制出来
        // 使用“双缓冲”：前缓冲存展示的装填，后缓冲存下次渲染（即指令作用在后缓冲）
        glfwSwapBuffers(window);
        // 检查键盘，鼠标的事件，更新窗口状态，触发注册的回调函数
        glfwPollEvents();
    }

    // 析构函数
    // glDeleteProgram(program);
    // glDeleteProgram(program2);
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);

    glfwTerminate();
    return 0;
}