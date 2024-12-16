#ifndef SHOOTERGAME_LOADER_H
#define SHOOTERGAME_LOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <android/log.h>
#include <vector>
#include "../Libraries/glm/glm.hpp"

#include "../AssetManager.h"

class Loader {
    public:
        Loader(const char *pFileName, AAssetManager* g_assetManager);
        ~Loader();

        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
    private:
        AAsset* asset;
};


#endif //SHOOTERGAME_LOADER_H
