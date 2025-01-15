#ifndef SHOOTERGAME_LOADER_H
#define SHOOTERGAME_LOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <android/log.h>
#include <vector>
#include <map>
#include <list>

#include "../AssetManager.h"
#include <GLES3/gl3.h>

#include "../Buffers/VAO.h"
#include "../Buffers/VBO.h"
#include "../Buffers/EBO.h"
#include "../Shaders/shaderClass.h"
#include "../Textures/Texture.h"
#include "../FileIO/FileIO.h"
#include "../Camera/Camera.h"
#include "../Touch.h"

#include "../Mesh/Square.h"

#include "../Libraries/glm/glm.hpp"
#include "../Libraries/glm/gtc/matrix_transform.hpp"
#include "../Libraries/glm/gtc/type_ptr.hpp"
#include "../Libraries/glm/gtx/string_cast.hpp"

typedef struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
} Mesh_;

struct Material {
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class Loader {
    public:
        Loader(AAssetManager* g_assetManager);
        ~Loader();

        void Mesh();
        void DeleteMeshes();
        void LoadMTL(AAssetManager* g_assetManager, const char* mtlFile, int index);
        void RenderMeshes(int width, int height, float angle, glm::vec2 motionXY, bool *touch);

    private:
        AAsset* asset;
        std::map<int, const char*> pFileNames;
        std::map<int, const char*>pMTLFileNames;
        std::list<const char *> shader;
        struct Material structMaterial;
        const aiScene *scene;
        std::vector<int> totalMesh;

        std::vector<VAO> VAOs;
        std::vector<VBO> VBOs;
        std::vector<EBO> EBOs;
        std::vector<Shader> Shaders;
        std::vector<Texture> Textures;
        std::vector<Mesh_> Meshes;
        std::vector<Material> materials;

        Camera camera;
        bool newTouch = true;

        glm::mat4 enemyTransformations(glm::mat4 &model, float angle);
        glm::mat4 gunTransformations(glm::mat4 &model, float angle, Shader& shader);
        glm::mat4 cityTransformations(glm::mat4 &model, float angle, Shader& shader);

        // Cube Map
        VAO* VAOCubeMap;
        VBO* VBOCubeMap;
        EBO* EBOCubeMap;
        Shader *ptrCubeMapShader;
        Texture *skybox;

        // Square (JoySticks)
        std::vector<VAO*> VAOsSquare{4};
        std::vector<VBO*> VBOsSquare{4};
        std::vector<VBO*> VBOsSquareNormals{4};
        std::vector<EBO*> EBOsSquare{4};
        Shader* ptrSquareShader;
        std::vector<glm::vec3> square_normals;
};

glm::mat4 getPerspectiveProjection(int width, int height, Shader &shader);
glm::mat4 getOrthographicProjection(int width, int height, Shader* shader);


#endif //SHOOTERGAME_LOADER_H
