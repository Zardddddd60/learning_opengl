#include "mesh.h"
#include "vertex_buffer_layout.h"

Mesh::Mesh(
    std::vector<Vertex> vertices,
    std::vector<unsigned int> indices,
    std::vector<TextureStruct> textures
): m_Vertices(vertices), m_Indices(indices), m_Textures(textures),
    m_Vao(VertexArray()),
    m_Vbo(VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex))),
    m_Ibo(IndexBuffer(&indices[0], indices.size() * sizeof(unsigned int)))
{
    setupMesh();
}

void Mesh::draw(const Shader& shader)
{
    unsigned int diffsueCount = 1;
    unsigned int specularCount = 1;

    for (unsigned int i = 0; i < m_Textures.size(); i ++)
    {
        const auto& current = m_Textures[i];
        std::string number;
        std::string name = current.type;
        if (name == "texture_diffuse")
        {
            number = std::to_string(diffsueCount ++);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specularCount ++);
        }
        current.texture.bind(i);
        shader.setUniform1i("material." + name + "_" + number, i);
    }

    m_Vao.bind();
    // 使用ibo渲染
    glDrawElements(GL_TRIANGLES, m_Vertices.size(), GL_UNSIGNED_INT, 0);
    m_Vao.unbind();
}

void Mesh::setupMesh()
{
    m_Vao.bind();
    m_Vbo.bind();
    VertexBufferLayout vbl;
    vbl.push<float>(3);
    vbl.push<float>(3);
    vbl.push<float>(2);
    m_Vao.addBuffer(m_Vbo, vbl);

    m_Vao.unbind();
}
