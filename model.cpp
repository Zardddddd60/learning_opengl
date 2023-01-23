#include <iostream>
#include "model.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

Texture textureFromFile(const char* path, const std::string& directory, bool gamma = false);

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (
        !scene ||
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode
    )
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    m_Directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i ++)
    {
        // node.mMeshed存的时scene.mMeshes中的索引
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i ++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::draw(const Shader& shader)
{
    std::cout << m_Meshes.size() << std::endl;
    for (auto& mesh: m_Meshes)
    {
        mesh.draw(shader);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    // 处理vertex
    for (unsigned int i = 0; i < mesh->mNumVertices; i ++)
    {
        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;

        glm::vec2 texCoords;
        if (mesh->mTextureCoords[0])
        {
            // 只关心第一组纹理坐标
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            texCoords = glm::vec2(0.0f);
        }

        Vertex vertex = {
            position,
            normal,
            texCoords
        };
        vertices.push_back(vertex);
    }

    std::vector<unsigned int> indices;
    // 每个mesh有一组面（face），每个face是一个图元
    // 一个面又有多个顶点；
    // face.mIndices中存储了面中顶点对应的索引，以及面中顶点的绘制顺序
    for (unsigned int i = 0; i < mesh->mNumFaces; i ++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j ++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::vector<TextureStruct> textures;

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<TextureStruct> diffuseMaps = loadMaterialTextures(
            material,
            aiTextureType_DIFFUSE,
            "texture_diffuse"
        );
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<TextureStruct> specularMaps = loadMaterialTextures(
            material,
            aiTextureType_SPECULAR,
            "texture_specular"
        );
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<TextureStruct> Model::loadMaterialTextures(
    aiMaterial* materital,
    aiTextureType type,
    std::string typeName
)
{
    std::vector<TextureStruct> textures;
    for (unsigned int i = 0; i < materital->GetTextureCount(type); i ++)
    {
        aiString str;
        materital->GetTexture(type, i, &str);

        bool skip = false;
        for (auto& loadedTexture: m_LoadedTextures)
        {
            if (std::strcmp(loadedTexture.path.data(), str.C_Str()) == 0)
            {
                textures.push_back(loadedTexture);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture = textureFromFile(str.C_Str(), m_Directory);
            TextureStruct textureStruct = {
                texture,
                typeName,
                str.C_Str()
            };
            textures.push_back(textureStruct);
            m_LoadedTextures.push_back(textureStruct);
        }

    }

    return textures;
}

Texture textureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    Texture texture(filename);
    return texture;
}
