#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.inl>

bool ShaderProgram::load(const std::string& vertFileName, const std::string& fragFileName) {
    GLenum ErrorCheckValue = glGetError();

    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Creiamo gli eseguibili degli shader
    //Leggiamo il codice del Vertex Shader
    GLchar* vertexShader = readShaderSource(vertFileName);
    //Visualizzo sulla console il CODICE VERTEX SHADER
    //cout << vertexShader;

    //Generiamo un identificativo per il vertex shader
    _vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Associamo all'identificativo il codice del vertex shader
    glShaderSource(_vertexShader, 1, (const char**)&vertexShader, NULL);
    //Compiliamo il Vertex SHader
    glCompileShader(_vertexShader); 
    


    //Leggiamo il codice del Fragment Shader
    const GLchar* fragmentShader = readShaderSource(fragFileName);
    //Visualizzo sulla console il CODICE FRAGMENT SHADER
    //cout << fragmentShader;

    //Generiamo un identificativo per il FRAGMENT shader
    _fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragShader, 1, (const char**)&fragmentShader, NULL);
    //Compiliamo il FRAGMENT SHader
    glCompileShader(_fragShader);

    int success;
    char infoLog[512];
    glGetShaderiv(_fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(_fragShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        printf("%s\n", infoLog);
    }

    gl::log::errors::assertOpenGLError("glGetShaderiv");
    //Creiamo un identificativo di un eseguibile e gli colleghiamo i due shader compilati
    _shaderProgram = glCreateProgram();

    glAttachShader(_shaderProgram, _vertexShader);
    glAttachShader(_shaderProgram, _fragShader);
    glLinkProgram(_shaderProgram);
    
    if (_shaderProgram < 0)
        return false;
    
    loadUniforms();
    
    return true;
}

char* ShaderProgram::readShaderSource(const std::string& shaderFile)
{
    FILE* fp = fopen(shaderFile.c_str(), "rb");

    if (fp == NULL) { return NULL; }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);

    buf[size] = '\0';
    fclose(fp);

    return buf;
}

void ShaderProgram::enable() {
    glUseProgram(_shaderProgram);
}

void ShaderProgram::disable() {
    glUseProgram(0);
}

void ShaderProgram::loadUniforms() {
    //loading all uniforms possible, -1 will be returned if the Uniform doesn't exist in the shader program
    _pointers["P_Matrix_pointer"] = glGetUniformLocation(_shaderProgram, "P");
    _pointers["V_Matrix_pointer"] = glGetUniformLocation(_shaderProgram, "V");
    _pointers["M_Matrix_pointer"] = glGetUniformLocation(_shaderProgram, "M");
    _pointers["time_delta_pointer"] = glGetUniformLocation(_shaderProgram, "time");
    _pointers["height_scale_pointer"] = glGetUniformLocation(_shaderProgram, "h");
    _pointers["frequency_pointer"] = glGetUniformLocation(_shaderProgram, "a");

    _pointers["light_color_pointer"] = glGetUniformLocation(_shaderProgram, "light.color");
    _pointers["light_position_pointer"] = glGetUniformLocation(_shaderProgram, "light.position");
    _pointers["light_power_pointer"] = glGetUniformLocation(_shaderProgram, "light.power");
    _pointers["material_ambient"] = glGetUniformLocation(_shaderProgram, "material.ambient");
    _pointers["material_diffuse"] = glGetUniformLocation(_shaderProgram, "material.diffuse");
    _pointers["material_shininess"] = glGetUniformLocation(_shaderProgram, "material.shininess");
    _pointers["material_specular"] = glGetUniformLocation(_shaderProgram, "material.specular");
}

void ShaderProgram::setUniformMatrix4(const std::string& name, glm::mat4 value){
    glUniformMatrix4fv(_pointers[name], 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniformVector4(const std::string& name, glm::vec4 value){
    glUniform4fv(_pointers[name], 1, glm::value_ptr(value));
}

void ShaderProgram::setUniformVector3(const std::string& name, glm::vec3 value){
    glUniform3fv(_pointers[name], 1, glm::value_ptr(value));
}

void ShaderProgram::setUniformFloat(const std::string& name, float value){
    glUniform1f(_pointers[name], value);
}

void ShaderProgram::setUniformInt(const std::string& name, int value){
    glUniform1i(_pointers[name], value);
}
