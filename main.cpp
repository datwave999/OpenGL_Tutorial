#define STB_IMAGE_IMPLEMENTATION

#include<iostream>
#include<string.h>
#include<cmath>
#include<vector>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"CommonValues.h"
#include"Utils.h"

#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
#include"Camera.h"
#include"Texture.h"
#include"DirectionalLight.h"
#include"PointLight.h"
#include"SpotLight.h"
#include"Material.h" 
#include"Model.h"
#include"Skybox.h"

const float toRad = 3.14159265f / 180.0f;

//Uniforms
GLuint uniProjection = 0, uniModel = 0, uniView = 0,
uniformEyePosition = 0, uniformShininess = 0, uniformSpecularIntensity = 0,
uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture floorTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Skybox skybox;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

Material shinyMaterial;
Material dullMaterial;

Model spaceShip;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
unsigned int frames = 0;
GLfloat prevSecond = 0.0f;

static const char* vertShader = "Shaders/shader.vert";
static const char* fragShader = "Shaders/shader.frag";

void CreateObjects() {
	//Vertices for the triangle
	GLfloat vertices[] = {
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
	};

	GLuint indices[] = {
			0, 1, 3, // Left Face (CCW)
			1, 2, 3, // Right Face (CCW)
			2, 0, 3, // Back Face (CCW)
			0, 2, 1  // Bottom Face (CCW)
	};

	GLfloat floorVertices[] = {
			-10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f,
			10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, 1.0f, 0.0f
	};

	GLuint floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	Utils::clacAverageNormals(indices, sizeof(indices) / sizeof(GLuint), vertices, sizeof(vertices) / sizeof(GLfloat), 8, 5);
	

	//using class to create the triangle
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.emplace_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.emplace_back(obj2);

	Mesh* floor = new Mesh();
	floor->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.emplace_back(floor);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vertShader, fragShader);
	shaderList.push_back(shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.frag", "Shaders/omni_shadow_map.geom");
}

void RenderScene() {
	//Creating the Identity model matrix
	glm::mat4 model(1.0f);

	//Applying translation, rotation, scale to the model matrix
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	//model = glm::rotate(model, float(sin(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));


	brickTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -3.0f));
	//model = glm::rotate(model, float(sin(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	dirtTexture.UseTexture();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	floorTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	//Space Ship model
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0 * sin(glfwGetTime()), -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	spaceShip.RenderModel();
	//
}

void DirectionalShadowMapPass(DirectionalLight* light) {

	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniModel = directionalShadowShader.GetModelLocation();

	glm::mat4 lightTransformMatrix = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&lightTransformMatrix);

	directionalShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return;
}

void OmniShadowMapPass(PointLight* light) {
	omniShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
	
	glViewport(0, 0, 1920, 1080);

	//Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	shaderList[0]->UseShader();

	uniModel = shaderList[0]->GetModelLocation();
	uniProjection = shaderList[0]->GetProjectionLocation();
	uniView = shaderList[0]->GetViewLocation();

	uniformEyePosition = shaderList[0]->GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0]->GetSpecularInstensityLocation();
	uniformShininess = shaderList[0]->GetShininessLocation();

	glUniformMatrix4fv(uniProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	//enabling lighting
	shaderList[0]->SetDirectionalLight(&mainLight);
	shaderList[0]->SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0]->SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	glm::mat4 lightTransformMatrix = mainLight.CalculateLightTransform();
	shaderList[0]->SetDirectionalLightTransform(&lightTransformMatrix);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0]->SetTexture(1);
	shaderList[0]->SetDirectionalShadowMap(2);

	RenderScene();
}

int main() {
	mainWindow = Window(1920, 1080);
	mainWindow.Initialise();

	//Creating the triangle and compiling shaders
	CreateObjects();
	CreateShaders();

	//Camera
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.05f);

	//Textures
	brickTexture = Texture("Textures/Brick.jpg");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/Grass.jpg");
	dirtTexture.LoadTexture();
	floorTexture = Texture("Textures/Grass.jpg");
	floorTexture.LoadTexture();

	// Light: Moderate directional sunlight
	mainLight = DirectionalLight(2048, 2048,
								1.0f, 0.8f, 0.3f,
								0.1f, 0.8f,
								-10.0f, -12.0f, 19.0f);

	// Point Lights: warm and cool fill points
	// common attenuation: constant, linear, exponent
	pointLights[0] = PointLight(1024, 1024,
								0.1f, 100.0f,
								0.0f, 0.0f, 0.1f,
								0.0f, 0.6f,      
								-4.0f, 3.0f, 0.0f,
								1.0f, 0.2f, 0.01f); 

	//pointLightCount++;

	pointLights[1] = PointLight(1024, 1024,
								0.1f, 100.0f,
								0.1f, 0.0f, 0.0f, 
								0.05f, 0.6f,      
								4.0f, 2.0f, 0.0f, 
								1.0f, 0.1f, 0.01f); 

	//pointLightCount++;

	// Spot Lights: flashlight and overhead fill
	spotLights[0] = SpotLight(1024, 1024,
							0.1f, 100.0f,
							1.0f, 1.0f, 1.0f,
							0.0f, 1.0f,       
							0.0f, 0.0f, 0.0f,
							0.0f, -1.0f, 0.0f,
							1.0f, 0.001f, 0.0032f,
							20.0f);

	spotLightCount++;

	spotLights[1] = SpotLight(1024, 1024,
							0.1f, 60.0f,
							1.0f, 0.9f, 0.8f, 
							0.0f, 1.5f,      
							0.0f, 6.0f, -1.5f,
							0.0f, -1.0f, 0.0f,
							0.3f, 0.1f, 0.01f,// Adjusted attenuation
							30.0f);           // Wider edge

	//spotLightCount++;

	//Skybox
	std::vector<std::string> skyboxFaces;

	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	//Material
	shinyMaterial = Material(0.9f, 64);
	dullMaterial = Material(0.4f, 16);

	//Model
	spaceShip = Model();
	spaceShip.LoadModel("Models/Spaceship.obj");

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);


	// MAIN LOOP
	while (!mainWindow.getShouldClose()) {

		frames += 1;

		GLfloat currentTime = (GLfloat)glfwGetTime(); 
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		
		if (currentTime - prevSecond >= 1.0f) {
			std::string title = "OpenGL Graphics Engine - FPS: " + std::to_string(frames);
			mainWindow.ChangeTitle(title.c_str());
			frames = 0;
			prevSecond = currentTime;
		}
		
		//Get and Handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		// Update flashlight (spot light) position/direction from camera before rendering shadow maps
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f; // Lowered further
		glm::vec3 rightDir = glm::normalize(glm::cross(camera.getCameraDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
		lowerLight += rightDir * 0.5f; // Offset to the right
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		DirectionalShadowMapPass(&mainLight);
		for (unsigned int i = 0; i < pointLightCount; i++) {
			OmniShadowMapPass(&pointLights[i]);
		}
		for (unsigned int i = 0; i < spotLightCount; i++) {
			OmniShadowMapPass(&spotLights[i]);
		}
		RenderPass(projection, camera.calculateViewMatrix());

		shaderList[0]->StopShader();

		mainWindow.SwapBuffers();
	}

	return 0;
}