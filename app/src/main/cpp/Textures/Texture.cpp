#include "Texture.h"

Texture::Texture(const char* filename, AAssetManager* g_assetManager) {

    auto asset = AAssetManager_open(g_assetManager, filename, AASSET_MODE_BUFFER);
    auto length = AAsset_getLength(asset);
    auto buffer = AAsset_getBuffer(asset);

    if (!buffer) {
        __android_log_print(ANDROID_LOG_INFO, "LOG", "Failed loading Image Directory");
        AAsset_close(asset);
        return;
    }

    bytes = stbi_load_from_memory((const uint8_t *) buffer, length, &textureWidth, &textureHeight, nullptr, 4);
    AAsset_close(asset);

    if (!bytes) {
        __android_log_print(ANDROID_LOG_INFO, "LOG", "Failed loading bytes");
        return;
    }

    glGenTextures(1, &texture); // Generate a Texture buffer
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, textureWidth, textureHeight); // set the image texture in newly created texture unit
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureWidth, textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}
