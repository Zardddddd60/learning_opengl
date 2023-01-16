#include <iostream>
// 管理OpenGL的函数指针
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
    "} ";

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
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 校验shader是否compile成功
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        char info[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, info);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info << std::endl;
    }

    // 创建shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        char info[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, info);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info << std::endl;
    }

    // 创建program
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        char info[512];
        glGetProgramInfoLog(program, 512, NULL, info);
        std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << info << std::endl;
    }
    // 激活
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ----------------------------- buffers ---------------------------------
    // 定点坐标 
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // 绑定的VBO
    unsigned int VBO;
    // 一个通用的buffer，
    glGenBuffers(1, &VBO);
    // bind之后，任意对GL_ARRAY_BUFFER的调用，都用来配置VBO的缓冲；
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 基本不会变，使用GL_STATIC_DRAW
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // 绑定对应的vbo和属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 在调用glVertexAttribPointer后，就可以解绑vbo的内容了
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    // 这里绑定的EBO会存在bind的VAO里，之后切换不同VAO，则它的EBO也会切换
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // EBO不能解绑，因为它与VAO是绑定的
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 线框模式绘制
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

        glUseProgram(program);
        // 这个VAO绑定的EBO
        glBindVertexArray(VAO);
        // 从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取其索引
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // 解绑
        glBindVertexArray(0);

        // 交换窗口每一个像素颜色值的缓冲，被绘制出来
        // 使用“双缓冲”：前缓冲存展示的装填，后缓冲存下次渲染（即指令作用在后缓冲）
        glfwSwapBuffers(window);
        // 检查键盘，鼠标的事件，更新窗口状态，触发注册的回调函数
        glfwPollEvents();
    }

    // 清理工作
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);


    glfwTerminate();
    return 0;
}