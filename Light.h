#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include<vector>

#include"ShadowMap.h"

class Light {
public:

	Light();
	Light(GLuint shadowWidth, GLuint shadowHeight,
			GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat aIntensity, GLfloat dIntensity);

	ShadowMap* GetShadowMap() { return depthMap; }

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* depthMap;
};

