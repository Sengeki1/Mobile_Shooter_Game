#include "Texture.h"

Texture::Texture(const char* filename) {
    __android_log_print(ANDROID_LOG_INFO, "LOG", "Loading Image..");
    bytes = stbi_load(filename,  &textureWidth, &textureHeight, &nrChannels, 0);
    __android_log_print(ANDROID_LOG_INFO, "LOG", "Image Loaded");

    if (!bytes) {
        __android_log_print(ANDROID_LOG_INFO, "LOG", "Failed loading Image");
    }

    glGenTextures(1, &texture); // Generate a Texture buffer
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, texture); // make the newly created texture current
}

void Texture::setTexture(int i) {
    glActiveTexture(GL_TEXTURE0 + i); // Activate a new texture unit
    glBindTexture(GL_TEXTURE_2D, texture); // make the newly created texture current

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes); // set the image texture in newly created texture unit
    glGenerateMipmap(GL_TEXTURE_2D); // idk but we gotta generate a mipmap to the image to appear
}

void Texture::Unbind() {
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbinding it
}

void Texture::Delete() {
    glDeleteTextures(1, &texture);
}
