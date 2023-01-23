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

#include "vendors/glm/glm.hpp"
#include "vendors/glm/gtc/matrix_transform.hpp"
#include "vendors/imgui/imgui.h"
#include "vendors/imgui/imgui_impl_glfw.h"
#include "vendors/imgui/imgui_impl_opengl3.h"

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
const unsigned int cubePositionCount = sizeof(cubePositions) / sizeof(cubePositions[0]);

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};
const unsigned int pointLightPositionCount = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);

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

    VertexArray boxVa;
    VertexBuffer boxVb(vertices, sizeof(vertices));
    VertexBufferLayout boxVbl;
    boxVbl.push<float>(3);
    boxVbl.push<float>(3);
    boxVbl.push<float>(2);
    boxVa.addBuffer(boxVb, boxVbl);

    VertexArray lightVa;
    VertexBuffer lightVb(vertices, sizeof(vertices));
    VertexBufferLayout lightVbl;
    lightVbl.push<float>(3);
    lightVbl.push<float>(3);
    // 虽然没用，但是还要加上要不然strip不对
    lightVbl.push<float>(2);
    lightVa.addBuffer(lightVb, lightVbl);

    Shader boxShader("res/shaders/light/box.vs", "res/shaders/light/box.fs");

    Shader lightShader("res/shaders/light/light.vs", "res/shaders/light/light.fs");

    Texture texture0("res/textures/container2.png");
    Texture texture1("res/textures/container2_specular.png");
    // Texture texture1("res/textures/lighting_maps_specular_color.png");

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

        lightPosVec = glm::vec3(lightPos[0], lightPos[1], lightPos[2]);
        // lightPosVec = glm::vec3(2 * cos(glm::radians(glfwGetTime()) * 100.0f), 1.0f, 2 * sin(glm::radians(glfwGetTime()) * 100.0f));
        lightColorVec = glm::vec3(lightColor[0], lightColor[1], lightColor[2]);

        boxShader.bind();
        glm::mat4 view = camera.getViewMatrix();
        boxShader.setUniformMatrix4fv("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)size.width / size.height, 0.1f, 100.0f);
        boxShader.setUniformMatrix4fv("projection", projection);

        boxShader.setUniformVector3fv("viewPos", camera.getPosision());
        // // 可以近似看成是材料的颜色
        // boxShader.setUniformVector3fv("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        // // 可以看到ambient和diffuse都是材料本身的颜色
        // boxShader.setUniformVector3fv("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        // specular是一个自定义的颜色
        // 
        boxShader.setUniform1f("material.shininess", 32.0f);

        // 光
        boxShader.setUniformVector3fv("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        boxShader.setUniformVector3fv("dirLight.ambient", glm::vec3(0.05f));
        boxShader.setUniformVector3fv("dirLight.diffuse", glm::vec3(0.4f));
        boxShader.setUniformVector3fv("dirLight.specular", glm::vec3(0.5f));

        for (unsigned int i = 0; i < pointLightPositionCount; i ++)
        {
            std::string name = "pointLights[" + std::to_string(i) + "].";
            boxShader.setUniformVector3fv(name + "position", pointLightPositions[i]);
            boxShader.setUniformVector3fv(name + "ambient", glm::vec3(0.05f));
            boxShader.setUniformVector3fv(name + "diffuse", glm::vec3(0.8f));
            boxShader.setUniformVector3fv(name + "specular", glm::vec3(1.0f));
            boxShader.setUniform1f(name + "constant", 1.0f);
            boxShader.setUniform1f(name + "linear", 0.09f);
            boxShader.setUniform1f(name + "quadratic", 0.032f);
        }

        boxShader.setUniformVector3fv("spotLight.position", camera.getPosision());
        boxShader.setUniformVector3fv("spotLight.direction", camera.getFront());
        boxShader.setUniformVector3fv("spotLight.ambient", glm::vec3(0.0f));
        boxShader.setUniformVector3fv("spotLight.diffuse", glm::vec3(1.0f));
        boxShader.setUniformVector3fv("spotLight.specular", glm::vec3(1.0f));
        boxShader.setUniform1f("spotLight.constant", 1.0f);
        boxShader.setUniform1f("spotLight.linear", 0.09f);
        boxShader.setUniform1f("spotLight.quadratic", 0.032f);
        boxShader.setUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        boxShader.setUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f))); 

        // 纹理
        boxShader.setUniform1i("material.diffuse", 0);
        texture0.bind();
        boxShader.setUniform1i("material.specular", 1);
        texture1.bind(1);

        glm::mat4 model(1.0f);
        boxVa.bind();
        for (unsigned int i = 0; i < cubePositionCount; i ++)
        {
            model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            boxShader.setUniformMatrix4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        boxVa.unbind();
        boxShader.unbind();

        lightShader.bind();
        lightShader.bind();
        lightShader.setUniformMatrix4fv("projection", projection);
        lightShader.setUniformMatrix4fv("view", view);
        lightShader.setUniformVector3fv("lightColor", lightColorVec);
        lightVa.bind();
        for (unsigned int i = 0; i < pointLightPositionCount; i ++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setUniformMatrix4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        lightVa.unbind();
        lightShader.unbind();

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
