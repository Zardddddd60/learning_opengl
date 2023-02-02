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

const unsigned int initWidth = 800;
const unsigned int initHeight = 600;

float points[] = {
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
};

int main()
{
    Glfw glfw(800, 600);

    WindowSize size = glfw.getActucalSize();

    SelfImgui imgui(glfw.m_Window);

    Shader shader("res/shaders/geometry-shader/gs.vs", "res/shaders/geometry-shader/gs.fs", "res/shaders/geometry-shader/gs.gs");

    VertexArray va;
    VertexBuffer vb(points, sizeof(points));
    VertexBufferLayout vbl;
    vbl.push<float>(2);
    vbl.push<float>(3);
    va.addBuffer(vb, vbl);

    // glEnable(GL_CULL_FACE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(glfw.m_Window))
    {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        shader.bind();
        va.bind();
        glDrawArrays(GL_POINTS, 0, 4);
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
