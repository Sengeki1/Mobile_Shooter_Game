#include "Loader.h"

Loader::Loader(AAssetManager* g_assetManager) {
    // Load OBJ files
    pFileNames = {  std::pair<int, const char*>(0, "Models/Hand/hand.obj"),
                    std::pair<int, const char*>(1, "Models/City Block/model.obj"),
                    std::pair<int, const char*>(2, "Models/Zombie/zombo.obj"),
                    std::pair<int, const char*>(3, "Models/Pistol/gun.obj") };

    pShaderNames = {  std::pair <int, std::list<const char*>>(0, {"model.vert", "model.frag"}),
                      std::pair <int, std::list<const char*>>(1, {"model.vert", "model.frag"}),
                      std::pair <int, std::list<const char*>>(2, {"model.vert", "model.frag"}),
                      std::pair <int, std::list<const char*>>(3, {"model.vert", "model.frag"}) };

    for (int k = 0; k < pFileNames.size(); k++) {
        Meshes.push_back(Mesh_()); // create a new instance of Mesh for storing
        Shaders.push_back(Shader(pShaderNames[k].front(), pShaderNames[k].back(), g_assetManager));
        VAOs.push_back(VAO());
        VBOs.push_back(VBO());

        asset = AAssetManager_open(g_assetManager, pFileNames[k], AASSET_MODE_BUFFER);
        if (asset) {
            size_t assetSize = AAsset_getLength(asset); // get the asset size
            void *assetData = malloc(
                    assetSize); // allocate data to have the same size as the to the assetSize
            AAsset_read(asset, assetData, assetSize);
            AAsset_close(asset);

            // Now pass the assetData to Assimp
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFileFromMemory(assetData, assetSize,
                                                               aiProcess_JoinIdenticalVertices |
                                                               aiProcess_SortByPType |
                                                               aiProcess_FlipUVs |
                                                               aiProcess_Triangulate);

            if (scene) {
                int vertices_accumulation = 0;
                const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
                /* Go through each mesh in the scene. */
                for (int i = 0; i < scene->mNumMeshes; i++) {
                    /* Add all the vertices in the mesh to our array. */
                    for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
                        const aiVector3D &vector = scene->mMeshes[i]->mVertices[j];
                        const aiVector3D &texCoord = scene->mMeshes[i]->HasTextureCoords(0) ? (scene->mMeshes[i]->mTextureCoords[0][i]) : Zero3D;

                        Meshes[k].vertices.push_back(glm::vec3(vector.x, vector.y, vector.z));
                        Meshes[k].texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));

                        if (scene->mMeshes[i]->mNormals) {
                            const aiVector3D &normal = scene->mMeshes[i]->mNormals[j];
                            Meshes[k].normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
                        }
                    }

                    /*  Add all the indices in the mesh to our array. */
                    for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
                        const aiFace &face = scene->mMeshes[i]->mFaces[j];
                        assert(face.mNumIndices == 3);
                        Meshes[k].indices.push_back(face.mIndices[0] + vertices_accumulation);
                        Meshes[k].indices.push_back(face.mIndices[1] + vertices_accumulation);
                        Meshes[k].indices.push_back(face.mIndices[2] + vertices_accumulation);
                    }

                    __android_log_print(ANDROID_LOG_INFO, "LOG", "vertices %zu", Meshes[k].vertices.size());

                    /* Keep track of number of vertices loaded so far to use as an offset for the indices. */
                    vertices_accumulation += scene->mMeshes[i]->mNumVertices;
                }
                __android_log_print(ANDROID_LOG_INFO, "LOG", "Loaded OBJ Successfully");

            } else {
                __android_log_print(ANDROID_LOG_ERROR, "LOG", "Failed to Load OBJ Scene");
            }

            free(assetData);
            scene = nullptr;
        } else {
            __android_log_print(ANDROID_LOG_ERROR, "LOG", "Failed to Load Asset Path");
        }
    }
}

Loader::~Loader() {
    asset = nullptr;
}

void Loader::Mesh() {
    for (int i = 0; i < Meshes.size(); i++) {
        VAOs[i].bind();
        VBOs[i].bind();
        EBOs.push_back(EBO(Meshes[i].indices));

        VBOs[i].addVertices(Meshes[i].vertices);
        VAOs[i].LinkAttrib(0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0); // a void pointer can hold an address of any type

        VAOs[i].unbind();
        VBOs[i].unbind();
        EBOs[i].unbind();
    }
}

void Loader::RenderMeshes(int width, int height, float angle) {
    for (int i = 0; i < Meshes.size(); i++) {
        Shaders[i].Activate();
        // Projection
        float inv_aspect = (float) width / (float) height;
        glm::mat4 projection = glm::perspective(45.0f, inv_aspect, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(Shaders[i].ID, "projection"), 1, GL_FALSE,
                           glm::value_ptr(projection));
        VAOs[i].bind();

        // transformations
        glm::mat4 model = glm::mat4(1.0f);
        glUniform1f(glGetUniformLocation(Shaders[i].ID, "scale"), 1.0f);

        if (pFileNames[i] == "Models/Pistol/gun.obj") {
            model = gunTransformations(model, angle);
            glUniform1f(glGetUniformLocation(Shaders[i].ID, "scale"), 0.5f);
        }
        glUniformMatrix4fv(glGetUniformLocation(Shaders[i].ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glDrawElements(GL_LINE_LOOP, Meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
    }
}

void Loader::DeleteMeshes() {
    for(int i = 0; i < Meshes.size(); i++) {
        Shaders[i].Delete();
        VAOs[i].Delete();
        VBOs[i].Delete();
        EBOs[i].Delete();
    }
}

glm::mat4 Loader::gunTransformations(glm::mat4& model, float angle) {
    model = glm::translate(model, glm::vec3(0.0f, -0.9f, -3.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angle * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

    return model;
}

glm::mat4 Loader::enemyTransformations(glm::mat4& model, float angle) {
    model = glm::translate(model, glm::vec3(0.0f, -0.9f, -3.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angle * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

    return model;
}

glm::mat4 Loader::cityTransformations(glm::mat4& model, float angle) {
    model = glm::translate(model, glm::vec3(0.0f, -0.9f, -3.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angle * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

    return model;
}







