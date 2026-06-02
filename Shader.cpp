#include "Shader.h"

Shader::Shader() {
	shaderID = 0;
	uniModel = 0;
	uniProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
	CompileShader(vertexCode, fragmentCode, "");
}

std::string Shader::ReadFile(const char* fileLocation) {
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to real %s! File does not exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation, const char* geometryLocation) {
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	if (geometryLocation != nullptr) {
		std::string geometryString = ReadFile(geometryLocation);
		const char* geometryCode = geometryString.c_str();
		CompileShader(vertexCode, fragmentCode, geometryCode);
	}
	else CompileShader(vertexCode, fragmentCode);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode, const char* geometryCode) {
	//creating a shader program where we compile the vertex and fragment shaders
	shaderID = glCreateProgram();

	//error handleing
	if (!shaderID) {
		printf("Error creating shader program!\n");
		return;
	}

	//compiling all created shaders into the shader program
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	if(geometryCode != nullptr) AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

GLuint Shader::GetProjectionLocation() { return uniProjection; }
GLuint Shader::GetModelLocation() { return uniModel; }
GLuint Shader::GetViewLocation() { return uniView; }

GLuint Shader::GetAmbientIntensityLocation() { return uniformDirectionalLight.uniformAmbientIntensity; }
GLuint Shader::GetAmbientColorLocation() { return uniformDirectionalLight.uniformColour; }
GLuint Shader::GetDiffuseIntensityLocation() { return uniformDirectionalLight.uniformDiffuseIntensity; }
GLuint Shader::GetDirectionLocation() { return uniformDirectionalLight.uniformDirection; }
GLuint Shader::GetEyePositionLocation() { return uniformEyePosition; }
GLuint Shader::GetSpecularInstensityLocation() { return uniformSpecularInstensity; }
GLuint Shader::GetShininessLocation() { return uniformShininess; }
GLuint Shader::GetOmniLightPosLocation() { return uniformOmniLightPos; }
GLuint Shader::GetFarPlaneLocation() { return uniformFarPlane; }


void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		pLight[i].UseLight(uniformPointLight[i].uniformColour,
			uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition, 
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);

		pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, pLight[i].GetFarPlane());
	}
}

void Shader::SetSpotLights(SpotLight* sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		sLight[i].UseLight(uniformSpotLight[i].uniformColour,
		uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition,
		uniformSpotLight[i].uniformDirection,
		uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
		uniformSpotLight[i].uniformEdge);

		sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, sLight[i].GetFarPlane());
	}

}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (int i = 0; i < 6; i++) {
		glUniformMatrix4fv(uniformlightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}


void Shader::UseShader() {
	if (shaderID == 0) return;
	glUseProgram(shaderID);
}

void Shader::ClearShader() {
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniModel = 0;
	uniProjection = 0;
}


void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	//Creating an empty shader
	GLuint theShader = glCreateShader(shaderType);
	//storing the shader code in an array
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	//storing the shader code's length
	GLint codeLength[1];
	codeLength[0] = static_cast<GLint>(strlen(shaderCode));

	//compiling the shader after putting in the code in the empty shader...
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	//For getting error if happens in compiling the shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	//Attaching shader to the final program
	glAttachShader(theProgram, theShader);

	return;
}

void Shader::Validate()
{

	//For getting error if happens in compiling shaders
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	//Validating shader program and error handleing
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error Validating Program: '%s'\n", eLog);
		return;
	}

}

void Shader::CompileProgram()
{
	//For getting error if happens in compiling shaders
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	//Linking the final compiled program, and error handleing
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error Linking Program: '%s'\n", eLog);
		return;
	}



	uniModel = glGetUniformLocation(shaderID, "model");
	uniProjection = glGetUniformLocation(shaderID, "projection");
	uniView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformSpecularInstensity = glGetUniformLocation(shaderID, "material.specularInstensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (int i = 0; i < static_cast<int>(MAX_POINT_LIGHTS); i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (int i = 0; i < static_cast<int>(MAX_SPOT_LIGHTS); i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (int i = 0; i < 6; i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
		uniformlightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
	}


	for (int i = 0; i < 6; i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
		uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderID, locBuff);
	}

	return;
}

void Shader::StopShader() {
	glUseProgram(0);
}

Shader::~Shader() {
	ClearShader();
}