#include <iostream>
#include <cmath>
#include <algorithm>
// 管理OpenGL的函数指针
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "vendors/stb_image/stb_image.h"

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

void processUpAndDown(GLFWwindow* window, float* mixNumber)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        // glfwSetWindowShouldClose(window, true);
        *mixNumber = std::min(1.0f, *mixNumber + 0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        *mixNumber = std::max(0.0f, *mixNumber - 0.1f);
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
    Shader shader1("res/shaders/vs1.vs", "res/shaders/fs1.fs");

    // ----------------------------- buffers ---------------------------------
    // 顶点坐标 
    float vertices[] = {
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    // 调整indices顺序
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定第一个VAO和VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // if (data)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    stbi_set_flip_vertically_on_load(true);
    unsigned int textureId;
    glGenTextures(1, &textureId);
    // glActiveTexture(GL_TEXTURE0);
    // 绑定纹理到激活的纹理单元
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("res/textures/container.jpeg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // 制作mipmap
    // glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    unsigned int textureId2;
    glGenTextures(1, &textureId2);
    glBindTexture(GL_TEXTURE_2D, textureId2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("res/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // unsigned int EBO;
    // glGenBuffers(1, &EBO);
    // // 这里绑定的EBO会存在bind的VAO里，之后切换不同VAO，则它的EBO也会切换
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // EBO不能解绑，因为它与VAO是绑定的
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 线框模式绘制
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader1.bind();
    // 这个VAO绑定的EBO
    glBindVertexArray(VAO);
    shader1.setUniform1i("texture1", 0);
    shader1.setUniform1i("texture2", 1);
    float mixNumber = 0.5f;
    // 渲染循环
    while(!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);
        processUpAndDown(window, &mixNumber);
        std::cout << mixNumber << std::endl;
        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 指定要清空的缓冲的哪一个，通过缓冲位（Buffer Bit）来指定
        glClear(GL_COLOR_BUFFER_BIT);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        shader1.setUniform1f("mixNumber", mixNumber);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureId2);

        // 从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取其索引
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 解绑
        // glBindVertexArray(0);

        // 交换窗口每一个像素颜色值的缓冲，被绘制出来
        // 使用“双缓冲”：前缓冲存展示的装填，后缓冲存下次渲染（即指令作用在后缓冲）
        glfwSwapBuffers(window);
        // 检查键盘，鼠标的事件，更新窗口状态，触发注册的回调函数
        glfwPollEvents();
    }

    // 析构函数
    // glDeleteProgram(program);
    // glDeleteProgram(program2);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);

    glfwTerminate();
    return 0;
}