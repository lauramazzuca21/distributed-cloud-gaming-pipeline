#ifndef _MODEL_H_
#define _MODEL_H_

#include "Errors.h"
#include "Mesh.h"
#include "constants/Enums.h"

class Model {
    Mesh * _mesh;
    Constants::ShadingType _shader;
    std::string _name;
    glm::mat4 _M;

    glm::vec3 _scale = glm::vec3(1.0f);
    glm::vec3 _rotAngles = glm::vec3(15.0f, -15.0f, 0.0f);

public:
    Model(std::string filePath, std::string name, Constants::ShadingType shader = Constants::ShadingType::PASS_THROUGH);

    void Draw(GLuint * MatModel);
    Constants::ShadingType GetShader();
    const std::string& GetName() const;
    void SetShader(Constants::ShadingType shader);
};

#endif