#include "Light.h"
#include <iostream>

Light::Light() {
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
	depthMap = nullptr;
}

Light::Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) {

	depthMap = new ShadowMap();
	depthMap->Init(shadowWidth, shadowHeight);

	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

Light::~Light() {

}
