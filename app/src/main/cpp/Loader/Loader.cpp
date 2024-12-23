#include "Loader.h"

Loader::Loader(AAssetManager* g_assetManager) {
    // Load OBJ files
    pFileNames = {  std::pair<int, const char*>(0, "Models/Pistol/gun.obj"),
                    std::pair<int, const char*>(1, "Models/Hand/hand.obj")
    };

    // Load MTL files
    pMTLFileNames = {   std::pair<int, const char*>(0, "Models/Pistol/gun.mtl"),
                        std::pair<int, const char*>(1, "Models/Hand/hand.mtl")
    };

    shader = {"Shaders/model.vert", "Shaders/model.frag"};

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
                    for (int i = 0; i < scene->mNumMaterials - 1; i++) {
                        aiString name;
                        scene->mMaterials[i + 1]->Get(AI_MATKEY_NAME, name);
                        __android_log_print(ANDROID_LOG_INFO, "LOG", "Material Name: %s", name.C_Str());
                        LoadMTL(g_assetManager, pMTLFileNames[k], i); // maybe put an iterator like the mNumMaterials to get the specific material Kd, Ks, d
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
}

Loader::~Loader() {
    asset = nullptr;
}

void Loader::RenderMeshes(int width, int height, float angle) {
    int indexMesh = 0;
    for (int k = 0; k < Meshes.size(); k++) {
        for (int i = 0; i < totalMesh[k]; i++) {
            Shaders[indexMesh].Activate();
            // Projection
            float inv_aspect = (float) width / (float) height;
            glm::mat4 projection = glm::perspective(45.0f, inv_aspect, 0.1f, 100.0f);
            glUniformMatrix4fv(glGetUniformLocation(Shaders[indexMesh].ID, "projection"), 1, GL_FALSE,
                               glm::value_ptr(projection));
            VAOs[indexMesh].bind();

            // transformations
            glm::mat4 model = glm::mat4(1.0f);
            if (pFileNames[k] == "Models/Pistol/gun.obj") {
                model = gunTransformations(model, angle);
            } else {
                model = enemyTransformations(model, angle);
            }
            glUniform1f(glGetUniformLocation(Shaders[indexMesh].ID, "scale"), 0.5f);
            glUniformMatrix4fv(glGetUniformLocation(Shaders[indexMesh].ID, "model"), 1, GL_FALSE,
                               glm::value_ptr(model));

            glDrawElements(GL_TRIANGLES, Meshes[k].indices.size(), GL_UNSIGNED_INT, 0);

            indexMesh++;
        }
    }
}

void Loader::DeleteMeshes() {
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

glm::mat4 Loader::gunTransformations(glm::mat4& model, float angle) {
    model = glm::translate(model, glm::vec3(0.0f, -0.9f, -3.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angle * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

    return model;
}

glm::mat4 Loader::enemyTransformations(glm::mat4& model, float angle) {
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
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

void Loader::LoadMTL(AAssetManager* g_assetManager, const char* mtlFile, int index) {
    AAsset* mtlAsset = AAssetManager_open(g_assetManager, mtlFile, AASSET_MODE_BUFFER);
    size_t assetSize = AAsset_getLength(mtlAsset);
    std::string assetData;
    assetData.resize(assetSize);

    AAsset_read(mtlAsset, assetData.data(), assetSize);
    AAsset_close(mtlAsset);

    if (mtlAsset != nullptr) {
        FileIO mtlFile(&assetData);
        structMaterial.diffuse = glm::vec3(mtlFile.Kd[index * 3], mtlFile.Kd[index * 3 + 1], mtlFile.Kd[index * 3 + 2]); // instead of static indice put the iterator of the material
        __android_log_print(ANDROID_LOG_INFO, "LOG", "Loaded OBJ diffuse: %s", glm::to_string(structMaterial.diffuse).c_str());
        //structMaterial.specular = glm::vec3(mtlFile.Ks[index], mtlFile.Ks[index + 1], mtlFile.Ks[index + 2]);
        //__android_log_print(ANDROID_LOG_INFO, "LOG", "Loaded OBJ specular: %s", glm::to_string(structMaterial.specular).c_str());
        //structMaterial.shininess = mtlFile.d[index];
        //__android_log_print(ANDROID_LOG_INFO, "LOG", "Loaded OBJ shininess: %f", structMaterial.shininess);
        //materials.push_back(structMaterial);
        return;
    }
}

Material Loader::getMaterial(aiMaterial *material, Shader& shader) {

    aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS ) {
        structMaterial.diffuse = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
        __android_log_print(ANDROID_LOG_INFO, "LOG", "Loaded OBJ diffuse: %s", glm::to_string(structMaterial.diffuse).c_str());
    }

    aiColor3D specularColor(0.0f, 0.0f, 0.0f);
    if (material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS) {
        structMaterial.specular = glm::vec3(specularColor.r, specularColor.g, specularColor.b);
        __android_log_print(ANDROID_LOG_INFO, "LOG", "Loaded OBJ specular: %s", glm::to_string(structMaterial.specular).c_str());
    }

    float shininess;
    if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
        structMaterial.shininess = shininess;
        __android_log_print(ANDROID_LOG_INFO, "LOG", "Loaded OBJ shininess: %f", structMaterial.shininess);
    }

    return structMaterial;
}