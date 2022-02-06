#include "Mesh.h"
#include <cstring>

Mesh::Mesh(const std::string& filePath) {
    loadFromFile(filePath);
    generateAndLoadBuffers();
}

void Mesh::draw() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(_vertexArrayObjID);
    //draws only the trinagle's lines
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    gl::log::errors::assertOpenGLError("glDrawArrays");

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}

void Mesh::computeNormal(std::vector<GLuint> * normalIndices, std::vector<glm::vec3> * tempNormals,
                    std::vector<GLuint> vertexIndices, std::vector<glm::vec3> tempVertices, bool forFace)
{
	(*tempNormals).resize(vertexIndices.size() / 3, glm::vec3(0.0, 0.0, 0.0));
	// normal of each face saved 1 time PER FACE!
	for (int i = 0; i < vertexIndices.size(); i += 3)
	{
		GLushort ia = vertexIndices[i];
		GLushort ib = vertexIndices[i + 1];
		GLushort ic = vertexIndices[i + 2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(tempVertices[ib]) - glm::vec3(tempVertices[ia]),
			glm::vec3(tempVertices[ic]) - glm::vec3(tempVertices[ia])));
		
		if(forFace)
		{
			//FACE Normals
			(*tempNormals)[i / 3] = normal;
			//Put an index to the normal for all 3 vertex of the face
			(*normalIndices).push_back(i / 3);
			(*normalIndices).push_back(i / 3);
			(*normalIndices).push_back(i / 3);
		}
		else
		{
			//VERTEX Normals
			(*tempNormals)[ia] += normal;
			(*tempNormals)[ib] += normal;
			(*tempNormals)[ic] += normal;
			//Put an index to the normal for all 3 vertex of the face
			(*normalIndices).push_back(ia);
			(*normalIndices).push_back(ib);
			(*normalIndices).push_back(ic);
		}

	}
}

void Mesh::loadFromFile(const std::string& filePath)
{
    FILE* file;
    try {
	file = fopen(filePath.c_str(), "r");
    } catch (const std::exception& e) {
        std::stringstream s;
        s << "Failed to open obj file! --> " << filePath;
        throw std::runtime_error(s.str());
	}
	// tmp data structures
	std::vector<GLuint> vertexIndices, normalIndices, uvIndices;
	std::vector<glm::vec3> tmpVertices, tmpNormals;
	std::vector<glm::vec2> tmpUvs;

	char lineHeader[128];
    try {
	while (fscanf(file, "%s", lineHeader) != EOF) {
		if (strcmp (lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, " %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmpVertices.push_back(vertex);
		}
		else if (strcmp (lineHeader,"vn") == 0) {
			glm::vec3 normal;
			fscanf(file, " %f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmpNormals.push_back(normal);
		}
		else if (strcmp (lineHeader,"vt") == 0) {
			glm::vec2 uv;
			fscanf(file, " %f %f\n", &uv.x, &uv.y);
			uv.y = 1 - uv.y;
			tmpUvs.push_back(uv);
		}
		else if (strcmp (lineHeader,"f") == 0) {
			GLuint v_a, v_b, v_c; // index in position array
			GLuint n_a, n_b, n_c; // index in normal array
			GLuint t_a, t_b, t_c; // index in UV array

			fscanf(file, "%s", lineHeader);
			if (strstr(lineHeader, "//")) { // case: v//n v//n v//n
				sscanf(lineHeader, "%d//%d", &v_a, &n_a);
				fscanf(file, "%d//%d %d//%d\n", &v_b, &n_b, &v_c, &n_c);
				n_a--, n_b--, n_c--;
				normalIndices.push_back(n_a); normalIndices.push_back(n_b); normalIndices.push_back(n_c);
			}
			else if (strstr(lineHeader, "/")) {// case: v/t/n v/t/n v/t/n
				sscanf(lineHeader, "%d/%d/%d", &v_a, &t_a, &n_a);
				fscanf(file, "%d/%d/%d %d/%d/%d\n", &v_b, &t_b, &n_b, &v_c, &t_c, &n_c);
				n_a--, n_b--, n_c--;
				t_a--, t_b--, t_c--;
				normalIndices.push_back(n_a); normalIndices.push_back(n_b); normalIndices.push_back(n_c);
				uvIndices.push_back(t_a); uvIndices.push_back(t_b); uvIndices.push_back(t_c);
			}
			else {// case: v v v
				sscanf(lineHeader, "%d", &v_a);
				fscanf(file, "%d %d\n", &v_b, &v_c);
			}
			v_a--; v_b--; v_c--;
			vertexIndices.push_back(v_a); vertexIndices.push_back(v_b); vertexIndices.push_back(v_c);
		}
	}
	fclose(file);
    } catch (const std::exception& e) {
        std::stringstream s;
        s << "Failed to correctly read the mesh file! --> " << filePath;
        throw std::runtime_error(s.str());

    }
	bool faces = false;
	// If normals and uvs are not loaded, we calculate normals for face
	if (tmpNormals.size() == 0) {
		computeNormal(&normalIndices, &tmpNormals, vertexIndices, tmpVertices, faces);
	}

	// We prepare the data for glDrawArrays calls, this is a simple but non optimal way of storing mesh data.
	// However, you could optimize the mesh data using a index array for both vertex positions, 
	// normals and textures and later use glDrawElements
	// Now following the index arrays, we build the final arrays that will contain the data for glDrawArray...
	for (int i = 0; i < vertexIndices.size(); i++) {

		_vertices.push_back(tmpVertices[vertexIndices[i]]);
		_normals.push_back(faces ? tmpNormals[normalIndices[i]] : glm::normalize(tmpNormals[normalIndices[i]]));
	}
}

void Mesh::generateAndLoadBuffers()
{
    // Genero 1 Vertex Array Object
    glGenVertexArrays(1, &_vertexArrayObjID);
    glBindVertexArray(_vertexArrayObjID);

    // Genero 1 Vertex Buffer Object per i vertici
    glGenBuffers(1, &_vertexBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER,_vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER,_vertices.size() * sizeof(glm::vec3), _vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					// attribute index in the shader
		3,                  // size
		GL_FLOAT,           // type
		false,              // normalized 
		0,					// stride
		(void*)0            // array buffer offset
	);

    // Genero 1 Buffer Object per le normali
    glGenBuffers(1, &_normalBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(glm::vec3), _normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// attribute index in the shader
		3,                  // size
		GL_FLOAT,           // type
		false,              // normalized 
		0,					// stride
		(void*)0            // array buffer offset
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}