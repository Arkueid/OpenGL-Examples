#pragma once

#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Drawable
{
public:
	virtual void Render() = 0;
	virtual void SetModelMatrix(glm::mat4 model) = 0;
	virtual void SetViewMatrix(glm::mat4 view) = 0;
	virtual void SetProjectionMatrix(glm::mat4 projection) = 0;
	virtual void SetProgram(unsigned int programID) = 0;
	virtual void SetTexture(unsigned int textureID) = 0;
};