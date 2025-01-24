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
    enemyShader = {"Shaders/Enemy/model.vert", "Shaders/Enemy/model.frag"};

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
                    if (k == 1) {
                        Shaders.push_back(Shader(enemyShader.front(), enemyShader.back(), g_assetManager));
                    } else {
                        Shaders.push_back(Shader(shader.front(), shader.back(), g_assetManager));
                    }
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

    square_normals = squareNormals();
    for (int i = 0; i < 4; i++) {
        VAOsSquare[i] = new VAO();
        VBOsSquare[i] = new VBO();
        VBOsSquareNormals[i] = new VBO();
        EBOsSquare[i] = new EBO();

        VAOsSquare[i]->bind();
        VBOsSquare[i]->addVertices(square_vertices);
        VBOsSquareNormals[i]->addVertices(square_normals);
        EBOsSquare[i]->addIndices(square_indices);
        VBOsSquare[i]->bind();
        VAOsSquare[i]->LinkAttrib(0, 2, GL_FLOAT, sizeof(GLfloat) * 2, (void *) 0);
        VBOsSquareNormals[i]->bind();
        VAOsSquare[i]->LinkAttrib(1, 3, GL_FLOAT, sizeof(GLfloat) * 3, (void *) 0);
        VAOsSquare[i]->unbind();
        VBOsSquare[i]->unbind();
        VBOsSquareNormals[i]->unbind();
        EBOsSquare[i]->unbind();
    }

    ptrSquareShader = new Shader("Shaders/JoyStick/model.vert", "Shaders/JoyStick/model.frag",
                                 g_assetManager);

    glDisable(GL_DEPTH_TEST);
}

Loader::~Loader() {
    asset = nullptr;
}

