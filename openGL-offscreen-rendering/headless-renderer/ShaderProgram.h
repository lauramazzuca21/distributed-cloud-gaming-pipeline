#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_

#include <string>
#include <map>

#include <glm/glm.hpp>

#include "LogUtils.h"
#include "constants/Maps.h"

class ShaderProgram
{

private:
    GLuint _vertexShader;
    GLuint _fragShader;
    GLuint _shaderProgram;

    Constants::ShadingType _type;    

    std::map<std::string, GLuint> _pointers = {
        {"P_Matrix_pointer", -1},
        {"V_Matrix_pointer", -1},
        {"M_Matrix_pointer", -1},
        {"time_delta_pointer", -1},
        {"frequency_pointer", -1},
        {"height_scale_pointer", -1},
        {"light_position_pointer", -1},
        {"light_color_pointer", -1},
        {"light_power_pointer", -1},
        {"material_diffuse", -1},
        {"material_ambient", -1},
        {"material_specular", -1},
        {"material_shininess", -1}
    };

public:
    ShaderProgram(const Constants::ShadingType& shaderType)
    {
        _type = shaderType;
        load(Constants::shaderFiles.at(_type).first, Constants::shaderFiles.at(_type).second);
    };

    ~ShaderProgram();

    Constants::ShadingType getType() { return _type; }

    void disable();

    void enable();

    void setUniformMatrix4(const std::string& name, glm::mat4 value);

    void setUniformVector4(const std::string& name, glm::vec4 value);

    void setUniformVector3(const std::string& name, glm::vec3 value);

    void setUniformFloat(const std::string& name, float value);

    void setUniformInt(const std::string& name, int value);

private:
    bool load(const std::string& vertName, const std::string& fragName);
    char* readShaderSource(const std::string& shaderFile);
    void loadUniforms();
};

#endif