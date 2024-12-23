#include "shaderClass.h"

// given the file reader and Compatibility issues with C++ and java we have to use g_assetManager to load files
std::string get_file_contents(const char* filename, AAssetManager* g_assetManager) {
    AAsset* asset = AAssetManager_open(g_assetManager, filename, AASSET_MODE_BUFFER);
    if (asset == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "LOG", "Failed to open asset: %s", filename);
        return "";
    }

    size_t size = AAsset_getLength(asset);
    std::string contents(size, '\0');
    AAsset_read(asset, &contents[0], size);
    AAsset_close(asset);
    return contents;
}

Shader::Shader(const char *vertexFile, const char *fragmentFile, AAssetManager* g_assetManager) {
    std::string vertexCode = get_file_contents(vertexFile, g_assetManager);
    std::string fragmentCode = get_file_contents(fragmentFile, g_assetManager);

    const char* vertexSource = vertexCode.c_str(); // explanation of c_str: https://stackoverflow.com/questions/60896478/string-to-const-char-conversion-using-c-str-or-tochararray
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // GLuint is an unsigned integer (aka a positive integer) in a openGL way - reference value
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader); // compile it into machine code

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    GLint compileStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        // Retrieve the error log
        GLint logLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetShaderInfoLog(vertexShader, logLength, nullptr, log.data());

        // Print the error log
        __android_log_print(ANDROID_LOG_INFO, "LOG", "Shader compilation failed: %s \n", log.data());

        // Cleanup and return 0 to indicate failure
        glDeleteShader(vertexShader);
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        // Retrieve the error log
        GLint logLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetShaderInfoLog(fragmentShader, logLength, nullptr, log.data());

        // Print the error log
        __android_log_print(ANDROID_LOG_INFO, "LOG", "Shader compilation failed: %s \n", log.data());

        // Cleanup and return 0 to indicate failure
        glDeleteShader(fragmentShader);
    }

    ID = glCreateProgram(); // to use both shaders we have to warp them up into a shader program

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader); // we delete both because the program is already using them so there is no need
    glDeleteShader(fragmentShader);
}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}
