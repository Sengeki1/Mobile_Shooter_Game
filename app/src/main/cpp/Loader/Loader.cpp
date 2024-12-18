#include "Loader.h"

Loader::Loader(AAssetManager* g_assetManager) {
    // Load OBJ files
    pFileNames = {//std::pair<int, const char*>(0, "Models/Pistol/gun.obj"),
                  //std::pair<int, const char*>(1, "Models/City Block/model.obj"),
                  //std::pair<int, const char*>(2, "Models/Zombie/zombo.obj"),
                  std::pair<int, const char *>(0, "Models/Hand/hand.obj")
    };

    pShaderNames = {  //std::pair <int, std::list<const char*>>(0, {"Shaders/Gun/model.vert", "Shaders/Gun/model.frag"}),
                      //std::pair <int, std::list<const char*>>(1, {"Shaders/City/model.vert", "Shaders/City/model.frag"}),
                      //std::pair <int, std::list<const char*>>(2, {"Shaders/Zombie/model.vert", "Shaders/Zombie/model.frag"}),
                      std::pair <int, std::list<const char*>>(0, {"Shaders/Hand/model.vert", "Shaders/Hand/model.frag"})
    };

    for (int k = 0; k < pFileNames.size(); k++) {
        Meshes.push_back(Mesh_()); // create a new instance of Mesh for storing
//        VAOs.push_back(VAO());
//        VBOs.push_back(VBO()); // for vertices
//        VBOs.push_back(VBO()); // for normals
//        VBOs.push_back(VBO()); // for texture Coordinates

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

            LoadMTL(g_assetManager);

            if (scene) {
                const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

                if (scene->HasMaterials()) {
                    for (int i = 0; i < scene->mNumMaterials; i++) {
                        aiString name;
                        scene->mMaterials[i]->Get(AI_MATKEY_NAME, name);
                        __android_log_print(ANDROID_LOG_INFO, "LOG", "Material Name: %s", name.C_Str());
                        //materials.push_back(getMaterial(scene->mMaterials[i], Shaders[k]));
                    }
                }

                /* Go through each mesh in the scene. */
                totalMesh.push_back(0);
                for (int i = 0; i < scene->mNumMeshes; i++) {
                    static int vertices_accumulation = 0;
                    totalMesh[k]++;
                    Shaders.push_back(Shader(pShaderNames[k].front(), pShaderNames[k].back(), g_assetManager));
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

                    VAOs[i].bind();
                    VBOs[i].bind();
                    EBOs.push_back(EBO(Meshes[k].indices));

                    VBOs[i].addVertices(Meshes[k].vertices);
                    VAOs[i].LinkAttrib(0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0); // a void pointer can hold an address of any type
                    VBOs[i].unbind();

                    if (!Meshes[k].normals.empty()) {
                        VBOs[3 * i + 1].bind();
                        VBOs[3 * i + 1].addVertices(Meshes[k].normals);
                        VAOs[i].LinkAttrib(1, 3, GL_FLOAT, sizeof(glm::vec3), (void *) 0);
                        VBOs[3 * i + 1].unbind();
                    }

                    VBOs[3 * i + 2].bind();
                    VBOs[3 * i + 2].addVertices(Meshes[k].texCoords);
                    VAOs[i].LinkAttrib(2, 2, GL_FLOAT, sizeof(glm::vec2), (void *) 0);
                    VBOs[3 * i + 2].unbind();

                    VAOs[i].unbind();
                    EBOs[i].unbind();

                    /* Keep track of number of vertices loaded so far to use as an offset for the indices. */
                    vertices_accumulation += scene->mMeshes[i]->mNumVertices;
                }
                __android_log_print(ANDROID_LOG_INFO, "LOG", "Loaded OBJ Successfully");

            } else {
                __android_log_print(ANDROID_LOG_ERROR, "LOG", "Failed to Load OBJ Scene");
            }

            free(assetData);
            //scene = nullptr;
        } else {
            __android_log_print(ANDROID_LOG_ERROR, "LOG", "Failed to Load Asset Path");
        }
    }
}

Loader::~Loader() {
    asset = nullptr;
}

//void Loader::Mesh() {
//    for (int i = 0; i < Meshes.size(); i++) {
//        VAOs[i].bind();
//        VBOs[i].bind();
//        EBOs.push_back(EBO(Meshes[i].indices));
//
//        VBOs[i].addVertices(Meshes[i].vertices);
//        VAOs[i].LinkAttrib(0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0); // a void pointer can hold an address of any type
//        VBOs[i].unbind();
//
//        if (!Meshes[i].normals.empty()) {
//            VBOs[i + 1].bind();
//            VBOs[i + 1].addVertices(Meshes[i].normals);
//            VAOs[i].LinkAttrib(1, 3, GL_FLOAT, sizeof(glm::vec3), (void *) 0);
//            VBOs[i + 1].unbind();
//        }
//
//        VBOs[i + 2].bind();
//        VBOs[i + 2].addVertices(Meshes[i].texCoords);
//        VAOs[i].LinkAttrib(2, 2, GL_FLOAT, sizeof(glm::vec2), (void *) 0);
//        VBOs[i + 2].unbind();
//
//        VAOs[i].unbind();
//        EBOs[i].unbind();
//    }
//}

void Loader::RenderMeshes(int width, int height, float angle) {
    for (int k = 0; k < Meshes.size(); k++) {
        for (int i = 0; i < totalMesh[k]; i++) {
            Shaders[i].Activate();
            // Projection
            float inv_aspect = (float) width / (float) height;
            glm::mat4 projection = glm::perspective(45.0f, inv_aspect, 0.1f, 100.0f);
            glUniformMatrix4fv(glGetUniformLocation(Shaders[i].ID, "projection"), 1, GL_FALSE,
                               glm::value_ptr(projection));
            VAOs[i].bind();

            // transformations
            glm::mat4 model = glm::mat4(1.0f);
            model = gunTransformations(model, angle);
            glUniform1f(glGetUniformLocation(Shaders[i].ID, "scale"), 0.5f);
            glUniformMatrix4fv(glGetUniformLocation(Shaders[i].ID, "model"), 1, GL_FALSE,
                               glm::value_ptr(model));

            glDrawElements(GL_TRIANGLES, Meshes[k].indices.size(), GL_UNSIGNED_INT, 0);
        }
    }
}

void Loader::DeleteMeshes() {
    for (int k = 0; k < Meshes.size(); k++) {
        for (int i = 0; i < totalMesh[k]; i++) {
            Shaders[i].Delete();
            VAOs[i].Delete();
            VBOs[i].Delete();
            EBOs[i].Delete();
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

void Loader::LoadMTL(AAssetManager* g_assetManager) {
    AAsset* mtlAsset = AAssetManager_open(g_assetManager, "Models/Pistol/gun.mtl", AASSET_MODE_BUFFER);
    size_t assetSize = AAsset_getLength(mtlAsset);
    const char* assetData[assetSize];

    AAsset_read(mtlAsset, assetData, assetSize);
    AAsset_close(mtlAsset);

    if (mtlAsset != nullptr) {
        __android_log_print(ANDROID_LOG_INFO, "LOG", "%s", assetData);
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