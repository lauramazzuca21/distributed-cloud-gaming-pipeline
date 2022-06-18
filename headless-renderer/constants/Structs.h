#pragma once
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
 #include "Enums.h"

namespace Constants {

    struct Input {
        float mouse_x;
        float mouse_y;
        Constants::MouseKey m_key_pressed;
        Constants::MouseKey m_key_released;
        Constants::KeyboardKey k_key_pressed;
        Constants::KeyboardKey k_key_released;
    };

    struct GameObjectAttrbutes {
        std::string _UUID; //this should be the id to ref the remote model

        std::string _mesh; //change to mesh_id:[string|enum]
        Constants::ShadingType _shader; 
        Constants::MaterialType _material; 
        
        glm::mat4 _M;
        glm::vec3 _scale = glm::vec3(1.0f);
        glm::vec3 _rotAngles = glm::vec3(0.0f);

    };
    
      struct Material {
        std::string name;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        GLfloat shininess;

        Material() {
            Material(Constants::MaterialType::NO_MATERIAL);
        }

        Material(Constants::MaterialType type) {
            switch(type)
            {
                case Constants::MaterialType::NO_MATERIAL:
                case Constants::MaterialType::RED_PLASTIC:
                    name = "RED PLASTIC";
                    ambient = glm::vec3( 0.1f, 0.0f, 0.0f );
                    diffuse = glm::vec3( 0.6f, 0.1f, 0.1f );
                    specular = glm::vec3( 0.7f, 0.6f, 0.6f );
                    shininess = 32.0f;
                    break;
                case Constants::MaterialType::BRASS:
                    name = "BRASS";
                    ambient = glm::vec3( 0.1f, 0.06f, 0.015f );
                    diffuse = glm::vec3( 0.78f, 0.57f, 0.11f );
                    specular = glm::vec3( 0.99f, 0.91f, 0.81f );
                    shininess = 27.8f;
                    break;
                case Constants::MaterialType::EMERALD:
                    name = "EMERALD";
                    ambient = glm::vec3( 0.0215f, 0.04745f, 0.0215f );
                    diffuse = glm::vec3( 0.07568f, 0.61424f, 0.07568f );
                    specular = glm::vec3( 0.633f, 0.727811f, 0.633f );
                    shininess = 78.8f;
                    break;
                case Constants::MaterialType::SLATE:
                    name = "SLATE";
                    ambient = glm::vec3( 0.02f, 0.02f, 0.02f );
                    diffuse = glm::vec3( 0.1f, 0.1f, 0.1f );
                    specular = glm::vec3( 0.4f, 0.4f, 0.4f );
                    shininess = 32.0f;
                    break;
                case Constants::MaterialType::YELLOW_RUBBER:
                    name = "YELLOW RUBBER";
                    ambient = glm::vec3( 0.1f,0.1f,0.015f );
                    diffuse = glm::vec3( 1.0f,1.0f,0.0f );
                    specular = glm::vec3( 0.7f,0.7f,0.04f );
                    shininess = 10.0f;
                    break;
            }
        }

    };
}
