#include "Loader.h"

Loader::Loader(AAssetManager* g_assetManager) {
    pFileNames = {  std::pair<int, const char*>(0, "Models/City Block/model.obj"),
                    std::pair<int, const char*>(1, "Models/Zombie/zombo.obj"),
                    std::pair<int, const char*>(2, "Models/Hand/hand.obj"),
                    std::pair<int, const char*>(3, "Models/Pistol/gun.obj")
    };

    pMTLFileNames = {   std::pair<int, const char*>(0, "Models/City Block/materials.mtl"),
                        std::pair<int, const char*>(1, "Models/Zombie/zombo.mtl"),
                        std::pair<int, const char*>(2, "Models/Hand/hand.mtl"),
                        std::pair<int, const char*>(3, "Models/Pistol/gun.mtl")
    };

    shader = {"Shaders/model.vert", "Shaders/model.frag"};

    // Load OBJ files
    for (int k = 0; k < pFileNames.size(); k++) {
        Meshes.push_back(Mesh_()); // create a new instance of Mesh for storing
        int vertices_accumulation = 0;

        asset = AAssetManager_open(g_assetManager, pFileNames[k], AASSET_MODE_BUFFER);
        if (asset) {
            size_t assetSize = AAsset_getLength(asset); // get the asset size
            void *assetData = malloc(
                    assetSize); // allocate data to have the same size as the to the assetSize
            AAsset_read(asset, assetData, assetSize);
            AAsset_close(asset);

            // Now pass the assetData to Assimp
            Assimp::Importer importer;
            scene = importer.ReadFileFromMemory(assetData, assetSize,
                                                aiProcess_JoinIdenticalVertices |
                                                aiProcess_SortByPType |
                                                aiProcess_FlipUVs |
                                                aiProcess_Triangulate |
                                                aiProcess_GenSmoothNormals |
                                                aiProcess_CalcTangentSpace |
                                                aiProcess_PreTransformVertices);

            if (scene) {
                const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

                if (scene->HasMaterials()) {
                    for (int i = 1; i < scene->mNumMaterials; i++) {
                        aiString name;
                        scene->mMaterials[i]->Get(AI_MATKEY_NAME, name);
                        __android_log_print(ANDROID_LOG_INFO, "LOG", "Material Name: %s", name.C_Str());
                        LoadMTL(g_assetManager, pMTLFileNames[k], i - 1); // maybe put an iterator like the mNumMaterials to get the specific material Kd, Ks, d
                    }
                }

                /* Go through each mesh in the scene. */
                totalMesh.push_back(0);
                static int indexMesh = 0; // because am adding VAOs and VBOs for each Mesh i need to increment the index
                // so i can iterate the CORRECT VBO and VAO for each Mesh
                for (int i = 0; i < scene->mNumMeshes; i++) {
                    totalMesh[k]++; // this serves to count the total Mesh in a model
                    Shaders.push_back(Shader(shader.front(), shader.back(), g_assetManager));
                    VAOs.push_back(VAO());
                    VBOs.push_back(VBO()); // for vertices
                    VBOs.push_back(VBO()); // for normals
                    VBOs.push_back(VBO()); // for texture Coordinates
                    /* Add all the vertices in the mesh to our array. */
                    for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
                        const aiVector3D &vector = scene->mMeshes[i]->mVertices[j];
                        const aiVector3D &texCoord = scene->mMeshes[i]->HasTextureCoords(0) ? (scene->mMeshes[i]->mTextureCoords[0][j]) : Zero3D;

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

                    VAOs[indexMesh].bind();
                    VBOs[indexMesh * 3].bind();
                    EBOs.push_back(EBO(Meshes[k].indices));

                    VBOs[indexMesh * 3].addVertices(Meshes[k].vertices);
                    VAOs[indexMesh].LinkAttrib(0, 3, GL_FLOAT, sizeof(GLfloat) * 3, (void*)0); // a void pointer can hold an address of any type
                    VBOs[indexMesh * 3].unbind();

                    if (!Meshes[k].normals.empty()) {
                        VBOs[indexMesh * 3 + 1].bind();
                        VBOs[indexMesh * 3 + 1].addVertices(Meshes[k].normals);
                        VAOs[indexMesh].LinkAttrib(1, 3, GL_FLOAT, sizeof(GLfloat) * 3, (void *)0);
                        VBOs[indexMesh * 3 + 1].unbind();
                    }

                    VBOs[indexMesh * 3 + 2].bind();
                    VBOs[indexMesh * 3 + 2].addVertices(Meshes[k].texCoords);
                    VAOs[indexMesh].LinkAttrib(2, 2, GL_FLOAT, sizeof(GLfloat) * 2, (void *)0);
                    VBOs[indexMesh * 3 + 2].unbind();

                    VAOs[indexMesh].unbind();
                    EBOs.back().unbind();

                    /* Keep track of number of vertices loaded so far to use as an offset for the indices. */
                    vertices_accumulation += scene->mMeshes[i]->mNumVertices;
                    indexMesh++;
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

    VAOCubeMap = new VAO();
    VBOCubeMap = new VBO();
    EBOCubeMap = new EBO();
    ptrCubeMapShader = new Shader("Shaders/Skybox/skybox.vert", "Shaders/Skybox/skybox.frag", g_assetManager);
    skybox = new Texture(g_assetManager, VAOCubeMap, VBOCubeMap, EBOCubeMap);

    for (int i = 0; i < 4; i++) {
        VAOsSquare[i] = new VAO();
        VBOsSquare[i] = new VBO();
        EBOsSquare[i] = new EBO();

        VAOsSquare[i]->bind();
        VBOsSquare[i]->addVertices(square_vertices);
        EBOsSquare[i]->addIndices(square_indices);
        VAOsSquare[i]->LinkAttrib(0, 2, GL_FLOAT, sizeof(GLfloat) * 2, (void *) 0);
        VAOsSquare[i]->unbind();
        VBOsSquare[i]->unbind();
        EBOsSquare[i]->unbind();
    }

    ptrSquareShader = new Shader("Shaders/JoyStick/model.vert", "Shaders/JoyStick/model.frag",
                                 g_assetManager);

    glDisable(GL_DEPTH_TEST);
}

Loader::~Loader() {
    asset = nullptr;
}

void Loader::RenderMeshes(int width, int height, float angle) {
//    // CubeMap
//    // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
    glDepthFunc(GL_LEQUAL);

    ptrCubeMapShader->Activate();
    Loader::getPerspectiveProjection(width, height, (* ptrCubeMapShader));

    VAOCubeMap->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubeMapTexture);
    glDrawElements(GL_TRIANGLES, skyboxIndices.size(), GL_UNSIGNED_INT, 0);
    VAOCubeMap->unbind();

    // Switch back to the normal depth function
    glDepthFunc(GL_LESS);

    // Meshes OBJ
    int indexMesh = 0;
    for (int k = 0; k < Meshes.size(); k++) {
        for (int i = 0; i < totalMesh[k]; i++) {
            Shaders[indexMesh].Activate();
            // Projection
            Loader::getPerspectiveProjection(width, height, Shaders[indexMesh]);

            VAOs[indexMesh].bind();

            // transformations
            glm::mat4 model = glm::mat4(1.0f);
            if (k == 0 || k == 1) {
                model = cityTransformations(model, angle, Shaders[indexMesh]);
            } else {
                model = gunTransformations(model, angle, Shaders[indexMesh]);
            }

            // Materials
            glUniform3f(glGetUniformLocation(Shaders[indexMesh].ID, "diffuse"), materials[indexMesh].diffuse.x,
                        materials[indexMesh].diffuse.y, materials[indexMesh].diffuse.z);
            glUniform3f(glGetUniformLocation(Shaders[indexMesh].ID, "specular"), materials[indexMesh].specular.x,
                        materials[indexMesh].specular.y, materials[indexMesh].specular.z);
            glUniform1f(glGetUniformLocation(Shaders[indexMesh].ID, "shininess"), materials[indexMesh].shininess);

            glDrawElements(GL_TRIANGLES, Meshes[k].indices.size(), GL_UNSIGNED_INT, 0);

            indexMesh++;
        }
    }

    // JoyStick
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ptrSquareShader->Activate();
    Loader::getOrthographicProjection(width, height, ptrSquareShader);
    for (int i = 0; i < 4; i++) {
        VAOsSquare[i]->bind();
        glUniform1i(glGetUniformLocation(ptrSquareShader->ID, "JOYSTICK_CONTROL"), i);
        glDrawElements(GL_TRIANGLES, square_indices.size(), GL_UNSIGNED_INT, 0);
    }

    glEnable(GL_DEPTH_TEST);
}

void Loader::DeleteMeshes() {
    delete VAOCubeMap;
    delete VBOCubeMap;
    delete EBOCubeMap;
    delete ptrCubeMapShader;
    delete skybox;

    for (int i = 0; i < 4; i++) {
        delete VAOsSquare[i];
        delete VBOsSquare[i];
        delete EBOsSquare[i];
    }
    delete ptrSquareShader;

    int indexMesh = 0;
    for (int k = 0; k < Meshes.size(); k++) {
        for (int i = 0; i < totalMesh[k]; i++) {
            Shaders[indexMesh].Delete();
            VAOs[indexMesh].Delete();
            VBOs[indexMesh].Delete();
            EBOs[indexMesh].Delete();

            indexMesh++;
        }
    }
}

glm::mat4 Loader::gunTransformations(glm::mat4& model, float angle, Shader& shader) {
    model = glm::translate(model, glm::vec3(0.0f, -0.9f, -2.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniform1f(glGetUniformLocation(shader.ID, "scale"), 0.5f);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    return model;
}

glm::mat4 Loader::enemyTransformations(glm::mat4& model, float angle) {
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angle * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

    return model;
}

glm::mat4 Loader::cityTransformations(glm::mat4& model, float angle, Shader& shader) {
    model = glm::translate(model, glm::vec3(0.0f, -0.8f, -8.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniform1f(glGetUniformLocation(shader.ID, "scale"), 20.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    return model;
}

void Loader::getPerspectiveProjection(int width, int height, Shader &shader) {
    float inv_aspect = (float) width / (float) height;
    glm::mat4 projection = glm::perspective(45.0f, inv_aspect, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Loader::getOrthographicProjection(int width, int height, Shader *shader) {
    float aspect_ratio = (float)width/(float)height;
    glm::mat4 projection = glm::ortho(-aspect_ratio, aspect_ratio, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Loader::LoadMTL(AAssetManager* g_assetManager, const char* mtlFile, int index) {
    AAsset* mtlAsset = AAssetManager_open(g_assetManager, mtlFile, AASSET_MODE_BUFFER);
    size_t assetSize = AAsset_getLength(mtlAsset);
    std::string assetData;
    assetData.resize(assetSize);

    AAsset_read(mtlAsset, assetData.data(), assetSize);
    AAsset_close(mtlAsset);

    if (mtlAsset != nullptr) {
        FileIO mtlFile(&assetData);
        if (!mtlFile.Kd.empty()) {
            structMaterial.diffuse = glm::vec3(mtlFile.Kd[index * 3], mtlFile.Kd[index * 3 + 1],
                                               mtlFile.Kd[index * 3 + 2]); // instead of static indice put the iterator of the material
        } else {
            structMaterial.diffuse = glm::vec3(0.0f);
        }
        if (!mtlFile.Ks.empty()) {
            structMaterial.specular = glm::vec3(mtlFile.Ks[index], mtlFile.Ks[index + 1],
                                                mtlFile.Ks[index + 2]);
        } else {
            structMaterial.specular = glm::vec3(0.0f);
        }
        if (!mtlFile.d.empty()) {
            structMaterial.shininess = mtlFile.d[index];
        } else {
            structMaterial.shininess = 0.0f;
        }
        materials.push_back(structMaterial);

        return;
    }
}