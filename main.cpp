#include <iostream>

#include "glfw.h"
#include "camera.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "shader.h"
#include "self_imgui.h"

#include "vendors/glm/glm.hpp"
#include "vendors/glm/gtc/matrix_transform.hpp"
#include "vendors/imgui/imgui.h"
#include "vendors/imgui/imgui_impl_glfw.h"
#include "vendors/imgui/imgui_impl_opengl3.h"

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

Camera camera(glm::vec3(2.0f, 3.0f, 10.0f));

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
    // glfwSetInputMode(glfw.m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // 鼠标移动就调用
    // glfwSetCursorPosCallback(glfw.m_Window, mouse_callback);
    // 滚轮回调
    glfwSetScrollCallback(glfw.m_Window, scroll_callback);

    SelfImgui imgui(glfw.m_Window);

    VertexArray boxVa;
    VertexBuffer boxVb(vertices, sizeof(vertices));
    VertexBufferLayout boxVbl;
    boxVbl.push<float>(3);
    boxVbl.push<float>(3);
    boxVa.addBuffer(boxVb, boxVbl);

    VertexArray lightVa;
    VertexBuffer lightVb(vertices, sizeof(vertices));
    VertexBufferLayout lightVbl;
    lightVbl.push<float>(3);
    lightVbl.push<float>(3);
    lightVa.addBuffer(lightVb, lightVbl);

    Shader boxShader("res/shaders/light/box.vs", "res/shaders/light/box.fs");

    Shader lightShader("res/shaders/light/light.vs", "res/shaders/light/light.fs");

    glEnable(GL_DEPTH_TEST);

    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    float lightColor[4] = {
        1.0f, 1.0f, 1.0f, 1.0f
    };
    float lightPos[3] = {
        2.2f, 2.0f, 4.0f
    };
    glm::vec3 lightPosVec(lightPos[0], lightPos[1], lightPos[2]);
    glm::vec3 lightColorVec(lightColor[0], lightColor[1], lightColor[2]);
    while(!glfwWindowShouldClose(glfw.m_Window))
    {
        // 计算deltatime
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // 处理输入
        processInput(glfw.m_Window);
        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 指定要清空的缓冲的哪一个，通过缓冲位（Buffer Bit）来指定
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glm::vec3 lightPos(2.2f, 2.0f, 4.0f);

        lightPosVec = glm::vec3(lightPos[0], lightPos[1], lightPos[2]);
        lightColorVec = glm::vec3(lightColor[0], lightColor[1], lightColor[2]);

        boxShader.bind();
        glm::mat4 model(1.0f);
        boxShader.setUniformMatrix4fv("model", model);
        glm::mat4 view = camera.getViewMatrix();
        boxShader.setUniformMatrix4fv("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)size.width / size.height, 0.1f, 100.0f);
        boxShader.setUniformMatrix4fv("projection", projection);

        boxShader.setUniformVector3fv("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        boxShader.setUniformVector3fv("viewPos", camera.getPosision());
        boxShader.setUniformVector3fv("lightColor",  glm::vec3(lightColorVec));
        boxShader.setUniformVector3fv("lightPos", glm::vec3(lightPosVec));
        boxShader.setUniform1f("ambientStrength", ambientStrength);
        boxShader.setUniform1f("specularStrength", specularStrength);
        boxVa.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        boxVa.unbind();
        boxShader.unbind();

        lightShader.bind();
        lightShader.bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosVec);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setUniformMatrix4fv("model", model);
        lightShader.setUniformMatrix4fv("projection", projection);
        lightShader.setUniformMatrix4fv("view", view);
        lightShader.setUniformVector3fv("lightColor", lightColorVec);
        lightVa.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        lightVa.unbind();
        lightShader.unbind();

        imgui.beforeRender();
        {
            ImGui::Begin("Hello, world!"); 
            ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit4("lightColor", lightColor);
            ImGui::SliderFloat3("lightPos", lightPos, 0.0f, 2.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        imgui.afterRender();

        // 交换窗口每一个像素颜色值的缓冲，被绘制出来
        // 使用“双缓冲”：前缓冲存展示的装填，后缓冲存下次渲染（即指令作用在后缓冲）
        glfwSwapBuffers(glfw.m_Window);
        // 检查键盘，鼠标的事件，更新窗口状态，触发注册的回调函数
        glfwPollEvents();
    }
    glfwTerminate();
}
