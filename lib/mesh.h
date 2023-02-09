#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture.h"
#include "assimp/scene.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    // 纹理坐标
    glm::vec2 texCoords;
};


struct TextureStruct
{
    Texture texture;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(
        std::vector<Vertex> vertices,
        std::vector<unsigned int> indices,
        std::vector<TextureStruct> textures
    );

    void draw(const Shader& shader) const;

    inline std::vector<Vertex> getVertices() const {
        return m_Vertices;
    };
    inline std::vector<TextureStruct> getTextures() const {
        return m_Textures;
    };
    // inline VertexArray& getVAO()
    // {
    //     return m_Vao;
    // }
    std::vector<unsigned int> m_Indices;
    VertexArray m_Vao;
private:
    std::vector<Vertex> m_Vertices;
    std::vector<TextureStruct> m_Textures;
    VertexBuffer m_Vbo;
    IndexBuffer m_Ibo;

    void setupMesh();
};
