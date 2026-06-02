#pragma once

#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>

#include<GL/glew.h>

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"CommonValues.h"
#include"DirectionalLight.h"
#include"PointLight.h"
#include"SpotLight.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation, const char* geometryLocation = nullptr);

	void Validate();

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetEyePositionLocation();
	GLuint GetSpecularInstensityLocation();
	GLuint GetShininessLocation();
	GLuint GetOmniLightPosLocation();
	GLuint GetFarPlaneLocation();

	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight* sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

	void UseShader();
	void ClearShader();
	static void StopShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniProjection, uniModel, uniView;
	GLuint uniformEyePosition;
	GLuint uniformSpecularInstensity, uniformShininess;
	GLuint uniformTexture;
	GLuint uniformDirectionalLightTransform, uniformDirectionalShadowMap;
	GLuint uniformOmniLightPos, uniformFarPlane;

	GLuint uniformlightMatrices[6];


	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	struct {
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];


	void CompileShader(const char* vertexCode, const char* fragmentCode, const char* geometryCode = nullptr);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	void CompileProgram();
};

