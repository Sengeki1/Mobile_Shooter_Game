#include "Loader.h"

Loader::Loader(AAssetManager* g_assetManager) {
    // Load OBJ files
    std::vector<const char*> pFileNames = {"Models/Pistol/gun.obj",
                                           "Models/Hand/hand.obj",
                                           "Models/City Block/model.obj",
                                           "Models/Zombie/Zombo.obj"};

    for (int k = 0; k < pFileNames.size(); k++) {
        Meshes.push_back(Mesh_()); // create a new instance of Mesh for storing
        Shaders.push_back(Shader("model.vert", "model.frag", g_assetManager));
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
                        const aiVector3D &normal = scene->mMeshes[i]->mNormals[j];
                        const aiVector3D &texCoord = scene->mMeshes[i]->HasTextureCoords(0) ?
                                                     (scene->mMeshes[i]->mTextureCoords[0][i])
                                                                                            : Zero3D;

                        Meshes[k].vertices.push_back(glm::vec3(vector.x, vector.y, vector.z));
                        Meshes[k].normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
                        Meshes[k].texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
                    }

                    /*  Add all the indices in the mesh to our array. */
                    for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
                        const aiFace &face = scene->mMeshes[i]->mFaces[j];
                        assert(face.mNumIndices == 3);
                        Meshes[k].indices.push_back(face.mIndices[0] + vertices_accumulation);
                        Meshes[k].indices.push_back(face.mIndices[1] + vertices_accumulation);
                        Meshes[k].indices.push_back(face.mIndices[2] + vertices_accumulation);
                    }
                    EBOs.push_back(EBO(Meshes[k].indices));

                    __android_log_print(ANDROID_LOG_INFO, "LOG", "vertices %zu", Meshes[k].vertices.size());

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
}

Loader::~Loader() {
    asset = nullptr;
}

void Loader::Mesh() {
    for (int i = 0; i < Meshes.size(); i++) {
        VAOs[i].bind();
        VBOs[i].bind();

        VBOs[i].addVertices(Meshes[i].vertices);
        EBOs[i].bind();
        VAOs[i].LinkAttrib(0, 3, GL_FLOAT, sizeof(GLfloat) * 3, (void*)0); // a void pointer can hold an address of any type

        VAOs[i].unbind();
        VBOs[i].unbind();
        EBOs[i].unbind();
    }
}

void Loader::RenderMeshes(int width, int height, int angle) {
    for (int i = 0; i < Meshes.size(); i++) {

        Shaders[i].Activate();
        VAOs[i].bind();
        // Projection
        float inv_aspect = (float)width / (float)height;
        glm::mat4 projection = glm::perspective(45.0f, inv_aspect, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(Shaders[i].ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // transformations
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.9f, -3.0f));
         model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(angle * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(Shaders[i].ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(Shaders[i].ID, "scale"), 0.5f);

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







