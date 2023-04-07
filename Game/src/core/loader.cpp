#include "loader.h"

#include "mesh.h"
#include "vertexBuffer.h"
#include "texture.h"
#include "shaders.h"
#include "xmaterial.h"

// tmp
#include "primitives.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp> 

#include <iostream>

static std::vector<std::string> getTextureFilepaths(aiMaterial* mat, aiTextureType type)
{
    std::vector<std::string> texturePaths;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        texturePaths.push_back(str.C_Str());
    }

    return texturePaths;
}

static std::pair<std::shared_ptr<Mesh>, std::shared_ptr<xMaterial>> processMesh(aiMesh* mesh, const aiScene* scene)
{
    struct Vertex { glm::vec3 pos, normal; glm::vec2 uv; };
    std::vector<Vertex> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 pos;
        pos.x = mesh->mVertices[i].x;
        pos.y = mesh->mVertices[i].y;
        pos.z = mesh->mVertices[i].z;

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;

        glm::vec2 uv{ 0.0f, 0.0f };
        //if (mesh->mTextureCoords[0]) // check whether it contains tex coords
        //{
        //    uv.x = mesh->mTextureCoords[i]->x;
        //    uv.y = mesh->mTextureCoords[i]->y;
        //}

        vertices.push_back(Vertex{ pos,normal,uv });
    }

    // process indices
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    std::vector<std::shared_ptr<Texture>> diffuseMaps;
    std::vector<std::shared_ptr<Texture>> specularMaps;

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<std::string> diffuseMapPaths = getTextureFilepaths(material, aiTextureType_DIFFUSE);
        std::vector<std::string> specularMapPaths = getTextureFilepaths(material, aiTextureType_SPECULAR);

        for (auto& path : diffuseMapPaths)
        {
            Texture map{ path, Texture::TextureFormat::RGBA };
            diffuseMaps.push_back(std::make_shared<Texture>(map));
        }
        for (auto& path : specularMapPaths)
        {
            Texture map{ path, Texture::TextureFormat::RGBA };
            specularMaps.push_back(std::make_shared<Texture>(map));
        }
    }

    VertexBufferLayout layout;
    layout.Add({ 3, GL_FLOAT }); // pos
    layout.Add({ 3, GL_FLOAT }); // normal
    layout.Add({ 2, GL_FLOAT }); // uvs

    auto myMesh = std::make_shared<Mesh>((float*)vertices.data(), vertices.size() * sizeof(Vertex), indices.data(), indices.size(), layout);
    auto myMaterial = std::make_shared<xMaterial>(glm::vec3(1.0f, 1.0f, 1.0f), Shaders::basicLit(), diffuseMaps, specularMaps, 128.0f);

    return std::make_pair(myMesh, myMaterial);
}

static std::vector<std::pair<std::shared_ptr<Mesh>, std::shared_ptr<xMaterial>>> processNode(aiNode* node, const aiScene* scene)
{
    std::vector<std::pair<std::shared_ptr<Mesh>, std::shared_ptr<xMaterial>>> meshesAndMaterials;

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshesAndMaterials.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        auto childMeshesAndMaterials = processNode(node->mChildren[i], scene);
        meshesAndMaterials.insert(meshesAndMaterials.begin(), childMeshesAndMaterials.begin(), childMeshesAndMaterials.end());
    }

    return meshesAndMaterials;
}

Object loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << '\n';
        //return Object{ std::vector<std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>>> {} };
        throw 0; // TODO : make this return invalid object? 
    }
    
    auto meshesAndMaterials = processNode(scene->mRootNode, scene);
    return Object{ meshesAndMaterials };
}