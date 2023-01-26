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

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
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

    Shader shader("res/shaders/model/model.vs", "res/shaders/model/model.fs");
    Model robotModel(std::string("res/models/nanosuit/nanosuit.obj"));
    glEnable(GL_DEPTH_TEST);

    float lightColor[4] = {
        1.0f, 1.0f, 1.0f, 1.0f
    };
    float lightPos[3] = {
        // 2.2f, 2.0f, 4.0f
        1.2f, 1.0f, 2.0f
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
        shader.bind();
        auto size = glfw.getActucalSize();
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)size.width / (float)size.height, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        shader.setUniformMatrix4fv("projection", projection);
        shader.setUniformMatrix4fv("view", view);
        shader.setUniform1f("material.shininess", 32.0f);

        shader.setUniformVector3fv("viewPos", camera.getPosision());

        // 光
        shader.setUniformVector3fv("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader.setUniformVector3fv("dirLight.ambient", glm::vec3(0.05f));
        shader.setUniformVector3fv("dirLight.diffuse", glm::vec3(0.4f));
        shader.setUniformVector3fv("dirLight.specular", glm::vec3(0.5f));

        for (unsigned int i = 0; i < 2; i ++)
        {
            std::string name = "pointLights[" + std::to_string(i) + "].";
            shader.setUniformVector3fv(name + "position", pointLightPositions[i]);
            shader.setUniformVector3fv(name + "ambient", glm::vec3(0.05f));
            shader.setUniformVector3fv(name + "diffuse", glm::vec3(0.8f));
            shader.setUniformVector3fv(name + "specular", glm::vec3(1.0f));
            shader.setUniform1f(name + "constant", 1.0f);
            shader.setUniform1f(name + "linear", 0.09f);
            shader.setUniform1f(name + "quadratic", 0.032f);
        }

        shader.setUniformVector3fv("spotLight.position", camera.getPosision());
        shader.setUniformVector3fv("spotLight.direction", camera.getFront());
        shader.setUniformVector3fv("spotLight.ambient", glm::vec3(0.0f));
        shader.setUniformVector3fv("spotLight.diffuse", glm::vec3(1.0f));
        shader.setUniformVector3fv("spotLight.specular", glm::vec3(1.0f));
        shader.setUniform1f("spotLight.constant", 1.0f);
        shader.setUniform1f("spotLight.linear", 0.09f);
        shader.setUniform1f("spotLight.quadratic", 0.032f);
        shader.setUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f))); 

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shader.setUniformMatrix4fv("model", model);
        robotModel.draw(shader);

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
