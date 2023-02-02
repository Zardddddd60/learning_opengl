#include <iostream>
#include <string>
#include <vector>

#include "glfw.h"
#include "camera.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "shader.h"
#include "self_imgui.h"
#include "texture.h"
#include "model.h"
#include "frame_buffer.h"
#include "texture_frame_buffer.h"
#include "render_buffer.h"
#include "cube_texture.h"
#include "uniform_buffer.h"
#include "uniform_buffer_layout.h"

#include "vendors/glm/glm.hpp"
#include "vendors/glm/gtc/matrix_transform.hpp"
#include "vendors/glm/gtc/type_ptr.hpp"
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


const std::vector<std::string> images = {
    "res/textures/skybox/right.jpg",
    "res/textures/skybox/left.jpg",
    "res/textures/skybox/top.jpg",
    "res/textures/skybox/bottom.jpg",
    "res/textures/skybox/front.jpg",
    "res/textures/skybox/back.jpg"
};

float cubeVertices[] = {
    // positions         
    -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,  
        0.5f,  0.5f, -0.5f,  
        0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 

    -0.5f, -0.5f,  0.5f, 
        0.5f, -0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f, 
    -0.5f, -0.5f,  0.5f, 

    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 

        0.5f,  0.5f,  0.5f,  
        0.5f,  0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  

    -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f,  0.5f,  
        0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f, -0.5f, 

    -0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f, -0.5f,  
        0.5f,  0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f, 
};

int main()
{
    Glfw glfw(800, 600);

    WindowSize size = glfw.getActucalSize();
    updataLastXY(size.width, size.height);
    // 捕捉鼠标
    glfwSetInputMode(glfw.m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 鼠标移动就调用
    glfwSetCursorPosCallback(glfw.m_Window, mouse_callback);
    // 滚轮回调
    glfwSetScrollCallback(glfw.m_Window, scroll_callback);

    SelfImgui imgui(glfw.m_Window);

    Shader shaderRed("res/shaders/uniform-buffer/vs.vs", "res/shaders/uniform-buffer/red.fs");
    Shader shaderGreen("res/shaders/uniform-buffer/vs.vs", "res/shaders/uniform-buffer/green.fs");
    Shader shaderBlue("res/shaders/uniform-buffer/vs.vs", "res/shaders/uniform-buffer/blue.fs");
    Shader shaderYellow("res/shaders/uniform-buffer/vs.vs", "res/shaders/uniform-buffer/yellow.fs");
    shaderRed.bindToUniformBlock("Matrices", 0);
    shaderGreen.bindToUniformBlock("Matrices", 0);
    shaderBlue.bindToUniformBlock("Matrices", 0);
    shaderYellow.bindToUniformBlock("Matrices", 0);

    UniformBufferLayout ubl;
    // 一定要注意这里的顺序…
    glm::mat4 projectMatrix = glm::perspective(glm::radians(45.0f), (float)size.width / size.height, 0.1f, 100.0f);
    // glm::mat4 projectMatrix = glm::mat4(1.0f);
    ubl.push(glm::value_ptr(projectMatrix));

    glm::mat4 viewMatrix = camera.getViewMatrix();
    // glm::mat4 viewMatrix = glm::mat4(1.0f);
    ubl.push(glm::value_ptr(viewMatrix));
    UniformBuffer ub;
    ub.addBuffer(ubl);
    ub.bindToPoint(0);

    // unsigned int uboMatrices;
    // glGenBuffers(1, &uboMatrices);
    // glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    // int qqqq = sizeof(glm::mat4);
    // glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    // glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // // define the range of the buffer that links to a uniform binding point

    // // store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
    // glm::mat4 projection = glm::perspective(45.0f, (float)size.width / (float)size.height, 0.1f, 100.0f);
    // glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    // glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    // glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // glm::mat4 view = camera.getViewMatrix();
    // glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    // glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    // glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    VertexArray va;
    VertexBuffer vb(cubeVertices, sizeof(cubeVertices));
    VertexBufferLayout vbl;
    vbl.push<float>(3);
    va.addBuffer(vb, vbl);

    // glEnable(GL_CULL_FACE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(glfw.m_Window))
    {
        // 计算deltatime
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // 处理输入
        processInput(glfw.m_Window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        shaderRed.bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
        shaderRed.setUniformMatrix4fv("model", model);
        va.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shaderGreen.bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-left
        shaderRed.setUniformMatrix4fv("model", model);
        va.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shaderBlue.bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move top-left
        shaderRed.setUniformMatrix4fv("model", model);
        va.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shaderYellow.bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move top-left
        shaderRed.setUniformMatrix4fv("model", model);
        va.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
