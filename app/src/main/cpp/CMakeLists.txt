# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("shootergame")

# Include Buffers files
set(BUFFERS_SOURCES
        Buffers/VAO.cpp
        Buffers/VBO.cpp
        Buffers/EBO.cpp
)

# Include Texture files
set(TEXTURES_SOURCES
        Textures/Texture.cpp)

## Include Shaders files
set(SHADERS_SOURCES
        Shaders/shaderClass.cpp)

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(shootergame SHARED
        main.cpp
        ${BUFFERS_SOURCES}
        ${SHADERS_SOURCES}
        ${TEXTURES_SOURCES}
        stb.cpp
        Renderer.cpp
)

add_definitions(-DAI_CONFIG_ANDROID_JNI_ASSIMP_MANAGER_SUPPORT)
#libassimp
set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/assimp/include")
set(ASSIMP_LIB_PATH "${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libassimp.so")

include_directories(${ASSIMP_INCLUDE_DIR})

add_library(assimp SHARED IMPORTED)
set_target_properties(assimp
        PROPERTIES
        IMPORTED_LOCATION ${ASSIMP_LIB_PATH}
        INTERFACE_INCLUDE_DIRECTORIES ${ASSIMP_INCLUDE_DIR}
)

# Searches for a package provided by the game activity dependency
find_package(game-activity REQUIRED CONFIG)

#Add libraries
add_subdirectory(Libraries/glm)

# Configure libraries CMake uses to link your target library.
target_link_libraries(shootergame
        # The game activity
        game-activity::game-activity

        # libraries
        glm
        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        EGL
        GLESv3
        jnigraphics
        android
        log

        # Assimp library
        assimp)