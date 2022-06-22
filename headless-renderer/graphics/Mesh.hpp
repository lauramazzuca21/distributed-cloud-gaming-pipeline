#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <string>

#include <stdio.h>
#include <glm/glm.hpp>
#include <fstream>

#include "../utils/glLogUtils.hpp"

class Mesh {
	std::vector<glm::vec3> _vertices;
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec2> _texCoords;
	GLuint _vertexArrayObjID;
	GLuint _vertexBufferObjID;
	GLuint _normalBufferObjID;
	GLuint _uvBufferObjID;

    public:
        Mesh(const std::string& filePath);
        void draw();
    private:
        void loadFromFile(const std::string& filePath);
        void computeNormal(std::vector<GLuint> * normalIndices, std::vector<glm::vec3> * tempNormals,
                    std::vector<GLuint> vertexIndices, std::vector<glm::vec3> tempVertices, bool forFace);
        void generateAndLoadBuffers();

};

#endif