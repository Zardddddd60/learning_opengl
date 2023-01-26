#include <iostream>
#include <string>
#include <vector>
#include <map>

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

float cubeVertices[] = {
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
float planeVertices[] = {
    // positions
    // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
};

float grassVertices[] = {
    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

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
    std::vector<glm::vec3> vegetation;
int main()
{
    vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
    vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
    vegetation.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
    vegetation.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
    vegetation.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));
    Glfw glfw(800, 600);

    WindowSize size = glfw.getActucalSize();
    updataLastXY(size.width, size.height);
    // 捕捉鼠标
    glfwSetInputMode(glfw.m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // 鼠标移动就调用
    // glfwSetCursorPosCallback(glfw.m_Window, mouse_callback);
    // 滚轮回调
    // glfwSetScrollCallback(glfw.m_Window, scroll_callback);

    SelfImgui imgui(glfw.m_Window);

    Shader shader("res/shaders/stencil-test/dt.vs", "res/shaders/stencil-test/dt.fs");
    Texture cubeTexture("res/textures/marble.jpg");
    Texture floorTexture("res/textures/metal.png");
    Texture grassTexture("res/textures/blending_transparent_window.png", false);

    VertexArray cubeVao;
    VertexBuffer cubeVb(cubeVertices, sizeof(cubeVertices));
    VertexBufferLayout cubeVbl;
    cubeVbl.push<float>(3);
    cubeVbl.push<float>(2);
    cubeVao.addBuffer(cubeVb, cubeVbl);

    VertexArray floorVao;
    VertexBuffer floorVb(planeVertices, sizeof(planeVertices));
    VertexBufferLayout floorVbl;
    floorVbl.push<float>(3);
    floorVbl.push<float>(2);
    floorVao.addBuffer(floorVb, floorVbl);

    VertexArray grassVao;
    VertexBuffer grassVb(grassVertices, sizeof(grassVertices));
    VertexBufferLayout grassVbl;
    grassVbl.push<float>(3);
    grassVbl.push<float>(2);
    grassVao.addBuffer(grassVb, grassVbl);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
        
        glm::mat4 model(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(camera.getFov(), (float)size.width / size.height, 1.0f, 100.0f);

        shader.bind();
        shader.setUniformMatrix4fv("view", view);
        shader.setUniformMatrix4fv("projection", projection);
        cubeTexture.bind();
        shader.setUniform1i("texture1", 0);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setUniformMatrix4fv("model", model);
        cubeVao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setUniformMatrix4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        floorTexture.bind();
        shader.setUniformMatrix4fv("model", glm::mat4(1.0f));
        shader.setUniformMatrix4fv("view", view);
        shader.setUniformMatrix4fv("projection", projection);
        floorTexture.bind();
        floorVao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader.bind();
        grassTexture.bind();
        grassVao.bind();
        std::map<float, glm::vec3> sorted;

        for (unsigned int i = 0; i < vegetation.size(); i ++)
        {
            float distance = glm::length(camera.getPosision() - vegetation[i]);
            sorted[distance] = vegetation[i];
        }
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++ it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);               
            shader.setUniformMatrix4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // imgui.beforeRender();
        // {
        //     ImGui::Begin("Hello, world!");
        //     ImGui::ColorEdit4("lightColor", lightColor);
        //     ImGui::SliderFloat3("lightPos", lightPos, 0.0f, 5.0f);
        //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //     ImGui::End();
        // }
        // imgui.afterRender();

        // 交换窗口每一个像素颜色值的缓冲，被绘制出来
        // 使用“双缓冲”：前缓冲存展示的装填，后缓冲存下次渲染（即指令作用在后缓冲）
        glfwSwapBuffers(glfw.m_Window);
        // 检查键盘，鼠标的事件，更新窗口状态，触发注册的回调函数
        glfwPollEvents();
    }
    glfwTerminate();
}
