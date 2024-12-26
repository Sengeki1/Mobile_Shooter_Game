#include "Texture.h"

//Texture::Texture(const char* filename, AAssetManager* g_assetManager) {
//
//    auto asset = AAssetManager_open(g_assetManager, filename, AASSET_MODE_BUFFER);
//    auto length = AAsset_getLength(asset);
//    auto buffer = AAsset_getBuffer(asset);
//
//    if (!buffer) {
//        __android_log_print(ANDROID_LOG_INFO, "LOG", "Failed loading Image Directory");
//        AAsset_close(asset);
//        return;
//    }
//
//    bytes = stbi_load_from_memory((const uint8_t *) buffer, length, &textureWidth, &textureHeight, nullptr, 4);
//    AAsset_close(asset);
//
//    if (!bytes) {
//        __android_log_print(ANDROID_LOG_INFO, "LOG", "Failed loading bytes");
//        return;
//    }
//
//    glGenTextures(1, &texture); // Generate a Texture buffer
//    glBindTexture(GL_TEXTURE_2D, texture);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, textureWidth, textureHeight); // set the image texture in newly created texture unit
//    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureWidth, textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
//
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    stbi_image_free(bytes);
//}

Texture::Texture(AAssetManager* g_assetManager, VAO* VAO, VBO* VBO, EBO* EBO) {
    (*VAO).bind();
    (*VBO).bind();
    (*VBO).addVertices(sizeof(float) * 3 * 8, skyboxVertices);
    (*EBO).bind();
    (*EBO).addIndices(skyboxIndices);
    (*VBO).bind();
    (*VAO).bind();
    (*VAO).LinkAttrib(0, 3, GL_FLOAT, sizeof(float) * 3, (void*)0);

    (*VAO).unbind();
    (*VBO).unbind();
    (*EBO).unbind();

    glGenTextures(1, &cubeMapTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < 6; i++) {
        AAsset* asset = AAssetManager_open(g_assetManager, facesCubeMap[i].c_str(), AASSET_MODE_BUFFER);
        auto assetSize = AAsset_getLength(asset);
        auto assetData = AAsset_getBuffer(asset);

        if (!assetData) {
            __android_log_print(ANDROID_LOG_INFO, "LOG", "Failed loading Image Directory");
            AAsset_close(asset);
            return;
        }

        int textureWidth, textureHeight, channels;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* bytes = stbi_load_from_memory((const uint8_t *) assetData, assetSize, &textureWidth, &textureHeight, &channels, STBI_rgb_alpha);
        AAsset_close(asset);

        if (!bytes) {
            __android_log_print(ANDROID_LOG_INFO, "LOG", "Failed loading bytes");
            return;
        } else {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
            __android_log_print(ANDROID_LOG_INFO, "LOG", "Loaded CubeMap Face");
            stbi_image_free(bytes);
        }
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &cubeMapTexture);
}
