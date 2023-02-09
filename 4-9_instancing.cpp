#include <iostream>
#include <string>

#include "glfw.h"
#include "camera.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "shader.h"
#include "self_imgui.h"
#include "texture.h"
#include "model.h"

#include "vendors/glm/glm.hpp"
#include "vendors/glm/gtc/matrix_transform.hpp"
#include "vendors/imgui/imgui.h"
#include "vendors/imgui/imgui_impl_glfw.h"
#include "vendors/imgui/imgui_impl_opengl3.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

const unsigned int initWidth = 800;
const unsigned int initHeight = 600;
// 计算deltatime
float lastTime = 0.0f;
float deltaTime = 0.0f;

// 计算mousemove的offset
float lastX = initWidth / 2.0f;
float lastY = initHeight / 2.0f;
bool isFirstMouse = true;
void updataLastXY(int width, int height)
{
    lastX = width / 2.0f;
    lastY = height / 2.0f;
}
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (isFirstMouse)
    {
        // 防止进入时鼠标跳动
        lastX = xPos;
        lastY = yPos;
        isFirstMouse = false;
    }
    float xOffset = xPos - lastX;
    float yOffset = -(yPos - lastY); // yPos基于屏幕左上角（从上到下递增），lastY从下到商递增

    lastX = xPos;
    lastY = yPos;

    camera.processMouseMove(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.processMouseScroll(static_cast<float>(yOffset));
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.processKeyBoard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.processKeyBoard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.processKeyBoard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.processKeyBoard(RIGHT, deltaTime);
    }
}

int main()
{
    Glfw glfw(800, 600);

    WindowSize size = glfw.getActucalSize();
    updataLastXY(size.width, size.height);
    // 捕捉鼠标
    glfwSetInputMode(glfw.m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // 鼠标移动就调用
    glfwSetCursorPosCallback(glfw.m_Window, mouse_callback);
    // 滚轮回调
    glfwSetScrollCallback(glfw.m_Window, scroll_callback);

    SelfImgui imgui(glfw.m_Window);

    float quadVertices[] = {
        // 位置          // 颜色
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,   
        0.05f,  0.05f,  0.0f, 1.0f, 1.0f                   
    };  
    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for(int x = -10; x < 10; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }
    Shader shader("res/shaders/instancing/ins.vs", "res/shaders/instancing/ins.fs");
    
    VertexArray vao;
    VertexBuffer vb(quadVertices, sizeof(quadVertices));
    VertexBufferLayout vbl;
    vbl.push<float>(2);
    vbl.push<float>(3);
    vao.addBuffer(vb, vbl);

    VertexBuffer vb2(&translations[0], sizeof(glm::vec2) * 100);
    // vao.addInstanceData(vb2, 2);
    VertexBufferLayout vbl2;
    vbl2.push<glm::vec2>(2, true);
    vao.addBuffer(vb2, vbl2);

    // glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(glfw.m_Window))
    {
        // 计算deltatime
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // 处理输入
        processInput(glfw.m_Window);
        // 渲染指令
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // 指定要清空的缓冲的哪一个，通过缓冲位（Buffer Bit）来指定
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // glm::vec3 lightPos(2.2f, 2.0f, 4.0f);

        // imgui.beforeRender();
        // {
        //     ImGui::Begin("Hello, world!");
        //     ImGui::ColorEdit4("lightColor", lightColor);
        //     ImGui::SliderFloat3("lightPos", lightPos, 0.0f, 5.0f);
        //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //     ImGui::End();
        // }
        // imgui.afterRender();
        shader.bind();
        vao.bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

        // 交换窗口每一个像素颜色值的缓冲，被绘制出来
        // 使用“双缓冲”：前缓冲存展示的装填，后缓冲存下次渲染（即指令作用在后缓冲）
        glfwSwapBuffers(glfw.m_Window);
        // 检查键盘，鼠标的事件，更新窗口状态，触发注册的回调函数
        glfwPollEvents();
    }
    glfwTerminate();
}