void Loader::RenderMeshes(int width, int height, float deltaTime, glm::vec2 motionXY, bool* touch, bool* button_touch, android_app *app) {
    if ((*touch) && newTouch) {
        camera.firstTouch = true;
        newTouch = false;
    }
    if (!(*touch)) {
        newTouch = true;
    } else {
        camera.mouse(motionXY.x, motionXY.y);
    }

    // CubeMap
    // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
    glDepthFunc(GL_LEQUAL);

    ptrCubeMapShader->Activate();
    float inv_aspect = (float) width / (float) height;
    glm::mat4 projection = glm::perspective(45.0f, inv_aspect, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(ptrCubeMapShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    VAOCubeMap->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubeMapTexture);
    glDrawElements(GL_TRIANGLES, skyboxIndices.size(), GL_UNSIGNED_INT, 0);
    VAOCubeMap->unbind();

    // Switch back to the normal depth function
    glDepthFunc(GL_LESS);

    // Meshes OBJ
    int indexMesh = 0;
    static float counter = 0.0f;
    for (int k = 0; k < Meshes.size(); k++) {
        for (int i = 0; i < totalMesh[k]; i++) {
            if (k == 1) {
                counter += 0.001f;
                if (counter >= 1.0f) {
                    enemies_count += 1;
                    counter = 0.0f;
                }
                for (int j = 0; j < enemies_count; j++) {
                    Shaders[indexMesh].Activate();
                    // Projection
                    camera.setCamera(width, height, Shaders[indexMesh], getPerspectiveProjection);

                    VAOs[indexMesh].bind();

                    // transformations
                    glm::mat4 model = glm::mat4(1.0f);

                    glUniform1i(glGetUniformLocation(Shaders[indexMesh].ID, "ID"), k);
                    enemyTransformations(model, deltaTime, Shaders[indexMesh], camera, app);

                    // Materials
                    glUniform3f(glGetUniformLocation(Shaders[indexMesh].ID, "diffuse"),
                                materials[indexMesh].diffuse.x,
                                materials[indexMesh].diffuse.y, materials[indexMesh].diffuse.z);
                    glUniform3f(glGetUniformLocation(Shaders[indexMesh].ID, "specular"),
                                materials[indexMesh].specular.x,
                                materials[indexMesh].specular.y, materials[indexMesh].specular.z);
                    glUniform1f(glGetUniformLocation(Shaders[indexMesh].ID, "shininess"),
                                materials[indexMesh].shininess);

                    glDrawElements(GL_TRIANGLES, Meshes[k].indices.size(), GL_UNSIGNED_INT, 0);
                }
                indexMesh++;
            } else {
                Shaders[indexMesh].Activate();
                // Projection
                camera.setCamera(width, height, Shaders[indexMesh], getPerspectiveProjection);

                if (k == 2 || k == 3) {
                    glUniformMatrix4fv(glGetUniformLocation(Shaders[indexMesh].ID, "view"), 1,
                                       GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
                }

                VAOs[indexMesh].bind();

                // transformations
                glm::mat4 model = glm::mat4(1.0f);

                glUniform1i(glGetUniformLocation(Shaders[indexMesh].ID, "ID"), k);
                if (k == 0) {
                    cityTransformations(model, deltaTime, Shaders[indexMesh]);
                } else {
                    gunTransformations(model, deltaTime, Shaders[indexMesh]);
                }

                // Materials
                glUniform3f(glGetUniformLocation(Shaders[indexMesh].ID, "diffuse"),
                            materials[indexMesh].diffuse.x,
                            materials[indexMesh].diffuse.y, materials[indexMesh].diffuse.z);
                glUniform3f(glGetUniformLocation(Shaders[indexMesh].ID, "specular"),
                            materials[indexMesh].specular.x,
                            materials[indexMesh].specular.y, materials[indexMesh].specular.z);
                glUniform1f(glGetUniformLocation(Shaders[indexMesh].ID, "shininess"),
                            materials[indexMesh].shininess);

                glDrawElements(GL_TRIANGLES, Meshes[k].indices.size(), GL_UNSIGNED_INT, 0);

                indexMesh++;
            }
        }
    }

    // JoyStick
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ptrSquareShader->Activate();
    glm::mat4 orthographicProjection = getOrthographicProjection(width, height, ptrSquareShader);

    glm::vec3 mouse_ndc = convertNDC(motionXY, width, height);

    for (int i = 0; i < 4; i++) {
        VAOsSquare[i]->bind();

        if ((*button_touch)) {
            glm::vec2 min, max;

            if (i == 0) {
                std::vector<glm::vec2> min_max = min_maxSquare(orthographicProjection, -8.0f, 0.0f);
                min = min_max[0];
                max = min_max[1];
            } else if (i == 1) {
                std::vector<glm::vec2> min_max = min_maxSquare(orthographicProjection, -5.0f, 0.0f);
                min = min_max[0];
                max = min_max[1];
            }  else if (i == 2) {
                std::vector<glm::vec2> min_max = min_maxSquare(orthographicProjection, -6.4f, -1.5f);
                min = min_max[0];
                max = min_max[1];
            }  else if (i == 3) {
                std::vector<glm::vec2> min_max = min_maxSquare(orthographicProjection, -6.4f, 1.5f);
                min = min_max[0];
                max = min_max[1];
            }

            if (mouse_ndc.x >= min.x && mouse_ndc.x <= max.x &&
                mouse_ndc.y >= min.y && mouse_ndc.y <= max.y) {

                if (i == 0) {
                    camera.position += (camera.speed) * glm::normalize(glm::cross(camera.upDirection, camera.orientation));
                } else if (i == 1) {
                    camera.position += (camera.speed) * glm::normalize(-glm::cross(camera.upDirection, camera.orientation));
                }  else if (i == 2) {
                    camera.position += (camera.speed) * glm::normalize(glm::cross(camera.upDirection, glm::normalize(glm::cross(camera.upDirection, camera.orientation))));
                }  else if (i == 3) {
                    camera.position += (camera.speed) * glm::normalize(glm::cross(camera.upDirection, glm::normalize(-glm::cross(camera.upDirection, camera.orientation))));
                }
            }
        }

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

void Loader::gunTransformations(glm::mat4& model, float angle, Shader& shader) {
    model = glm::translate(model, glm::vec3(0.0f, -0.9f, -2.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniform1f(glGetUniformLocation(shader.ID, "scale"), 0.5f);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Loader::enemyTransformations(glm::mat4& model, float deltaTime, Shader& shader, Camera& camera, android_app *app) {

    // Translate enemy to a position
    glm::vec3 position = glm::vec3(0.0f, -1.5f, -5.0f);
    model = glm::translate(model, position);

    // extract direction and calculate rotation matrix
    glm::vec3 direction = -glm::vec3(camera.orientation.x, 0.0f, camera.orientation.z);
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, -1.0f, 0.0f), direction));
    glm::vec3 up = glm::normalize(glm::cross(right, direction));

    glm::mat4 rotation_matrix = {
            right.x,     right.y,     right.z,       0,  // First row (right vector)
            up.x,        up.y,        up.z,          0,  // Second row (up vector)
            direction.x, direction.y, direction.z,   0,  // Third row (direction vector)
            0,           0,           0,             1   // Fourth row (translation component)
    };

    // move the enemy
    glm::vec3 new_pos = -(glm::vec3(1.0f, 0.0f, 1.0f) +
                          glm::vec3(position.x - camera.position.x, 0.0f, position.z - camera.position.z)) * (deltaTime * 0.0005f);
    model = glm::translate(model, new_pos);

    glm::vec3 current_pos = position + new_pos;
    if (glm::length(current_pos - camera.position) < 2.9f) {
        //app->destroyRequested = 1;
        //GameActivity_finish(app->activity);
    }

    model *= rotation_matrix;

    glUniform1f(glGetUniformLocation(shader.ID, "scale"), 0.4f);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Loader::cityTransformations(glm::mat4& model, float angle, Shader& shader) {
    model = glm::translate(model, glm::vec3(0.0f, -1.8f, -8.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniform1f(glGetUniformLocation(shader.ID, "scale"), 50.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

glm::mat4 getPerspectiveProjection(int width, int height, Shader &shader) {
    float inv_aspect = (float) width / (float) height;
    glm::mat4 projection = glm::perspective(45.0f, inv_aspect, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    return projection;
}

glm::mat4 getOrthographicProjection(int width, int height, Shader *shader) {
    float aspect_ratio = (float)width/(float)height;
    glm::mat4 projection = glm::ortho(-aspect_ratio, aspect_ratio, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    return projection;
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