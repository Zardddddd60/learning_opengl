#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct WindowSize
{
    int width;
    int height;
};


class Glfw
{
private:
    int m_CalculatedWidth;
    int m_CalculatedHeight;
public:
    Glfw(unsigned int initWidth, unsigned int initHeight);
    ~Glfw();
    
    GLFWwindow* m_Window;

    WindowSize getActucalSize() const;
};
