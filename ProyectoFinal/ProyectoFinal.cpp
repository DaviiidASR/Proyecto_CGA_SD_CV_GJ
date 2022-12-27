// ProyectoFinal.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"
#include "Headers/AnimationUtils.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow* window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;

//Variables para el manejo de la luz ambiental
glm::vec3 ambientLight, diffuseLight, specularLight, directionLight;

//Camaras
std::shared_ptr<FirstPersonCamera> cameraFP(new FirstPersonCamera());
std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromPlayer = 7.0f;
bool cameraSwitch = false;
bool cameraState = false;
//std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere skyboxSphere(20, 20);
//Box boxCesped;
// Models complex instances
Model modelFinnAnim;

Model modelHeliChasis;
Model modelHeliHeli;
// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;

Terrain terrain(-1, -1, 200, 10, "../Textures/heightmap.png");
GLuint skyboxTextureID;

GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureCespedID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;



// Model matrix definitions
glm::mat4 modelMatrixFinn = glm::mat4(1.0f);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);

int modelSelected = 0;

// Lamps positions
std::vector<glm::vec3> lamp1Position = { glm::vec3(-7.03, 0, -19.14), 
	glm::vec3(24.41, 0, -34.57), 
	glm::vec3(-10.15, 0, -54.10) , 
	glm::vec3(17.96, 0, -51.08) }; 
std::vector<float> lamp1Orientation = { -17.0, -82.67, 23.70 , -40.0 };

std::vector<glm::vec3> lamp2Position = { glm::vec3(-36.52, 0, -23.24),
		glm::vec3(-52.73, 0, -3.90), 
	glm::vec3(25.0f, 0.0f, -53.51f) };
std::vector<float> lamp2Orientation = { 21.37 + 90, -65.0 + 90, -42.2 + 90 };

double deltaTime;
double currTime, lastTime;

