#ifndef SHOOTERGAME_LOADER_H
#define SHOOTERGAME_LOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <android/log.h>
#include <vector>
#include "../Libraries/glm/glm.hpp"

#include "../AssetManager.h"

#include "../Buffers/VAO.h"
#include "../Buffers/VBO.h"
#include "../Buffers/EBO.h"
#include "../Shaders/shaderClass.h"
#include "../Textures/Texture.h"
#include "../Renderer.h"

typedef struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
} Mesh_;

class Loader {
    public:
        Loader(AAssetManager* g_assetManager);
        ~Loader();

        void Mesh();
        void DeleteMeshes();
        void RenderMeshes(int width, int height, int angle);
    private:
        AAsset* asset;
        std::vector<VAO> VAOs;
        std::vector<VBO> VBOs;
        std::vector<EBO> EBOs;
        std::vector<Shader> Shaders;
        std::vector<Texture> Textures;
        std::vector<Mesh_> Meshes;
};


#endif //SHOOTERGAME_LOADER_H
