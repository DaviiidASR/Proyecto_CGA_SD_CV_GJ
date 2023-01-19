// ProyectoFinal.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>
#include <iomanip>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

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

// Include Colision headers functions
#include "Headers/Colisiones.h"

//Include e text rendering
#include "Headers/FontTypeRendering.h"

// OpenAL include
#include <AL/alut.h>

#include "Headers/ShadowBox.h"

///
#include "Player.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

GLFWwindow* window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para las particulas
Shader shaderParticulasFountain;
//shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profundidad
Shader shaderDepth;
Shader shaderTextura;
ShadowBox* shadowBox;

//Variables para el manejo de la luz ambiental
glm::vec3 ambientLight, diffuseLight, specularLight, directionLight;

//Camaras
std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromPlayer = 7.0f;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;
Box boxIntro;
// Models complex instances
Model modelPlayerAnim;
Model modelEstadio;

Player player;
//Obstaculos
std::map<std::string, std::tuple<glm::mat4, bool, Model*>> obstaculos;
Model modelObstaculo1;
Model modelObstaculo2;
Model modelObstaculo3;
Model modelObstaculo4;
Model modelObstaculo5;
Model modelObstaculo6;
Model modelObstaculo7;
Model modelObstaculo8;
Model modelObstaculo9;
Model modelObstaculo10;

//Variables obstaculos
float offsetVelObs = 5.0f;
float intervaloObstaculos = 10.0f;
int sizeObstaculos = 0;
std::vector<std::string> namesObs;

//Variables de control para el movimiento de la pista
int step = 2;
float stepVel = 0.1f;
bool isRight = false;
bool isPress = false;

//pista
Box pista;
Box pista2;
Box pista3;
Box curva;

//Game Controller
bool isStart = true;
bool isRunning = false;
bool esInicio = true, esPista1 = false;

Model modelHeliChasis;
Model modelHeliHeli;
//Grass
Model modelGrass;
// Fountain
Model modelFountain;
// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;

Terrain terrain(-1, -1, 200, 10, "../Textures/heightmap2.png");
GLuint skyboxTextureID;

FontTypeRendering::FontTypeRendering* textRender;
/********************************DECLARACION DE TEXTURAS************************/
GLuint texturaMenuID, texturaSelFinnID, texturaSelJakeID, texturaSelLegoID, texturaActivaID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, 
	textureTerrainBlendMapID, texturePistaBlendMapID, textureCurvaBlendMapID;
GLuint textureCespedID, textureParticleFountainID;

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
glm::mat4 modelMatrixPlayer = glm::mat4(1.0f);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);
glm::mat4 modelMatrixEstadio = glm::mat4(1.0f);

glm::mat4 modelMatrixCurva = glm::mat4(1.0f);
glm::mat4 modelMatrixPista = glm::mat4(1.0f);

int animationIndex = 0;
//int modelSelected = 3;

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

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"particulasAgua", glm::vec3(0.0f)}
};

double deltaTime;
double currTime, lastTime;
double velocity = 0.1;

bool enableCountSelected = true;

//Variables para el salto
bool isJump = false;
float gravity = 3.1f;
double tmv = 0.0;
double startTimeJump = 0.0;

//Definicion de variables para el sistema de partuculas
GLuint initVel, startTime;
GLuint VAOParticulas;
GLuint numParticulas = 8000;
double currTimeParticulasFountain, lastTimeParticulasFountain;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Framesbuffers
GLuint depthMap, depthMapFBO;

/****************************VARIABLES GLOBALES PARA LA MAUINA DE ESTADOS DE UI MENU****************************/
bool iniciaPartida = false, presionarOpcion = false, presionarOpcion2 = false, presionarOpcion3 = false;
enum state { MENU, CHOOSEPLAYER, PLAYGAME };
state estadoActual = MENU;
float timeInt = -intervaloObstaculos;
int contObs = 0;


/**********************
 * OpenAL config
 */

 // OpenAL Defines
#define NUM_BUFFERS 4
#define NUM_SOURCES 4
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Source 3
ALfloat source3Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };

// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid* data;
int ch;
ALboolean loop;
//Este arreglo controla la ejecucion del sonido
std::vector<bool> sourcesPlay = { true, true, true, true };
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
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);
void inicializacionParticulasFountain();
int getObstaclePosition();
void processObstacles(std::string name);
void gameOver();

void gameOver() {
	isRunning = false;
	animationIndex = 3;

}

void processObstacles(std::string name) {
	
	glm::mat4 modelMatrixObstaculo = std::get<0>(obstaculos.find(name)->second);
	int direccion = -1;
	if (name == "nueve" || name == "diez")
	{
		direccion = 1;
	}
	if (std::get<1>(obstaculos.find(name)->second))
	{	
		modelMatrixObstaculo = glm::translate(modelMatrixObstaculo, glm::vec3(0.0f, 0.0f, deltaTime * velocity * offsetVelObs * direccion));
		modelMatrixObstaculo[3][1] = terrain.getHeightTerrain(modelMatrixObstaculo[3][0], modelMatrixObstaculo[3][2]);
	}
	
	if (modelMatrixObstaculo[3][0] < -1.0f)
	{
		modelMatrixObstaculo = glm::translate(modelMatrixObstaculo, glm::vec3(getObstaclePosition(), 0.0f, 18.0f));
		std::get<1>(obstaculos.find(name)->second) = false;
		contObs--;
	}
	std::get<0>(obstaculos.find(name)->second) = modelMatrixObstaculo;
}