bool enableCountSelected = true;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	shaderTerrain.initialize("../Shaders/terrain.vs", "../Shaders/terrain.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	//boxCesped.init();
	//boxCesped.setShader(&shaderMulLighting);

	//Terrain
	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	/*Finn*/
	modelFinnAnim.loadModel("../models/Finn/finn_9.fbx");
	modelFinnAnim.setShader(&shaderMulLighting);

	// Helicopter
	modelHeliChasis.loadModel("../models/Helicopter/Mi_24_chasis.obj");
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.loadModel("../models/Helicopter/Mi_24_heli.obj");
	modelHeliHeli.setShader(&shaderMulLighting);
	//Lamp models
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.loadModel("../models/Street_Light/LampPost.obj");
	modelLampPost2.setShader(&shaderMulLighting);

	//camera->setPosition(glm::vec3(0.0, 3.0, 4.0));

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP* bitmap;
	unsigned char* data;

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP* bitmap = skyboxTexture.loadImage(true);
		unsigned char* data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);

		// Definiendo la textura a utilizar
		Texture textureCesped("../Textures/cesped.jpg");
		// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
		bitmap = textureCesped.loadImage();
		// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
		data = textureCesped.convertToData(bitmap, imageWidth,
			imageHeight);
		// Creando la textura con id 1
		glGenTextures(1, &textureCespedID);
		// Enlazar esa textura a una tipo de textura de 2D.
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Verifica si se pudo abrir la textura
		if (data) {
			// Transferis los datos de la imagen a memoria
			// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
			// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
			// a los datos
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
			// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		// Libera la memoria de la textura
		textureCesped.freeImage(bitmap);
	}
	// Definiendo la textura a utilizar
	Texture textureTerrainR("../Textures/mud.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainR.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainR.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainG("../Textures/grassFlowers.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainG.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainG.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainGID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainB("../Textures/path.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainB.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainB.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBlendMap("../Textures/blendMap.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap); 
}

	void destroy() {
		glfwDestroyWindow(window);
		glfwTerminate();
		// --------- IMPORTANTE ----------
		// Eliminar los shader y buffers creados.

		// Shaders Delete
		shader.destroy();
		shaderMulLighting.destroy();
		shaderSkybox.destroy();
		shaderTerrain.destroy();

		// Basic objects Delete
		skyboxSphere.destroy(); 
		//boxCesped.destroy();

		// Terrains objects Delete
		terrain.destroy();
		// Custom objects Delete
		
		modelHeliChasis.destroy();
		modelHeliHeli.destroy();
		modelLamp1.destroy();
		modelLamp2.destroy();
		modelLampPost2.destroy();

		// Custom objects animate
		/*Finn*/
		modelFinnAnim.destroy();

		// Textures Delete
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &textureCespedID);
		glDeleteTextures(1, &textureTerrainBackgroundID);
		glDeleteTextures(1, &textureTerrainRID);
		glDeleteTextures(1, &textureTerrainGID);
		glDeleteTextures(1, &textureTerrainBID);
		glDeleteTextures(1, &textureTerrainBlendMapID);

		// Cube Maps Delete
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glDeleteTextures(1, &skyboxTextureID);
	}

	void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
		screenWidth = widthRes;
		screenHeight = heightRes;
		glViewport(0, 0, widthRes, heightRes);
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action,
		int mode) {
		if (action == GLFW_PRESS) {
			switch (key) {
			case GLFW_KEY_ESCAPE:
				exitApp = true;
				break;
			}
		}
	}

	void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
		offsetX = xpos - lastMousePosX;
		offsetY = ypos - lastMousePosY;
		lastMousePosX = xpos;
		lastMousePosY = ypos;
	}

	void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
		if (state == GLFW_PRESS) {
			switch (button) {
			case GLFW_MOUSE_BUTTON_RIGHT:
				std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
				break;
			case GLFW_MOUSE_BUTTON_LEFT:
				std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
				break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
				std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
				break;
			}
		}
	}

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		distanceFromPlayer -= yoffset;
		camera->setDistanceFromTarget(distanceFromPlayer);
	}

	bool processInput(bool continueApplication) {
		if (exitApp || glfwWindowShouldClose(window) != 0) {
			return false;
		}

		if (cameraSwitch)
		{
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				cameraFP->moveFrontCamera(true, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				cameraFP->moveFrontCamera(false, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				cameraFP->moveRightCamera(false, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				cameraFP->moveRightCamera(true, deltaTime);
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
		}
		else
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
			{
				camera->mouseMoveCamera(offsetX, 0, deltaTime);
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
			{
				camera->mouseMoveCamera(0, offsetY, deltaTime);
			}
		}

		offsetX = 0;
		offsetY = 0;

		/*
		* Cambiar de camaras
		*/
		if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) && !cameraState)
		{
			cameraSwitch = !cameraSwitch;
			cameraState = !cameraState;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
		{
			cameraState = !cameraState;
		}
		// Seleccionar modelo
		if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
			enableCountSelected = false;
			modelSelected++;
			std::cout << "modelSelected = " << modelSelected << std::endl;
			if (modelSelected > 3)
				modelSelected = 0;
		}
		else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
			enableCountSelected = true;

		/* Controles Finn*/
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		modelMatrixFinn = glm::rotate(modelMatrixFinn, 0.02f, glm::vec3(0.0f, 1.0f, 0.0f));
		modelFinnAnim.setAnimationIndex(1);
	}
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		modelMatrixFinn = glm::rotate(modelMatrixFinn, -0.02f, glm::vec3(0.0f, 1.0f, 0.0f));
		modelFinnAnim.setAnimationIndex(1);
	}
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		modelMatrixFinn = glm::translate(modelMatrixFinn, glm::vec3(0.0f, 0.0f, 0.03f));
		modelFinnAnim.setAnimationIndex(1);
	}
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		modelMatrixFinn = glm::translate(modelMatrixFinn, glm::vec3(0.0f, 0.0f, -0.03f));
		modelFinnAnim.setAnimationIndex(1);
	}

		glfwPollEvents();
		return continueApplication;
	}
	
	void applicationLoop() {
		bool psi = true;

		//
		glm::mat4 view;
		glm::vec3 target;
		glm::vec3 axisTarget;
		float angleTarget;

		modelMatrixFinn = glm::translate(modelMatrixFinn, glm::vec3(3.0f, 3.0f, 0.0f));
		modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));

		while (psi) {
			currTime = TimeManager::Instance().GetTime();
			if (currTime - lastTime < 0.016666667) {
				glfwPollEvents();
				continue;
			}
			lastTime = currTime;
			TimeManager::Instance().CalculateFrameRate(true);
			deltaTime = TimeManager::Instance().DeltaTime;
			psi = processInput(true);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
			glm::mat4 view = camera->getViewMatrix();

			if (modelSelected == 3)
			{
				axisTarget = glm::axis(glm::quat_cast(modelMatrixFinn));
				angleTarget = glm::angle(glm::quat_cast(modelMatrixFinn));
				target = glm::vec3(modelMatrixFinn[3]) + glm::vec3(0.0f, 0.5f, 0.0f);
			}

			if (std::isnan(angleTarget))
			{
				angleTarget = 0.0f;
			}

			if (axisTarget.y == 0)
			{
				angleTarget = -angleTarget;
			}


			if (cameraSwitch)
			{
				cameraFP->setPosition(glm::vec3(modelMatrixFinn[3]) + glm::vec3(0.0f, 3.0f, 0.0f));
				view = cameraFP->getViewMatrix();
			}
			else
			{
				camera->setCameraTarget(target);
				camera->setAngleTarget(angleTarget);
				camera->updateCamera();
				view = camera->getViewMatrix();
			}

			// Settea la matriz de vista y projection al shader con solo color
			shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
			shader.setMatrix4("view", 1, false, glm::value_ptr(view));

			// Settea la matriz de vista y projection al shader con skybox
			shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
			shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
			// Settea la matriz de vista y projection al shader con multiples luces
			shaderMulLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
			shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
			// Settea la matriz de vista y projection al shader con multiples luces
			shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
			shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));

			ambientLight = glm::vec3(0.5f, 0.5f, 0.5f);
			diffuseLight = glm::vec3(0.3f, 0.3f, 0.3f); 
			specularLight = glm::vec3(0.4f, 0.4f, 0.4f); 
			directionLight = glm::vec3(-1.0f, 0.0f, 0.0f);
			/*******************************************
			 * Propiedades Luz direccional
			 *******************************************/
			if (!cameraSwitch) {
				shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));

			}
			else {
				shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(cameraFP->getPosition()));

			}
			//shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
			shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(ambientLight));
			shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(diffuseLight));
			shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(specularLight));
			shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

			/*******************************************
			 * Propiedades Luz direccional Terrain
			 *******************************************/
			if (!cameraSwitch) {
				shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));

			}
			else {
				shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(cameraFP->getPosition()));

			}
			//shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
			shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(ambientLight));
			shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(diffuseLight));
			shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(specularLight));
			shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(directionLight));

			/*******************************************
			 * Propiedades SpotLights
			 *******************************************/
			shaderMulLighting.setInt("spotLightCount", 1);
			shaderTerrain.setInt("spotLightCount", 1);
			glm::vec3 spotPosition = glm::vec3(modelMatrixHeli[3]);

			shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.03)));
			shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
			shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0.0f, -1.0f, 0.0f)));
			shaderMulLighting.setFloat("spotLights[0].constant", 1.0f);
			shaderMulLighting.setFloat("spotLights[0].linear", 0.02f);
			shaderMulLighting.setFloat("spotLights[0].quadratic", 0.01f);
			shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
			shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

			shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.03)));
			shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
			shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0.0f, -1.0f, 0.0f)));
			shaderTerrain.setFloat("spotLights[0].constant", 1.0f);
			shaderTerrain.setFloat("spotLights[0].linear", 0.02f);
			shaderTerrain.setFloat("spotLights[0].quadratic", 0.01f);
			shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
			shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

			/*******************************************
			 * Propiedades PointLights
			 *******************************************/
			shaderMulLighting.setInt("pointLightCount", lamp1Position.size() + lamp2Position.size());
			shaderTerrain.setInt("pointLightCount", lamp1Position.size() + lamp2Position.size());
			for (int i = 0; i < lamp1Position.size(); i++) {
				glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
				matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp1Position[i]);
				matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
				matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.5, 0.5, 0.5));
				matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0, 10.3585, 0));
				glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
				shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
			}

			for (int i = 0; i < lamp2Position.size(); i++)
			{
				glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
				matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
				matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0f, 1.0f, 1.0f));
				matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.7856f, 4.975f, 0.0f));
				glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i + lamp1Position.size()) + "].light.ambient",
					glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i + lamp1Position.size()) + "].light.diffuse",
					glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i + lamp1Position.size()) + "].light.specular",
					glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
				shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i + lamp1Position.size()) + "].position",
					glm::value_ptr(lampPosition));
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i + lamp1Position.size()) + "].constant", 1.0f);
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i + lamp1Position.size()) + "].linear", 0.02f);
				shaderMulLighting.setFloat("pointLights[" + std::to_string(i + lamp1Position.size()) + "].quadratic", 0.01f);
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i + lamp1Position.size()) + "].light.ambient",
					glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i + lamp1Position.size()) + "].light.diffuse",
					glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i + lamp1Position.size()) + "].light.specular",
					glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
				shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i + lamp1Position.size()) + "].position",
					glm::value_ptr(lampPosition));
				shaderTerrain.setFloat("pointLights[" + std::to_string(i + lamp1Position.size()) + "].constant", 1.0f);
				shaderTerrain.setFloat("pointLights[" + std::to_string(i + lamp1Position.size()) + "].linear", 0.02f);
				shaderTerrain.setFloat("pointLights[" + std::to_string(i + lamp1Position.size()) + "].quadratic", 0.01f);
			}

			/*******************************************
			 * Terrain Cesped
			 *******************************************/
			glm::mat4 modelCesped = glm::mat4(1.0);
			modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
			modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
			// Se activa la textura del background
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
			shaderTerrain.setInt("backgroundTexture", 0);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
			shaderTerrain.setInt("textureB", 2);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
			shaderTerrain.setInt("textureR", 3);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
			glActiveTexture(GL_TEXTURE5);
			shaderTerrain.setInt("textureG", 4);
			glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
			shaderTerrain.setInt("textureBlendMap", 5);
			shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
			terrain.render();
			shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
			glBindTexture(GL_TEXTURE_2D, 0);

			/*******************************************
			 * Custom objects obj
			 *******************************************/

			 // Helicopter
			glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
			modelHeliChasis.render(modelMatrixHeliChasis);

			glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
			//modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
			//modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY, glm::vec3(0, 1, 0));
			//modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
			modelHeliHeli.render(modelMatrixHeliHeli);

			// Render the lamps
			for (int i = 0; i < lamp1Position.size(); i++) {
				lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x, lamp1Position[i].z);
				modelLamp1.setPosition(lamp1Position[i]);
				modelLamp1.setScale(glm::vec3(0.5, 0.5, 0.5));
				modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
				modelLamp1.render();
			}

			for (int i = 0; i < lamp2Position.size(); i++)
			{
				lamp2Position[i].y = terrain.getHeightTerrain(lamp2Position[i].x, lamp2Position[i].z);
				modelLamp2.setPosition(lamp2Position[i]);
				modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
				modelLamp2.render();
				modelLampPost2.setPosition(lamp2Position[i]);
				modelLampPost2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
				modelLampPost2.render();
			}

		  /*******************************************
		  * Custom Anim objects obj
		  *******************************************/
			modelMatrixFinn[3][1] = terrain.getHeightTerrain(modelMatrixFinn[3][0], modelMatrixFinn[3][2]);
			glm::mat4 modelMatrixFinnBody = glm::mat4(modelMatrixFinn);
			modelMatrixFinnBody = glm::translate(modelMatrixFinnBody, glm::vec3(0.0f, 0.0f, 0.0f));
			modelMatrixFinnBody = glm::scale(modelMatrixFinnBody, glm::vec3(1.0f, 1.0f, 1.0f) * 0.01f);
			modelFinnAnim.render(modelMatrixFinnBody);
			modelFinnAnim.setAnimationIndex(0);

		   /*******************************************
		   * Skybox
		   *******************************************/
			GLint oldCullFaceMode;
			GLint oldDepthFuncMode;
			// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
			glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
			glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
			shaderSkybox.setFloat("skybox", 0);
			glCullFace(GL_FRONT);
			glDepthFunc(GL_LEQUAL);
			glActiveTexture(GL_TEXTURE0);
			skyboxSphere.render();
			glCullFace(oldCullFaceMode);
			glDepthFunc(oldDepthFuncMode);
		
		glfwSwapBuffers(window);


	}
}
int main(int argc, char** argv)
{
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}