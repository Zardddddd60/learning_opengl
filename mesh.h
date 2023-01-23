#pragma once

#include <string>
#include <vector>
#include "vendors/glm/glm.hpp"
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

    void draw(const Shader& shader);

    inline std::vector<Vertex> getVertices() const {
        return m_Vertices;
    };
    inline std::vector<TextureStruct> getTextures() const {
        return m_Textures;
    };
private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<TextureStruct> m_Textures;
    VertexArray m_Vao;
    VertexBuffer m_Vbo;
    IndexBuffer m_Ibo;

    void setupMesh();
};