int getObstaclePosition() {
	
	int posObs1 = (rand() % 4) - 1;
	if (posObs1 == 1)
		posObs1 = 2;
	else if (posObs1 == -1)
		posObs1 = -2;
	std::cout << posObs1 << std::endl;
	return posObs1;
}
void inicializacionParticulasFountain() {
	//generarBuffers
	glGenBuffers(1, &initVel);
	glGenBuffers(1, &startTime);

	//generara el espacion de todos los buffers
	int size = numParticulas * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, numParticulas * sizeof(float), NULL, GL_STATIC_DRAW);

	//llenar las velocidades iniciales
	glm::vec3 v(0.0f);
	float velocity, theta, pi;
	GLfloat* data = new GLfloat[numParticulas * 3];
	for (unsigned int i = 0; i < numParticulas; i++) {
		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		pi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));
		v.x = sinf(theta) * cosf(pi);
		v.y = cosf(theta);
		v.z = sinf(theta);

		velocity = glm::mix(0.5f, 0.7f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;
		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data); //a partir de una memoria almacenada se refresca

	//LLenar el buffer de tiempo de inicio
	delete[] data;
	data = new GLfloat[numParticulas];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < numParticulas; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numParticulas * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticulas);
	glBindVertexArray(VAOParticulas);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	shaderTerrain.initialize("../Shaders/terrain.vs", "../Shaders/terrain.fs");
	//shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	//shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	//shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	//shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_fog.vs", "../Shaders/multipleLights_fog.fs");
	//shaderTerrain.initialize("../Shaders/terrain_fog.vs", "../Shaders/terrain_fog.fs");
	shaderParticulasFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	shaderTextura.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxIntro.init();
	boxIntro.setShader(&shaderTextura);
	boxIntro.setScale(glm::vec3(2.0f, 2.0f, 1.0f));

	//Shadows
	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);
	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);
	//Terrain
	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	/*Player*/
	player.setModel("finn");
	modelPlayerAnim.loadModel(player.getPath());
	modelPlayerAnim.setShader(&shaderMulLighting);
	//Pista
	pista.init();
	pista.setShader(&shaderTerrain);
	pista.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
	//Pista2
	pista2.init();
	pista2.setShader(&shaderTerrain);
	pista2.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
	//Pista3
	pista3.init();
	pista3.setShader(&shaderTerrain);
	pista3.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
	//Curva
	curva.init();
	curva.setShader(&shaderTerrain);

	/*Estadio*/
	modelEstadio.loadModel("../models/Estadio/estadioV2.obj");
	modelEstadio.setShader(&shaderMulLighting);
	
	/*Obstaculos*/
	modelObstaculo1.loadModel("../models/obstaculos/ballaAtletismo.obj");
	modelObstaculo1.setShader(&shaderMulLighting);
	modelObstaculo2.loadModel("../models/box/coinBlock1.obj");
	modelObstaculo2.setShader(&shaderMulLighting);
	modelObstaculo3.loadModel("../models/obstaculos/ballaAtletismo.obj");
	modelObstaculo3.setShader(&shaderMulLighting);
	modelObstaculo4.loadModel("../models/box/coinBlock1.obj");
	modelObstaculo4.setShader(&shaderMulLighting);
	modelObstaculo5.loadModel("../models/obstaculos/ballaAtletismo.obj");
	modelObstaculo5.setShader(&shaderMulLighting);
	modelObstaculo6.loadModel("../models/box/coinBlock1.obj");
	modelObstaculo6.setShader(&shaderMulLighting);
	modelObstaculo7.loadModel("../models/obstaculos/ballaAtletismo.obj");
	modelObstaculo7.setShader(&shaderMulLighting);
	modelObstaculo8.loadModel("../models/box/coinBlock1.obj");
	modelObstaculo8.setShader(&shaderMulLighting);
	modelObstaculo9.loadModel("../models/obstaculos/Lego22.obj");
	modelObstaculo9.setShader(& shaderMulLighting);
	modelObstaculo10.loadModel("../models/obstaculos/Lego22.obj");
	modelObstaculo10.setShader(&shaderMulLighting);

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

	//Grass
	modelGrass.loadModel("../models/grass/grassModel.obj");
	modelGrass.setShader(&shaderMulLighting);

	//Fountain
	modelFountain.loadModel("../models/fountain/fountain.obj");
	modelFountain.setShader(&shaderMulLighting);

	textRender = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	textRender->Initialize();

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
	Texture textureTerrainB("../Textures/GroundDirtForest.jpg");
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
	Texture textureTerrainBlendMap("../Textures/blendMap3.png");
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
	}else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texturePistaBlendMap("../Textures/blendMap1.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texturePistaBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texturePistaBlendMap.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &texturePistaBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, texturePistaBlendMapID);
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
	texturePistaBlendMap.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureCurvaBlendMap("../Textures/blendMap2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureCurvaBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureCurvaBlendMap.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureCurvaBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCurvaBlendMapID);
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
	textureCurvaBlendMap.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureParticulaAgua("../Textures/bluewater.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureParticulaAgua.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureParticulaAgua.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureParticleFountainID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
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
	textureParticulaAgua.freeImage(bitmap);

	/*******************************************TEXTURAS DE LA INTERFAZ MENU PRINCIPAL*******************************************/
	// Definiendo la textura a utilizar para el Menu
	Texture textureMainMenu("../Textures/MainMenu.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureMainMenu.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureMainMenu.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &texturaMenuID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, texturaMenuID);
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
	textureMainMenu.freeImage(bitmap);

	// Definiendo la textura a utilizar para la seleccion de personaje FINN
	Texture textureFinnSelect("../Textures/CharacterSelFinn.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureFinnSelect.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureFinnSelect.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &texturaSelFinnID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, texturaSelFinnID);
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
	textureFinnSelect.freeImage(bitmap);

	// Definiendo la textura a utilizar para la seleccion de personaje JAKE
	Texture textureJakeSelect("../Textures/CharacterSelJake.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureJakeSelect.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureJakeSelect.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &texturaSelJakeID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, texturaSelJakeID);
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
	textureJakeSelect.freeImage(bitmap);

	// Definiendo la textura a utilizar para la seleccion de personaje JAKE
	Texture textureLegoSelect("../Textures/CharacterSelLego.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureLegoSelect.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureLegoSelect.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &texturaSelLegoID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, texturaSelLegoID);
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
	textureLegoSelect.freeImage(bitmap);
	/*******************************************FIN TEXTURAS DE LA INTERFAZ MENU PRINCIPAL****************************************/

	//Inicializacion de la funcion para las particulas
	inicializacionParticulasFountain();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Config source 0
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/darth_vader.wav");
	buffer[3] = alutCreateBufferFromFile("../sounds/silbato.wav");

	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR) {
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}


	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 3.0f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 2000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 500);

	alSourcef(source[3], AL_PITCH, 1.0f);
	alSourcef(source[3], AL_GAIN, 0.3f);
	alSourcefv(source[3], AL_POSITION, source3Pos);
	alSourcefv(source[3], AL_VELOCITY, source3Vel);
	alSourcei(source[3], AL_BUFFER, buffer[3]);
	alSourcei(source[3], AL_LOOPING, AL_FALSE);
	alSourcef(source[3], AL_MAX_DISTANCE, 500);
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

		//shadows
		shaderDepth.destroy();
		shaderViewDepth.destroy();
		boxViewDepth.destroy();
		boxLightViewBox.destroy();

		// Basic objects Delete
		skyboxSphere.destroy(); 
		//boxCesped.destroy();
		boxCollider.destroy();
		sphereCollider.destroy();

		// Terrains objects Delete
		terrain.destroy();
		
		// Custom objects Delete
		modelHeliChasis.destroy();
		modelHeliHeli.destroy();
		modelLamp1.destroy();
		modelLamp2.destroy();
		modelLampPost2.destroy();
		modelFountain.destroy();

		modelEstadio.destroy();
		modelObstaculo1.destroy();
		modelObstaculo2.destroy();
		modelObstaculo3.destroy();
		modelObstaculo4.destroy();
		modelObstaculo5.destroy();
		modelObstaculo6.destroy();
		modelObstaculo7.destroy();
		modelObstaculo8.destroy();
		modelObstaculo9.destroy();
		modelObstaculo10.destroy();

		// Custom objects animate
		/*Player*/
		modelPlayerAnim.destroy();
		pista.destroy();
		pista2.destroy();
		pista3.destroy();
		curva.destroy();


		/*Grass*/
		modelGrass.destroy();
		// Textures Delete
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &textureCespedID);
		glDeleteTextures(1, &textureTerrainBackgroundID);
		glDeleteTextures(1, &textureTerrainRID);
		glDeleteTextures(1, &textureTerrainGID);
		glDeleteTextures(1, &textureTerrainBID);
		glDeleteTextures(1, &textureTerrainBlendMapID);
		glDeleteTextures(1, &textureParticleFountainID);
		/**************************LIBERACION DE MEMORIA DE TEXTURAS************************/
		glDeleteTextures(1, &texturaActivaID);
		glDeleteTextures(1, &texturaMenuID);
		glDeleteTextures(1, &texturaSelFinnID);
		glDeleteTextures(1, &texturaSelJakeID);
		glDeleteTextures(1, &texturaSelLegoID);

		// Cube Maps Delete
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glDeleteTextures(1, &skyboxTextureID);

		//Elimina el buffer de la fuente de agua(Particulas)
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &initVel);
		glDeleteBuffers(1, &startTime);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAOParticulas);
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
		/*distanceFromPlayer -= yoffset;
		camera->setDistanceFromTarget(distanceFromPlayer);*/
	}

	bool processInput(bool continueApplication) {
		if (exitApp || glfwWindowShouldClose(window) != 0) {
			return false;
		}

		if (!iniciaPartida) {
			bool statusEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;

			if (!presionarOpcion && statusEnter && estadoActual == MENU) {
				presionarOpcion = true;
				texturaActivaID = texturaSelFinnID;
				estadoActual = CHOOSEPLAYER;

			}
			else if (!presionarOpcion && statusEnter && estadoActual == CHOOSEPLAYER) {
				iniciaPartida = true;
				estadoActual = PLAYGAME;
			}
			else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
				presionarOpcion = false;
			}

			if (!presionarOpcion3 && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && estadoActual == CHOOSEPLAYER) {
				presionarOpcion3 = true;
				if (texturaActivaID == texturaSelJakeID) {
					texturaActivaID = texturaSelFinnID;
				}
				else if (texturaActivaID == texturaSelLegoID) {
					texturaActivaID = texturaSelJakeID;
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && estadoActual == CHOOSEPLAYER)
				presionarOpcion3 = false;

			if (!presionarOpcion2 && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				presionarOpcion2 = true;
				if (texturaActivaID == texturaSelFinnID) {
					texturaActivaID = texturaSelJakeID;
				}
				else if (texturaActivaID == texturaSelJakeID) {
					texturaActivaID = texturaSelLegoID;
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE && estadoActual == CHOOSEPLAYER)
				presionarOpcion2 = false;

		}
		
		/*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			camera->mouseMoveCamera(offsetX, 0, deltaTime);
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			camera->mouseMoveCamera(0, offsetY, deltaTime);
		}*/

		//Process joystick
		if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE)
		{
			//std::cout << "Esta conectado el joystick 0" << std::endl;
			int numberAxes, numberBotones;
			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &numberAxes);
			//std::cout << "Numero de ejes:... " << numberAxes << std::endl;
			//std::cout << "Axes[0]->" << axes[0] << std:: endl;
			//std::cout << "Axes[1]->" << axes[1] << std::endl;
			//std::cout << "Axes[2]->" << axes[2] << std::endl;
			//std::cout << "Axes[3]->" << axes[3] << std::endl;
			//std::cout << "Axes[4]->" << axes[4] << std::endl;
			//std::cout << "Axes[5]->" << axes[5] << std::endl;

			if (fabs(axes[1]) > 0.2f)
			{
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 0, axes[1] * 0.1f));
				animationIndex = 0;
			}

			if (fabs(axes[0]) > 0.2f)
			{
				modelMatrixPlayer = glm::rotate(modelMatrixPlayer, glm::radians(-axes[0] * 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
				animationIndex = 0;
			}

			if (fabs(-axes[2]) > 0.2f)
			{
				camera->mouseMoveCamera(axes[2] * 0.5f, 0.0f, deltaTime);
			}

			if (fabs(-axes[3]) > 0.2f)
			{
				camera->mouseMoveCamera(0.0f, -axes[3] * 0.5f, deltaTime);
			}
			const unsigned char* botones = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &numberBotones);
			//std::cout << "numero de botones " << numberBotones << std::endl;
			//if (botones[0] == GLFW_PRESS)
			//	std::cout << "se presiona" << std::endl;
			if (!isJump && botones[0] == GLFW_PRESS)
			{
				isJump = true;
				tmv = 0;
				startTimeJump = currTime;
			}
		}

		/*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			camera->mouseMoveCamera(0.0, offsetY, deltaTime);*/
		offsetX = 0;
		offsetY = 0;

		// Seleccionar modelo
		//if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		//	enableCountSelected = false;
		//	modelSelected++;
		//	std::cout << "modelSelected = " << modelSelected << std::endl;
		//	if (modelSelected > 3)
		//		modelSelected = 0;
		//}
		//else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		//	enableCountSelected = true;

		/* Controles Player*/
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			
			if (step == 3 && !isPress)
				step = 2;
			else if ((step == 1 || step == 2) && !isPress)
				step = 1;
			std::cout << "Step: " << step << std::endl;
			isRight = false;
			isPress = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			if ((step == 3 || step == 2) && !isPress)
				step = 3;
			else if (step == 1 && !isPress)
				step = 2;
			std::cout << "Step: " << step << std::endl;
			isRight = true;
			isPress = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			//modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0.0f, 0.0f, 0.03f));
			//animationIndex = 1;
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			//modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0.0f, 0.0f, -0.03f));
			//animationIndex = 1; 
		}
		//Salto
		bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		if (!isJump && keySpaceStatus)
		{
			isJump = true;
			tmv = 0;
			startTimeJump = currTime;
			animationIndex = 4;
		}

		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
		{
			isRunning = true;
			isStart = false;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		{
			isRunning = !isRunning;
		}

		glfwPollEvents();
		return continueApplication;
	}
	
	void applicationLoop() {
		bool psi = true;

		glm::mat4 view;
		glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 axisTarget;
		float angleTarget;

		float startTimeCont = 0.0f, pauseTime = 0.0f, runningTime = 0.0f;

		modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0.0f, 3.0f, 0.0f));
		modelMatrixPlayer = glm::rotate(modelMatrixPlayer, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixEstadio = glm::translate(modelMatrixEstadio, glm::vec3(0.0f));

		modelMatrixCurva = glm::rotate(modelMatrixCurva, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixCurva = glm::translate(modelMatrixCurva, glm::vec3(0.0f, 0.0f, 12.0f));

		namesObs.push_back("uno");
		obstaculos["uno"] = std::make_tuple(glm::mat4(1.0f), false, &modelObstaculo1);
		std::get<0>(obstaculos.find("uno")->second) = glm::translate(
			std::get<0>(obstaculos.find("uno")->second), glm::vec3(18.0f, 0.0f, getObstaclePosition()));
		std::get<0>(obstaculos.find("uno")->second) = glm::rotate(
			std::get<0>(obstaculos.find("uno")->second), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		/*std::get<0>(obstaculos.find("uno")->second) = glm::scale(
			std::get<0>(obstaculos.find("uno")->second), glm::vec3(1.0f) * 5.0f);*/

		namesObs.push_back("dos");
		obstaculos["dos"] = std::make_tuple(glm::mat4(1.0f), false, &modelObstaculo2);
		std::get<0>(obstaculos.find("dos")->second) = glm::translate(
			std::get<0>(obstaculos.find("dos")->second), glm::vec3(18.0f, 0.5f, getObstaclePosition()));
		std::get<0>(obstaculos.find("dos")->second) = glm::rotate(
			std::get<0>(obstaculos.find("dos")->second), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		namesObs.push_back("tres");
		obstaculos["tres"] = std::make_tuple(glm::mat4(1.0f), false, &modelObstaculo3);
		std::get<0>(obstaculos.find("tres")->second) = glm::translate(
			std::get<0>(obstaculos.find("tres")->second), glm::vec3(18.0f, 0.0f, getObstaclePosition()));
		std::get<0>(obstaculos.find("tres")->second) = glm::rotate(
			std::get<0>(obstaculos.find("tres")->second), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		namesObs.push_back("cuatro");
		obstaculos["cuatro"] = std::make_tuple(glm::mat4(1.0f), false, &modelObstaculo4);
		std::get<0>(obstaculos.find("cuatro")->second) = glm::translate(
			std::get<0>(obstaculos.find("cuatro")->second), glm::vec3(18.0f, 0.5f, getObstaclePosition()));
		std::get<0>(obstaculos.find("cuatro")->second) = glm::rotate(
			std::get<0>(obstaculos.find("cuatro")->second), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		namesObs.push_back("cinco");
		obstaculos["cinco"] = std::make_tuple(glm::mat4(1.0f), false, &modelObstaculo5);
		std::get<0>(obstaculos.find("cinco")->second) = glm::translate(
			std::get<0>(obstaculos.find("cinco")->second), glm::vec3(18.0f, 0.0f, getObstaclePosition()));
		std::get<0>(obstaculos.find("cinco")->second) = glm::rotate(
			std::get<0>(obstaculos.find("cinco")->second), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		namesObs.push_back("seis");
		obstaculos["seis"] = std::make_tuple(glm::mat4(1.0f), false, &modelObstaculo6);
		std::get<0>(obstaculos.find("seis")->second) = glm::translate(
			std::get<0>(obstaculos.find("seis")->second), glm::vec3(18.0f, 0.5f, getObstaclePosition()));
		std::get<0>(obstaculos.find("seis")->second) = glm::rotate(
			std::get<0>(obstaculos.find("seis")->second), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		namesObs.push_back("siete");
		obstaculos["siete"] = std::make_tuple(glm::mat4(1.0f), false, &modelObstaculo7);
		std::get<0>(obstaculos.find("siete")->second) = glm::translate(
			std::get<0>(obstaculos.find("siete")->second), glm::vec3(18.0f, 0.0f, getObstaclePosition()));
		std::get<0>(obstaculos.find("siete")->second) = glm::rotate(
			std::get<0>(obstaculos.find("siete")->second), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		namesObs.push_back("ocho");
		obstaculos["ocho"] = std::make_tuple(glm::mat4(1.0f), false, &modelObstaculo8);
		std::get<0>(obstaculos.find("ocho")->second) = glm::translate(
			std::get<0>(obstaculos.find("ocho")->second), glm::vec3(18.0f, 0.5f, getObstaclePosition()));
		std::get<0>(obstaculos.find("ocho")->second) = glm::rotate(
			std::get<0>(obstaculos.find("ocho")->second), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		namesObs.push_back("nueve");
		obstaculos["nueve"] = std::make_tuple(glm::mat4(1.0f), false, &modelObstaculo9);
		std::get<0>(obstaculos.find("nueve")->second) = glm::translate(
			std::get<0>(obstaculos.find("nueve")->second), glm::vec3(18.0f, 0.0f, getObstaclePosition()));
		std::get<0>(obstaculos.find("nueve")->second) = glm::rotate(
			std::get<0>(obstaculos.find("nueve")->second), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		namesObs.push_back("diez");
		obstaculos["diez"] = std::make_tuple(glm::mat4(1.0f), false, &modelObstaculo9);
		std::get<0>(obstaculos.find("diez")->second) = glm::translate(
			std::get<0>(obstaculos.find("diez")->second), glm::vec3(18.0f, 0.0f, getObstaclePosition()));
		std::get<0>(obstaculos.find("diez")->second) = glm::rotate(
			std::get<0>(obstaculos.find("diez")->second), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));


		modelMatrixFountain = glm::translate(modelMatrixFountain, glm::vec3(5.0, 0.0, -40.0));
		modelMatrixFountain[3][1] = terrain.getHeightTerrain(modelMatrixFountain[3][0], modelMatrixFountain[3][2]) + 0.2;
		modelMatrixFountain = glm::scale(modelMatrixFountain, glm::vec3(10.0f, 10.0f, 10.0f));
		
		glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);
		shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.0f, 45.0f);
		texturaActivaID = texturaMenuID;

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

			std::map<std::string, bool> collisionDetection;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Manejo de la camara
			glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
			glm::mat4 view = camera->getViewMatrix();
			axisTarget = glm::axis(glm::quat_cast(modelMatrixPlayer));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixPlayer));
			target = glm::vec3(modelMatrixPlayer[3]) + glm::vec3(-2.0f, 2.5f, 0.0f);


			if (std::isnan(angleTarget))
			{
				angleTarget = 0.0f;
			}

			if (axisTarget.y == 0)
			{
				angleTarget = -angleTarget;
			}

			camera->setCameraTarget(target);
			camera->setAngleTarget(angleTarget);
			camera->updateCamera();
			view = camera->getViewMatrix();


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

			shaderParticulasFountain.setMatrix4("projection", 1, false, glm::value_ptr(projection));
			shaderParticulasFountain.setMatrix4("view", 1, false, glm::value_ptr(view));

			/*******************************************
			* Propiedades de neblina
			*******************************************/
			shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
			shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
			shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));


			ambientLight = glm::vec3(0.5f, 0.5f, 0.5f);
			diffuseLight = glm::vec3(0.3f, 0.3f, 0.3f);
			specularLight = glm::vec3(0.4f, 0.4f, 0.4f);
			directionLight = glm::vec3(-1.0f, 0.0f, 0.0f);
			/*******************************************
			 * Propiedades Luz direccional
			 *******************************************/

			shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
			shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(ambientLight));
			shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(diffuseLight));
			shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(specularLight));
			shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

			/*******************************************
			 * Propiedades Luz direccional Terrain
			 *******************************************/
			shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
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

			if (!iniciaPartida)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, screenWidth, screenHeight);
				shaderTextura.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0f)));
				shaderTextura.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0f)));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texturaActivaID);
				boxIntro.render();
				glfwSwapBuffers(window);
				continue;
			}

			/*******************************************
			 * 1.- We render the depth buffer
			 *******************************************/
			glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//Render de la escena desde el punto de vista del observador
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			//glCullFace(GL_FRONT);
			//Setea la profundidad
			prepareDepthScene();
			renderScene(false);
			//glCullFace(GL_BACK);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//Solo para debug, se ve lo que se dibuja desde la luz
			//Reset viewport
			/*glViewport(0, 0, screenWidth, screenHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//renderizado en un cuadrado
			shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0f)));
			shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0f)));
			shaderViewDepth.setFloat("near_plane", near_plane);
			shaderViewDepth.setFloat("far_plane", far_plane);
			shaderViewDepth.setInt("depthMap", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			boxViewDepth.setScale(glm::vec3(2.0f, 2.0f, 1.0f));
			boxViewDepth.render();*/

			/*******************************************
			 * 2.- We render the normal objects
			 *******************************************/ 
			glViewport(0, 0, screenWidth, screenHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			prepareScene();
			glActiveTexture(GL_TEXTURE10);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			shaderMulLighting.setInt("shadowMap", 10);
			shaderTerrain.setInt("shadowMap", 10);
			renderScene(true);
			
			
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

			/**********
			 * Update the position with alpha objects
			 */
			 //update the fountain
			blendingUnsorted.find("particulasAgua")->second = glm::vec3(modelMatrixFountain[3]);

			/**********
			 * Sorter with alpha objects
			 */
			std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
			std::map<std::string, glm::vec3>::iterator itblend;
			for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++) {
				float distanceFromView = glm::length(camera->getPosition() - itblend->second);
				blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
			}
			/**********
			 * Render de las transparencias
			 */
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_CULL_FACE);
			for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {
				if (it->second.first.compare("particulasAgua") == 0) {
					//Inicializacion del sistema de particulas
					glm::mat4 modelMatrixParticulasFuente = glm::mat4(1.0f);
					modelMatrixParticulasFuente = glm::translate(modelMatrixParticulasFuente, it->second.second);
					modelMatrixParticulasFuente[3][1] += 3.7f;
					modelMatrixParticulasFuente = glm::scale(modelMatrixParticulasFuente, glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f);
					currTimeParticulasFountain = TimeManager::Instance().GetTime();
					if (currTimeParticulasFountain - lastTimeParticulasFountain > 17.0f)
						lastTimeParticulasFountain = currTimeParticulasFountain;
					glDepthMask(GL_FALSE);
					glPointSize(10.0f);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
					shaderParticulasFountain.turnOn();
					shaderParticulasFountain.setFloat("Time", float(currTimeParticulasFountain - lastTimeParticulasFountain));
					shaderParticulasFountain.setFloat("ParticleLifetime", 16.0f);
					shaderParticulasFountain.setVectorFloat3("Gravity", glm::value_ptr(glm::vec3(0.0f, -0.1f, 0.0f)));
					shaderParticulasFountain.setMatrix4("model", 1, false, glm::value_ptr(modelMatrixParticulasFuente));
					shaderParticulasFountain.setInt("ParticleTex", 0);
					glBindVertexArray(VAOParticulas);
					glDrawArrays(GL_POINTS, 0, numParticulas);
					glDepthMask(GL_TRUE);
					shaderParticulasFountain.turnOff();
				}
			}
			glEnable(GL_CULL_FACE);

			/*******************************************
			 * Creacion de colliders
			 * IMPORTANT do this before interpolations
			 *******************************************/
			 //Player
			glm::mat4 modelMatrixColliderPlayer = glm::mat4(modelMatrixPlayer);
			AbstractModel::OBB playerCollider;

			modelMatrixColliderPlayer = glm::rotate(modelMatrixColliderPlayer,
				glm::radians(player.getAngleRotCol()), player.getVectorRotCol());
			playerCollider.u = glm::quat_cast(modelMatrixColliderPlayer);
			modelMatrixColliderPlayer = glm::scale(modelMatrixColliderPlayer, glm::vec3(1.0f, 1.0f, 1.0f) * player.getScaleCol());
			modelMatrixColliderPlayer = glm::translate(modelMatrixColliderPlayer, modelPlayerAnim.getObb().c);
			playerCollider.c = glm::vec3(modelMatrixColliderPlayer[3]) + player.getOffsetC();
			playerCollider.e = (modelPlayerAnim.getObb().e + player.getOffsetE()) *
				((player.getScaleCol() == 0.0f) ? 1.0f : player.getScaleCol());
			addOrUpdateColliders(collidersOBB, "player", playerCollider, modelMatrixPlayer);

			/*************
			* Obstaculos *
			**************/
			int l = 0;
			for (std::map<std::string, std::tuple<glm::mat4, bool, Model*>>::iterator it = obstaculos.begin();
				it != obstaculos.end(); it++)
			{
				AbstractModel::OBB obstacleCollider;
				glm::mat4 modelMatrixObsCollider = glm::mat4(1.0f);
				modelMatrixObsCollider = std::get<0>(it->second);
				obstacleCollider.u = glm::quat_cast(modelMatrixObsCollider);
				modelMatrixObsCollider = glm::translate(modelMatrixObsCollider, std::get<2>(it->second)->getObb().c);
				obstacleCollider.c = glm::vec3(modelMatrixObsCollider[3]);
				obstacleCollider.e = std::get<2>(it->second)->getObb().e;
				addOrUpdateColliders(collidersOBB, "obstacle-" + std::to_string(l), obstacleCollider, modelMatrixObsCollider);
				l++;
			}


			//Colision caja vs caja
			for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it = collidersOBB.begin();
				it != collidersOBB.end(); it++)
			{
				bool isCollision = false;
				for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt = collidersOBB.begin();
					jt != collidersOBB.end() && !isCollision; jt++)
				{
					if (it != jt && testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second)))
					{
						//std::cout << "Existe colisión entre: " << it->first << " y " << jt->first << std::endl;
						isCollision = true;
					}
				}
				addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
			}

			std::map<std::string, bool>::iterator it2;
			for (it2 = collisionDetection.begin(); it2 != collisionDetection.end(); it2++)
			{
				std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator
					modeloBuscadoOBB = collidersOBB.find(it2->first);
				std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator
					modeloBuscadoSBB = collidersSBB.find(it2->first);

				if (modeloBuscadoSBB != collidersSBB.end())
				{
					if (!it2->second)
					{
						addOrUpdateColliders(collidersSBB, it2->first);
					}
				}

				if (modeloBuscadoOBB != collidersOBB.end())
				{
					if (!it2->second)
					{
						addOrUpdateColliders(collidersOBB, it2->first);
					}
					else
					{
						if (modeloBuscadoOBB->first.compare("player") == 0)
						{
							std::cout << "Hay colision con player" << std::endl;
							gameOver();
						}
					}
				}
			}

			/*******************************************
			 * Render de colliders
			*******************************************/
			for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
				glm::mat4 matrixCollider = glm::mat4(1.0);
				matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
				matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
				matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
				boxCollider.setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
				boxCollider.enableWireMode();
				boxCollider.render(matrixCollider);
			}


		//UI
		std::stringstream miliTxt;
		std::stringstream segTxt;
		std::stringstream minTxt;
		float time = TimeManager::Instance().GetRunningTime();
		int segundos = 0, minutos = 0, milisegundos = 0;

		/*Controlador Time para UI*/
		if (!isRunning && startTimeCont)
			startTimeCont = time;
		else if (!isRunning && !startTimeCont)
		{
			pauseTime = time - startTime - runningTime;
		}
		else if (isRunning && !startTimeCont)
		{
			runningTime = time - startTime - pauseTime;
		}

		if (isRunning)
		{
			milisegundos = (int)(fmod(runningTime, 1) * 100);
			if (time >= 60)
			{
				minutos = (int)runningTime / 60;
				segundos = (int)(runningTime - (minutos * 60));
			}
			else
			{
				segundos = (int)(runningTime);
			}	
		}
		miliTxt << std::setw(2) << std::setfill('0') << milisegundos;
		segTxt << std::setw(2) << std::setfill('0') << segundos;
		minTxt << std::setw(2) << std::setfill('0') << minutos;
		std::string s = minTxt.str() + ":" + segTxt.str() + ":" + miliTxt.str();
		glEnable(GL_BLEND);
		textRender->render(s, 0.6, 0.8, 60, 1.0, 0.9, 0.2);
		glDisable(GL_BLEND);

		/*********************
		* Maquinas de estado *
		**********************/

		switch(step) {
			case 1:
				if (!isRight && modelMatrixPlayer[3][2] >= -2.0)
				{
					modelMatrixPlayer[3][2] = modelMatrixPlayer[3][2] - stepVel;
				}
				if (modelMatrixPlayer[3][2] < -2.0 && isPress)
				{
					modelMatrixPlayer[3][2] = -2;
					isPress = false;
				}
				break;
			case 2:
				if (isRight && modelMatrixPlayer[3][2] <= 0)
				{
					modelMatrixPlayer[3][2] = modelMatrixPlayer[3][2] + stepVel;
				}
				if (isRight && modelMatrixPlayer[3][2] > 0 && isPress)
				{
					isPress = false;
				}
				if (!isRight && modelMatrixPlayer[3][2] > 0) {
					modelMatrixPlayer[3][2] = modelMatrixPlayer[3][2] - stepVel;
				}
				if (!isRight && modelMatrixPlayer[3][2] < 0 && isPress)
				{
					isPress = false;
				}
				
				break;
				
			case 3:
				if (isRight && modelMatrixPlayer[3][2] <= 2)
				{
					std::cout << modelMatrixPlayer[3][2] << std::endl;
					modelMatrixPlayer[3][2] = modelMatrixPlayer[3][2] + stepVel;
				}
				if (isRight && modelMatrixPlayer[3][2] > 2 && isPress)
				{
					isPress = false;
				}
				break;
		}
		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		/*source0Pos[0] = modelMatrixFountain[3].x;
		source0Pos[1] = modelMatrixFountain[3].y;
		source0Pos[2] = modelMatrixFountain[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source2Pos[0] = modelMatrixDart[3].x;
		source2Pos[1] = modelMatrixDart[3].y;
		source2Pos[2] = modelMatrixDart[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);

		source3Pos[0] = modelMatrixLambo[3].x;
		source3Pos[1] = modelMatrixLambo[3].y;
		source3Pos[2] = modelMatrixLambo[3].z;
		alSourcefv(source[3], AL_POSITION, source3Pos);*/

		// Listener for the Thris person camera
	/*	listenerPos[0] = modelMatrixMayow[3].x;
		listenerPos[1] = modelMatrixMayow[3].y;
		listenerPos[2] = modelMatrixMayow[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixMayow[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixMayow[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;*/

		// Listener for the First person camera
		listenerPos[0] = camera->getPosition().x;
		listenerPos[1] = camera->getPosition().y;
		listenerPos[2] = camera->getPosition().z;
		alListenerfv(AL_POSITION, listenerPos);
		listenerOri[0] = camera->getFront().x;
		listenerOri[1] = camera->getFront().y;
		listenerOri[2] = camera->getFront().z;
		listenerOri[3] = camera->getUp().x;
		listenerOri[4] = camera->getUp().y;
		listenerOri[5] = camera->getUp().z;
		alListenerfv(AL_ORIENTATION, listenerOri);

		for (unsigned int i = 0; i < sourcesPlay.size(); i++) {
			if (sourcesPlay[i]) {
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}


	}
}

	void prepareScene() {
		skyboxSphere.setShader(&shaderSkybox);

		boxCollider.setShader(&shader);
		sphereCollider.setShader(&shader);

		boxViewDepth.setShader(&shaderViewDepth);

		//Terrain
		terrain.setShader(&shaderTerrain);

		/*Player*/
		modelPlayerAnim.setShader(&shaderMulLighting);
		//Pista
		pista.setShader(&shaderTerrain);
		pista2.setShader(&shaderTerrain);
		pista3.setShader(&shaderTerrain);
		curva.setShader(&shaderTerrain);

		/*Estadio*/
		modelEstadio.setShader(&shaderMulLighting);

		/*Obstaculos*/
		modelObstaculo1.setShader(&shaderMulLighting);
		modelObstaculo2.setShader(&shaderMulLighting);
		modelObstaculo3.setShader(&shaderMulLighting);
		modelObstaculo4.setShader(&shaderMulLighting);
		modelObstaculo5.setShader(&shaderMulLighting);
		modelObstaculo6.setShader(&shaderMulLighting);
		modelObstaculo7.setShader(&shaderMulLighting);
		modelObstaculo8.setShader(&shaderMulLighting);
		modelObstaculo9.setShader(&shaderMulLighting);
		modelObstaculo10.setShader(&shaderMulLighting);

		//Lamp models
		modelLamp1.setShader(&shaderMulLighting);
		modelLamp2.setShader(&shaderMulLighting);
		modelLampPost2.setShader(&shaderMulLighting);

	}

	void prepareDepthScene() {
		skyboxSphere.setShader(&shaderDepth);

		boxCollider.setShader(&shaderDepth);
		sphereCollider.setShader(&shaderDepth);


		//Terrain
		terrain.setShader(&shaderDepth);

		/*Player*/
		modelPlayerAnim.setShader(&shaderDepth);
		//Pista
		pista.setShader(&shaderDepth);
		pista2.setShader(&shaderDepth);
		pista3.setShader(&shaderDepth);
		curva.setShader(&shaderDepth);

		/*Estadio*/
		modelEstadio.setShader(&shaderDepth);

		/*Obstaculos*/
		modelObstaculo1.setShader(&shaderDepth);
		modelObstaculo2.setShader(&shaderDepth);
		modelObstaculo3.setShader(&shaderDepth);
		modelObstaculo4.setShader(&shaderDepth);
		modelObstaculo5.setShader(&shaderDepth);
		modelObstaculo6.setShader(&shaderDepth);
		modelObstaculo7.setShader(&shaderDepth);
		modelObstaculo8.setShader(&shaderDepth);
		modelObstaculo9.setShader(&shaderDepth);
		modelObstaculo10.setShader(&shaderDepth);

		//Lamp models
		modelLamp1.setShader(&shaderDepth);
		modelLamp2.setShader(&shaderDepth);
		modelLampPost2.setShader(&shaderDepth);
	}

	void renderScene(bool renderParticles) { 
		/*******************************************
		* Terrain Cesped
		*******************************************/
		// Se activa la textura del background
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
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
		/*glm::mat4 modelMatrixLego = glm::mat4(1.0f);
		modelMatrixLego = glm::translate(modelMatrixLego, glm::vec3(3.0f, 0.0f, 0.0f));
		modelMatrixLego[3][1] = terrain.getHeightTerrain(modelMatrixLego[3][0], modelMatrixLego[3][2]);
		modelMatrixLego = glm::rotate(modelMatrixLego, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixLego = glm::scale(modelMatrixLego, glm::vec3(1.0f) * 1.0f);
		modelLego.render(modelMatrixLego);*/

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

		/*Render modelo GRASS*/
		//glDisable(GL_CULL_FACE);
		//glm::vec3 grassPosition = glm::vec3(0.0f);
		//grassPosition.y = terrain.getHeightTerrain(grassPosition.x, grassPosition.z);
		//modelGrass.setPosition(grassPosition);
		//modelGrass.render();
		//glEnable(GL_CULL_FACE);

		// Fountain
		glDisable(GL_CULL_FACE);
		modelFountain.render(modelMatrixFountain);
		glEnable(GL_CULL_FACE);

		glm::mat4 modelMatrixEstadioBody = glm::mat4(modelMatrixEstadio);
		modelMatrixEstadioBody = glm::rotate(modelMatrixEstadioBody, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixEstadioBody[3][1] = terrain.getHeightTerrain(modelMatrixEstadioBody[3][0], modelMatrixEstadioBody[3][2]);
		modelMatrixEstadioBody = glm::scale(modelMatrixEstadioBody, glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f);
		modelEstadio.render(modelMatrixEstadioBody);

		/*******************
		* Obstaculos       *
		 ******************/
		if (isRunning)
		{
			int obstaculo = 0;

			if (TimeManager::Instance().GetRunningTime() - timeInt > intervaloObstaculos &&
				contObs < obstaculos.size())
			{
				do
				{
					obstaculo = rand() % obstaculos.size();
				} while (std::get<1>(obstaculos.find(namesObs.at(obstaculo))->second));

				std::get<1>(obstaculos.find(namesObs.at(obstaculo))->second) = true;
				contObs++;
				timeInt = TimeManager::Instance().GetRunningTime();
			}

			for (std::map<std::string, std::tuple<glm::mat4, bool, Model*>>::iterator it = obstaculos.begin();
				it != obstaculos.end(); it++)
			{
				if (std::get<1>(it->second))
				{
					processObstacles(it->first);
					std::get<2>(it->second)->render(std::get<0>(it->second));
				}
			}
		}

		/*******************************************
		* Custom Anim objects obj
		*******************************************/
		modelMatrixPlayer[3][1] = -tmv * tmv * gravity + 3.0 * tmv + terrain.getHeightTerrain(modelMatrixPlayer[3][0], modelMatrixPlayer[3][2]);
		tmv = currTime - startTimeJump;
		if (modelMatrixPlayer[3][1] < terrain.getHeightTerrain(modelMatrixPlayer[3][0], modelMatrixPlayer[3][2]))
		{
			isJump = false;
			modelMatrixPlayer[3][1] = terrain.getHeightTerrain(modelMatrixPlayer[3][0], modelMatrixPlayer[3][2]);
			//animationIndex = 1;
		}
		glm::mat4 modelMatrixPlayerBody = glm::mat4(modelMatrixPlayer);
		modelMatrixPlayerBody = glm::scale(modelMatrixPlayerBody, glm::vec3(1.0f, 1.0f, 1.0f) * player.getModelScale());
		if (isRunning)
			animationIndex = 1;
		//else
		//	animationIndex = 0;
		modelPlayerAnim.setAnimationIndex(animationIndex);
		modelPlayerAnim.render(modelMatrixPlayerBody);

		//Pista
		glm::mat4 matrixPista = glm::mat4(1.0f);
		matrixPista[3][1] = terrain.getHeightTerrain(matrixPista[3][0], matrixPista[3][2]);
		matrixPista = glm::rotate(matrixPista, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		matrixPista = glm::scale(matrixPista, glm::vec3(12.0f, 0.1f, 12.0f));

		// Se activa la textura del background
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
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
		glBindTexture(GL_TEXTURE_2D, texturePistaBlendMapID);
		shaderTerrain.setInt("textureBlendMap", 5);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(10, 10)));
		if (isRunning)
			pista.setPosition(pista.getPosition() + glm::vec3(0.0f, 0.0f, -deltaTime * velocity));
		if (pista.getPosition().z <= -0.6 && esInicio) {
			pista.setPosition(glm::vec3(0.0, 0.0, 2.4));
		}
		else if (pista.getPosition().z < -0.6 && !esInicio && esPista1)
		{
			pista.setPosition(glm::vec3(0.0, 0.0, 2.4));
		}
		pista.render(matrixPista);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		//Pista2
		glm::mat4 matrixPista2 = glm::mat4(1.0f);
		matrixPista2[3][1] = terrain.getHeightTerrain(matrixPista2[3][0], matrixPista2[3][2]);
		matrixPista2 = glm::translate(matrixPista2, glm::vec3(12.0f, 0.0f, 0.0f));
		matrixPista2 = glm::rotate(matrixPista2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		matrixPista2 = glm::scale(matrixPista2, glm::vec3(12.0f, 0.1f, 12.0f));

		// Se activa la textura del background
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
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
		glBindTexture(GL_TEXTURE_2D, texturePistaBlendMapID);
		shaderTerrain.setInt("textureBlendMap", 5);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(10, 10)));
		if (pista2.getPosition().z <= -1.8 && esInicio) {
			pista2.setPosition(glm::vec3(0.0, 0.0, 1.2));
		}
		else if (pista2.getPosition().z <= -1.8 && !esInicio && esPista1) {
			pista2.setPosition(glm::vec3(0.0, 0.0, 1.2));
		}
		if (isRunning)
			pista2.setPosition(pista2.getPosition() + glm::vec3(0.0f, 0.0f, -deltaTime * velocity));

		pista2.render(matrixPista2);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		//Pista3
		glm::mat4 matrixPista3 = glm::mat4(1.0f);
		matrixPista3[3][1] = terrain.getHeightTerrain(matrixPista3[3][0], matrixPista3[3][2]);
		matrixPista3 = glm::translate(matrixPista3, glm::vec3(24.0f, 0.0f, 0.0f));
		matrixPista3 = glm::rotate(matrixPista3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		matrixPista3 = glm::scale(matrixPista3, glm::vec3(12.0f, 0.1f, 12.0f));

		// Se activa la textura del background
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
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
		glBindTexture(GL_TEXTURE_2D, texturePistaBlendMapID);
		shaderTerrain.setInt("textureBlendMap", 5);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(10, 10)));
		if (pista3.getPosition().z <= -3.0 && esInicio) {
			pista3.setPosition(glm::vec3(0.0, 0.0, 0.0));
			esInicio = false;
			esPista1 = true;
		}
		else if (pista3.getPosition().z <= -3.0 && !esInicio && esPista1) {
			pista3.setPosition(glm::vec3(0.0, 0.0, 0.0));
		}
		if (isRunning)
			pista3.setPosition(pista3.getPosition() + glm::vec3(0.0f, 0.0f, -deltaTime * velocity));

		pista3.render(matrixPista3);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		//std::cout << pista.getPosition().z << "," << pista2.getPosition().z << "," << pista3.getPosition().z << std::endl;

		//Curva
		//glm::mat4 modelMatrixCurvaBody = modelMatrixCurva;
		//modelMatrixCurvaBody[3][1] = terrain.getHeightTerrain(modelMatrixCurvaBody[3][0], modelMatrixCurvaBody[3][2]);
		////modelMatrixCurvaBody = glm::translate(modelMatrixCurvaBody, glm::vec3(0.0f, 0.0f, -deltaTime*10));
		////modelMatrixCurvaBody = glm::rotate(modelMatrixCurvaBody, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//modelMatrixCurvaBody = glm::scale(modelMatrixCurvaBody, glm::vec3(12.0f, 0.1f, 12.0f));
		//
		//// Se activa la textura del background
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureCespedID);
		//shaderTerrain.setInt("backgroundTexture", 0);
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
		//shaderTerrain.setInt("textureB", 2);
		//glActiveTexture(GL_TEXTURE3);
		//glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
		//shaderTerrain.setInt("textureR", 3);
		//glActiveTexture(GL_TEXTURE4);
		//glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
		//glActiveTexture(GL_TEXTURE5);
		//shaderTerrain.setInt("textureG", 4);
		//glBindTexture(GL_TEXTURE_2D, textureCurvaBlendMapID);
		//shaderTerrain.setInt("textureBlendMap", 5);
		//shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(10, 10)));
		//if (curva.getPosition().z < -0.45 && curva.getOrientation().y < 90) {
		//	curva.setOrientation(curva.getOrientation() + glm::vec3(0.0f, deltaTime * 9, 0.0f));
		//	pista.setOrientation(pista.getOrientation() + glm::vec3(0.0f, deltaTime * 9, 0.0f));
		//}
		//std::cout << curva.getPosition().z << ","<< curva.getOrientation().y<<std::endl;
		//curva.setPosition(curva.getPosition() + glm::vec3(0.0f, 0.0f, -deltaTime * velocity));
		//curva.render(modelMatrixCurvaBody);
		//shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		//glBindTexture(GL_TEXTURE_2D, 0);

		glEnable(GL_CULL_FACE);

	}

int main(int argc, char** argv)
{
	srand(time(NULL));
	init(1366, 820, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}