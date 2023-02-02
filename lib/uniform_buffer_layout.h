#pragma once

#include <vector>
#include <glm/glm.hpp>

struct UniformBufferElement
{
    unsigned int size;
    void* data;
};

class UniformBufferLayout
{
private:
    std::vector<UniformBufferElement> elements;
public:
    void push(void* data)
    {
        // static_assert(false);
        UniformBufferElement ube = { sizeof(glm::mat4), data };
        elements.push_back(ube);
    };
    inline const std::vector<UniformBufferElement> getElements() const
    {
        return elements;
    }
};

// template<>
// inline void UniformBufferLayout::push<glm::mat4*>(glm::mat4* data)
// {
//     UniformBufferElement ube = { sizeof(glm::mat4), data };
//     elements.push_back(ube);
// }
