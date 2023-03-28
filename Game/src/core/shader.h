#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"
#include "glm/glm.hpp"

class Shader
{
public:
    unsigned int Id;
    
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader(const Shader& other) = default;
    virtual ~Shader();

    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4f(const std::string& name, const glm::mat4& matrix);
    void setVec3f(const std::string& name, glm::vec3 vector);
};