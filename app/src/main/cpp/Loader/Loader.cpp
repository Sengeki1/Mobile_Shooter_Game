#include "Loader.h"

Loader::Loader(const char *pFileName, AAssetManager* g_assetManager) {
    asset = AAssetManager_open(g_assetManager, pFileName, AASSET_MODE_BUFFER);
    if (asset) {
        size_t assetSize = AAsset_getLength(asset); // get the asset size
        void* assetData = malloc(assetSize); // allocate data to have the same size as the to the assetSize
        AAsset_read(asset, assetData, assetSize);
        AAsset_close(asset);

        // Now pass the assetData to Assimp
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFileFromMemory(assetData, assetSize, aiProcess_Triangulate |
                                                                                        aiProcess_GenSmoothNormals |
                                                                                                aiProcess_FlipUVs |
                                                                                                        aiProcess_JoinIdenticalVertices);

        if (scene) {
            int vertices_accumulation = 0;
            /* Go through each mesh in the scene. */
            for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
                /* Add all the vertices in the mesh to our array. */
                for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
                    const aiVector3D& vector = scene->mMeshes[i]->mVertices[j];
                    vertices.push_back(glm::vec3(vector.x, vector.y, vector.z));
                }

                /*  Add all the indices in the mesh to our array. */
                for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
                    const aiFace& face = scene->mMeshes[i]->mFaces[j];
                    indices.push_back(face.mIndices[0] + vertices_accumulation);
                    indices.push_back(face.mIndices[1] + vertices_accumulation);
                    indices.push_back(face.mIndices[2] + vertices_accumulation);
                }

                /* Keep track of number of vertices loaded so far to use as an offset for the indices. */
                vertices_accumulation += scene->mMeshes[i]->mNumVertices;
            }
            __android_log_print(ANDROID_LOG_INFO, "LOG", "Loaded OBJ Successfully");

        } else {
            __android_log_print(ANDROID_LOG_ERROR, "LOG", "Failed to Load OBJ Scene");
        }

        free(assetData);
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "LOG", "Failed to Load Asset Path");
    }
}

Loader::~Loader() {
    asset = nullptr;
}
