#pragma once

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "mesh.h"
#include "shader.h"

class Model
{
public:
    Model(const std::string& path)
    {
        loadModel(path);
    }
    void draw(const Shader& shader);
    // std::vector<Mesh> getMeshes() const;
    std::vector<Mesh> m_Meshes;
private:
    std::string m_Directory;
    std::vector<TextureStruct> m_LoadedTextures;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureStruct> loadMaterialTextures(
        aiMaterial* material,
        aiTextureType type,
        std::string typeName
    );
};
