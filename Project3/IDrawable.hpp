// ½Ó¿ÚÀà
#pragma once

#include <cgProgram.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class IDrawable
{
public:
	virtual void Render() = 0;
	virtual void SetModelMatrix(glm::mat4 model) = 0;
	virtual void SetViewMatrix(glm::mat4 view) = 0;
	virtual void SetProjectionMatrix(glm::mat4 projection) = 0;
	virtual void SetProgram(cgProgram* program) = 0;
	virtual void SetTexture(unsigned int textureID) = 0;
	virtual cgProgram* GetProgram() = 0;
};