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

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

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
//Shader con varias texturas
Shader shaderMultiTextures;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Box boxMultiTexture;
Sphere esfera1(15, 15, 1.0);//Creando una esfera con cortes verticales y horizontales, con un radio
// Models complex instances
Model modelRock;
Model modelAircraft;
Model modelEclipseChasis;
Model modelEclipseRearWheels;
Model modelEclipseFrontalWheels;
Model modelHeliChasis;
Model modelHeliHeli;
Model modelHeliTra;
Model modelLambo;
Model modelLamboLeftDor;
Model modelLamboRightDor;
Model modelLamboFrontLeftWheel;
Model modelLamboFrontRightWheel;
Model modelLamboRearLeftWheel;
Model modelLamboRearRightWheel;
// Dart lego
Model modelDartLegoBody;
Model modelDartLegoHead;
Model modelDartLegoMask;
Model modelDartLegoLeftArm;
Model modelDartLegoRightArm;
Model modelDartLegoLeftHand;
Model modelDartLegoRightHand;
Model modelDartLegoLeftLeg;
Model modelDartLegoRightLeg;
//Buzz
Model modelBuzzHead;
Model modelBuzzTorso;
Model modelBuzzLeftArm;
Model modelBuzzLeftForearm;
Model modelBuzzLeftHand;

Model modelBuzzHip;
Model modelBuzzRightArm;
Model modelBuzzRightForearm;
Model modelBuzzRightHand;

Model modelBuzzLeftCalf;
Model modelBuzzLeftFoot;
Model modelBuzzLeftThigh;
Model modelBuzzLeftWing1;
Model modelBuzzLeftWing2;

Model modelBuzzRightCalf;
Model modelBuzzRightFoot;
Model modelBuzzRightThigh;
Model modelBuzzRightWing1;
Model modelBuzzRightWing2;

//Pelota
Model modelBall;

//Librero
Model modelBooks;

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureSpongeID, textureWaterID;
GLuint skyboxTextureID;

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
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixBuzz = glm::mat4(1.0f);
glm::mat4 modelMatrixBall = glm::mat4(1.0f);
glm::mat4 modelMatrixBooks = glm::mat4(1.0f);

float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
float rotBuzzHead = 0.0, rotBuzzLeftArm = 0.0, rotBuzzLeftForearm = 0.0, rotBuzzLeftHand = 0.0, rotBuzzHip = 0.0, rotBuzzRightArm1 = 0.0,
rotBuzzRightArm2 = 0.0, rotBuzzRightForeArm = 0.0, rotBuzzRightHand = 0.0, rotBuzzLeftLeg = 0.0, rotBuzzRightLeg = 0.0, rotBuzzLeftThigh = 0.0, rotBuzzRightThigh = 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
std::vector<std::vector<float>> keyFramesDartJoints;
std::vector<std::vector<glm::mat4>> keyFramesDart;
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 20;
int numPasosDartJoints = 0;
int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;

// Joints interpolations Buzz
std::vector<std::vector<float>> keyFramesBuzzJoints;
std::vector<std::vector<glm::mat4>> keyFramesBuzz;
int indexFrameBuzzJoints = 0;
int indexFrameBuzzJointsNext = 1;
float interpolationBuzzJoints = 0.0;
int maxNumPasosBuzzJoints = 20;
int numPasosBuzzJoints = 0;
int indexFrameBuzz = 0;
int indexFrameBuzzNext = 1;
float interpolationBuzz = 0.0;
int maxNumPasosBuzz = 200;
int numPasosBuzz = 0;

// Var animate helicopter
float rotHelHelY = 0.0;
float rotHeliTra = 0.0f;

// Var animate lambo dor
int stateDoor = 1;
float dorRotCount = 0.0f;

double deltaTime;
double currTime, lastTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
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
	shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs", "../Shaders/multipleLights.fs");
	shaderMultiTextures.initialize("../Shaders/iluminacion_texture_res.vs", "../Shaders/multipleLights_multitextures.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	esfera1.init();//Inicializar y mandar todos los atributos a la tarjeta grafica
	esfera1.setShader(&shaderMulLighting);
	esfera1.setPosition(glm::vec3(5.0f, 2.0f, -1.0f));//Actualizamos la posición del modelo
	esfera1.setScale(glm::vec3(2.0f, 2.0f, 2.0f));

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	// Eclipse
	modelEclipseChasis.loadModel("../models/Eclipse/2003eclipse_chasis.obj");
	modelEclipseChasis.setShader(&shaderMulLighting);
	modelEclipseFrontalWheels.loadModel("../models/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseFrontalWheels.setShader(&shaderMulLighting);
	modelEclipseRearWheels.loadModel("../models/Eclipse/2003eclipse_rear_wheels.obj");
	modelEclipseRearWheels.setShader(&shaderMulLighting);
	// Helicopter
	modelHeliChasis.loadModel("../models/Helicopter/Mi_24_chasis.obj");
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.loadModel("../models/Helicopter/Mi_24_heli.obj");
	modelHeliHeli.setShader(&shaderMulLighting);
	modelHeliTra.loadModel("../models/Helicopter/Mi2_24_chasis.obj");
	modelHeliTra.setShader(&shaderMulLighting);
	// Lamborginhi
	modelLambo.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shaderMulLighting);

	// Buzz
	modelBuzzHead.loadModel("../models/buzz/buzzlightyHead.obj");
	modelBuzzHead.setShader(&shaderMulLighting);
	modelBuzzTorso.loadModel("../models/buzz/buzzlightyTorso.obj");;
	modelBuzzTorso.setShader(&shaderMulLighting);
	modelBuzzLeftArm.loadModel("../models/buzz/buzzlightyLeftArm.obj");;
	modelBuzzLeftArm.setShader(&shaderMulLighting);
	modelBuzzLeftForearm.loadModel("../models/buzz/buzzlightyLeftForearm.obj");;
	modelBuzzLeftForearm.setShader(&shaderMulLighting);
	modelBuzzLeftHand.loadModel("../models/buzz/buzzlightyLeftHand.obj");;
	modelBuzzLeftHand.setShader(&shaderMulLighting);

	modelBuzzHip.loadModel("../models/buzz/buzzlightyHip.obj");;
	modelBuzzHip.setShader(&shaderMulLighting);
	modelBuzzRightArm.loadModel("../models/buzz/buzzlightyRightArm.obj");;
	modelBuzzRightArm.setShader(&shaderMulLighting);
	modelBuzzRightForearm.loadModel("../models/buzz/buzzlightyRightForearm.obj");;
	modelBuzzRightForearm.setShader(&shaderMulLighting);
	modelBuzzRightHand.loadModel("../models/buzz/buzzlightyRightHand.obj");;
	modelBuzzRightHand.setShader(&shaderMulLighting);

	modelBuzzLeftCalf.loadModel("../models/buzz/buzzlightyLeftCalf.obj");
	modelBuzzLeftCalf.setShader(&shaderMulLighting);
	modelBuzzLeftThigh.loadModel("../models/buzz/buzzlightyLeftThigh.obj");
	modelBuzzLeftThigh.setShader(&shaderMulLighting);
	modelBuzzLeftFoot.loadModel("../models/buzz/buzzlightyLeftFoot.obj");
	modelBuzzLeftFoot.setShader(&shaderMulLighting);
	modelBuzzLeftWing1.loadModel("../models/buzz/buzzlightyLeftWing1.obj");
	modelBuzzLeftWing1.setShader(&shaderMulLighting);
	modelBuzzLeftWing2.loadModel("../models/buzz/buzzlightyLeftWing2.obj");
	modelBuzzLeftWing2.setShader(&shaderMulLighting);

	modelBuzzRightCalf.loadModel("../models/buzz/buzzlightyRightCalf.obj");
	modelBuzzRightCalf.setShader(&shaderMulLighting);
	modelBuzzRightThigh.loadModel("../models/buzz/buzzlightyRightThigh.obj");
	modelBuzzRightThigh.setShader(&shaderMulLighting);
	modelBuzzRightFoot.loadModel("../models/buzz/buzzlightyRightFoot.obj");
	modelBuzzRightFoot.setShader(&shaderMulLighting);
	modelBuzzRightWing1.loadModel("../models/buzz/buzzlightyRightWing1.obj");
	modelBuzzRightWing1.setShader(&shaderMulLighting);
	modelBuzzRightWing2.loadModel("../models/buzz/buzzlightyRightWing2.obj");
	modelBuzzRightWing2.setShader(&shaderMulLighting);

	// Pelota
	modelBall.loadModel("../models/Ball/pixarBall.obj");
	modelBall.setShader(&shaderMulLighting);

	// Librero
	modelBooks.loadModel("../models/Libros/BookCase.obj");
	modelBooks.setShader(&shaderMulLighting);

	boxMultiTexture.init();
	boxMultiTexture.setShader(&shaderMultiTextures);
	boxMultiTexture.setPosition(glm::vec3(0.0f, 2.0f, 4.0f));///Liberar memoria actualizar codigo
	boxMultiTexture.setScale(glm::vec3(2.0f, 2.0f, 2.0f));

	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));

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
	}

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

	// Definiendo la textura a utilizar
	Texture textureWall("../Textures/whiteWall.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureWall.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureWall.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureWallID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWallID);
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
	textureWall.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureWindow.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureWindow.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
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
	textureWindow.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureHighway.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureHighway.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
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
	textureHighway.freeImage(bitmap);

	//Crear un objeto de tipo texture
	Texture textureLandingPad("../Textures/landingPad.jpg");
	//Cargar el mapa de bits
	bitmap = textureLandingPad.loadImage();
	//Convertir el mapa de bits a un arreglo
	data = textureLandingPad.convertToData(bitmap, imageWidth, imageHeight);
	//Crear la textura con el id
	glGenTextures(1, &textureLandingPadID);
	//Se enlaza la textura a utilizar
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Configurar el filtering
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		//Transferir el arreglo data a la tarjeta gráfica
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo al cargar la textura" << "LandingPad" << std::endl;
	//Liberamos memoria
	textureLandingPad.freeImage(bitmap);

	////////////Para la textura Sponge/////////////////////////////////
	//Crear un objeto de tipo texture
	Texture textureSponge("../Textures/sponge.jpg");
	//Cargar el mapa de bits
	bitmap = textureSponge.loadImage();
	//Convertir el mapa de bits a un arreglo
	data = textureSponge.convertToData(bitmap, imageWidth, imageHeight);
	//Crear la textura con el id
	glGenTextures(1, &textureSpongeID);
	//Se enlaza la textura a utilizar
	glBindTexture(GL_TEXTURE_2D, textureSpongeID);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Configurar el filtering
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		//Transferir el arreglo data a la tarjeta gráfica
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo al cargar la textura" << "LandingPad" << std::endl;
	//Liberamos memoria
	textureSponge.freeImage(bitmap);

	//////////Para la textura water//////////////
	//Crear un objeto de tipo texture
	Texture textureWater("../Textures/water.jpg");
	//Cargar el mapa de bits
	bitmap = textureWater.loadImage();
	//Convertir el mapa de bits a un arreglo
	data = textureWater.convertToData(bitmap, imageWidth, imageHeight);
	//Crear la textura con el id
	glGenTextures(1, &textureWaterID);
	//Se enlaza la textura a utilizar
	glBindTexture(GL_TEXTURE_2D, textureWaterID);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Configurar el filtering
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		//Transferir el arreglo data a la tarjeta gráfica
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo al cargar la textura" << "LandingPad" << std::endl;
	//Liberamos memoria
	textureWater.freeImage(bitmap);

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

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	esfera1.destroy();

	// Custom objects Delete
	modelAircraft.destroy();
	modelDartLegoBody.destroy();
	modelDartLegoHead.destroy();
	modelDartLegoLeftArm.destroy();
	modelDartLegoLeftHand.destroy();
	modelDartLegoLeftLeg.destroy();
	modelDartLegoMask.destroy();
	modelDartLegoRightArm.destroy();
	modelDartLegoRightHand.destroy();
	modelDartLegoRightLeg.destroy();
	modelEclipseChasis.destroy();
	modelEclipseFrontalWheels.destroy();
	modelEclipseRearWheels.destroy();
	modelHeliChasis.destroy();
	modelHeliHeli.destroy();
	modelHeliTra.destroy();
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelRock.destroy();

	modelBuzzHead.destroy();
	modelBuzzLeftArm.destroy();
	modelBuzzLeftForearm.destroy();
	modelBuzzLeftHand.destroy();
	modelBuzzTorso.destroy();
	modelBuzzLeftCalf.destroy();
	modelBuzzLeftFoot.destroy();
	modelBuzzLeftThigh.destroy();
	modelBuzzLeftWing1.destroy();
	modelBuzzLeftWing2.destroy();
	modelBuzzRightArm.destroy();
	modelBuzzRightForearm.destroy();
	modelBuzzRightHand.destroy();
	modelBuzzRightCalf.destroy();
	modelBuzzRightFoot.destroy();
	modelBuzzRightThigh.destroy();
	modelBuzzRightWing1.destroy();
	modelBuzzRightWing2.destroy();

	modelBall.destroy();
	modelBooks.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);

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

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		enableCountSelected = false;
		modelSelected++;
		if (modelSelected > 4)
			modelSelected = 0;
		if (modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		if (modelSelected == 3)
			fileName = "../animaciones/animation_buzz_joints.txt";
		if (modelSelected == 4)
			fileName = "../animaciones/animation_buzz.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Guardar key frames
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		record = true;
		if (myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
		&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		record = false;
		myfile.close();
		if (modelSelected == 1)
			keyFramesDartJoints = getKeyRotFrames(fileName);
		if (modelSelected == 2)
			keyFramesDart = getKeyFrames(fileName);
		if (modelSelected == 3)
			keyFramesBuzzJoints = getKeyRotFrames(fileName);
		if (modelSelected == 4)
			keyFramesBuzz = getKeyFrames(fileName);
	}
	if (availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		saveFrame = true;
		availableSave = false;
	}if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	// Dart Lego model movements
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(-0.02, 0.0, 0.0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(0.02, 0.0, 0.0));

	//Movimiento del Buzz
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead -= 0.02;

	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzHip += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzHip -= 0.02;

	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzRightArm1 += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzRightArm1 -= 0.02;

	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotBuzzRightArm2 += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotBuzzRightArm2 -= 0.02;

	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotBuzzRightHand += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotBuzzRightHand -= 0.02;

	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotBuzzLeftArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotBuzzLeftArm -= 0.02;

	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotBuzzLeftForearm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotBuzzLeftForearm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		rotBuzzLeftHand += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		rotBuzzLeftHand -= 0.02;

	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		rotBuzzRightThigh += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		rotBuzzRightThigh -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		rotBuzzLeftThigh += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		rotBuzzLeftThigh -= 0.02;


	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, 0.02));
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, -0.02));

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(27.5, 0, 30.0));
	modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(180.0f), glm::vec3(0, 1, 0));
	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	float rotWheelsX = 0.0;
	float rotWheelsY = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	int stateLambo = 0;
	float advanceCountLambo = 0.0;
	float rotCountLambo = 0.0;
	float rotWheelsXLambo = 0.0;
	float rotWheelsYLambo = 0.0;
	int numberAdvanceLambo = 0;
	int maxAdvanceLambo = 0.0;

	float heliMov = 20.0f; //Variable para manejar la posicion del helicoptero
	int stateHeli = 0;
	float tiempoHelices = 30.0f;

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));

	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(10.0, 2.0, -17.5));

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(23.0, 0.0, 0.0));

	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));

	modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(3.0, 0.0, -20.0));

	modelMatrixBall = glm::translate(modelMatrixBall, glm::vec3(3.0, 1.0, -40.0));

	modelMatrixBooks = glm::translate(modelMatrixBooks, glm::vec3(5.0, 0.0, -50.0));




	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");

	//Variables para interpolar los key frames del Buzz
	keyFramesBuzzJoints = getKeyRotFrames("../animaciones/animation_buzz_joints.txt");
	keyFramesBuzz = getKeyFrames("../animaciones/animation_buzz.txt");

	lastTime = TimeManager::Instance().GetTime();

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

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;
		std::vector<float> matrixBuzzJoints;
		std::vector<glm::mat4> matrixBuzz;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

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

		//Settea la matriz de vistay projection al shader con multiples texturas
		shaderMultiTextures.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMultiTextures.setMatrix4("view", 1, false,
			glm::value_ptr(view));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		shaderMultiTextures.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMultiTextures.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMultiTextures.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMultiTextures.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMultiTextures.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 0);
		shaderMultiTextures.setInt("spotLightCount", 0);

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", 0);
		shaderMultiTextures.setInt("pointLightCount", 0);

		/*******************************************
		 * Cesped
		 *******************************************/
		glm::mat4 modelCesped = glm::mat4(1.0);
		modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
		modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
		// Se activa la textura del agua
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(200, 200)));
		boxCesped.render(modelCesped);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Casa
		 *******************************************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWallID);
		// Frontal wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 2.5, 0.0));
		boxWalls.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWalls.render();
		// Left wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-15.0, 2.5, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Right walls
		// First wall
		boxWalls.setScale(glm::vec3(3.75, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -1.875));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Second wall
		boxWalls.setScale(glm::vec3(3.75, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -8.125));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Third wall
		boxWalls.setScale(glm::vec3(2.5, 1.25, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 4.375, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Four wall
		boxWalls.setScale(glm::vec3(2.5, 1.25, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 0.625, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Back wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 2.5, -10.0));
		boxWalls.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWalls.render();
		// Ceilling
		boxWalls.setScale(glm::vec3(12.0, 12.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 5, -5.0));
		boxWalls.setOrientation(glm::vec3(90.0, 0.0, 0.0));
		boxWalls.render();
		// Window
		glBindTexture(GL_TEXTURE_2D, textureWindowID);
		boxWalls.setScale(glm::vec3(2.5, 2.5, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Highway 0
		glBindTexture(GL_TEXTURE_2D, textureHighwayID);
		boxHighway.setScale(glm::vec3(40.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(0.0, 0.05, 10.0));
		boxHighway.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxHighway.render();
		// Highway 1
		boxHighway.setScale(glm::vec3(80.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(25.0, 0.05, 0.0));
		boxHighway.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2, 1)));
		boxHighway.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		// Highway 2
		boxHighway.setScale(glm::vec3(80.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(-25.0, 0.05, 0.0));
		boxHighway.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2, 1)));
		boxHighway.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		// Highway 3
		boxHighway.setScale(glm::vec3(40.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(0.0, 0.05, -35.0));
		boxHighway.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxHighway.render();

		//Esfera 1 Render
		glBindTexture(GL_TEXTURE_2D, textureWindowID);
		glActiveTexture(GL_TEXTURE0);
		//esfera1.enableWireMode();
		esfera1.render();
		glBindTexture(GL_TEXTURE_2D, 0);

		//LandingPad render
		glBindTexture(GL_TEXTURE_2D, textureLandingPadID);
		glActiveTexture(GL_TEXTURE0);
		boxLandingPad.setScale(glm::vec3(10.0f, 0.05f, 10.0f));
		boxLandingPad.setPosition(glm::vec3(5.0f, 0.05f, -5.0f));
		boxLandingPad.render();
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureWaterID);
		shaderMultiTextures.setInt("texture1", 4);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureSpongeID);
		shaderMultiTextures.setInt("texture2", 2);
		shaderMultiTextures.setFloat("porcentajeMe", 0.3f);
		boxMultiTexture.render();

		/*******************************************
		 * Custom objects obj
		 *******************************************/
		 //Rock render
		modelRock.render(matrixModelRock);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		// Render for the aircraft model
		modelAircraft.render(modelMatrixAircraft);

		// Render de la pelota
		modelBall.render(modelMatrixBall);

		//Render del librero
		//modelMatrixBooks = glm::scale(modelMatrixBooks, glm::vec3(0.5f, 0.5f, 0.5f));
		modelBooks.render(modelMatrixBooks);

		// Render for the eclipse car
		glm::mat4 modelMatrixEclipseChasis = glm::mat4(modelMatrixEclipse);
		modelMatrixEclipseChasis = glm::scale(modelMatrixEclipse, glm::vec3(0.5, 0.5, 0.5));
		modelEclipseChasis.render(modelMatrixEclipseChasis);

		glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsY, glm::vec3(0, 1, 0));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
		modelEclipseFrontalWheels.render(modelMatrixFrontalWheels);

		glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157));
		modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
		modelEclipseRearWheels.render(modelMatrixRearWheels);

		// Helicopter
		glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
		modelHeliChasis.render(modelMatrixHeliChasis);

		glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
		modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY, glm::vec3(0, 1, 0));
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
		modelHeliHeli.render(modelMatrixHeliHeli);

		glm::mat4 modelMatrixHeliTra = glm::mat4(modelMatrixHeliChasis);
		modelMatrixHeliTra = glm::translate(modelMatrixHeliTra, glm::vec3(0.400102f, 2.08928f, -5.64672f));
		modelMatrixHeliTra = glm::rotate(modelMatrixHeliTra, glm::radians(rotHeliTra), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixHeliTra = glm::translate(modelMatrixHeliTra, glm::vec3(-0.400102f, -2.08928f, 5.64672f));
		modelHeliTra.render(modelMatrixHeliTra);

		// Lambo car
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixLamboChasis = glm::mat4(modelMatrixLambo);
		modelMatrixLamboChasis = glm::scale(modelMatrixLamboChasis, glm::vec3(1.3, 1.3, 1.3));
		modelLambo.render(modelMatrixLamboChasis);
		glActiveTexture(GL_TEXTURE0);
		glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboChasis);
		/*modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
		modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));*/
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.0867f, 0.700735f, 0.981789f));
		modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.0867f, -0.700735f, -0.981789f));
		modelLamboLeftDor.render(modelMatrixLamboLeftDor);

		modelLamboRightDor.render(modelMatrixLamboChasis);

		//Llanta delantera izquierda del Lambo
		glm::mat4 modelMatrixLamboLeftFrontWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboLeftFrontWheel = glm::translate(modelMatrixLamboLeftFrontWheel, glm::vec3(0.941901f, 0.377929f, 1.40069f));
		modelMatrixLamboLeftFrontWheel = glm::rotate(modelMatrixLamboLeftFrontWheel, -rotWheelsYLambo, glm::vec3(0, 1, 0));
		modelMatrixLamboLeftFrontWheel = glm::rotate(modelMatrixLamboLeftFrontWheel, rotWheelsXLambo, glm::vec3(1, 0, 0));
		modelMatrixLamboLeftFrontWheel = glm::translate(modelMatrixLamboLeftFrontWheel, glm::vec3(-0.941901f, -0.377929f, -1.40069f));
		modelLamboFrontLeftWheel.render(modelMatrixLamboLeftFrontWheel);

		//LLanta delantera derecha del Lambo
		glm::mat4 modelMatrixLamboRightFrontWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboRightFrontWheel = glm::translate(modelMatrixLamboRightFrontWheel, glm::vec3(-0.925465f, 0.377929f, 1.40069f));
		modelMatrixLamboRightFrontWheel = glm::rotate(modelMatrixLamboRightFrontWheel, -rotWheelsYLambo, glm::vec3(0, 1, 0));
		modelMatrixLamboRightFrontWheel = glm::rotate(modelMatrixLamboRightFrontWheel, rotWheelsXLambo, glm::vec3(1, 0, 0));
		modelMatrixLamboRightFrontWheel = glm::translate(modelMatrixLamboRightFrontWheel, glm::vec3(0.925465f, -0.377929f, -1.40069f));
		modelLamboFrontRightWheel.render(modelMatrixLamboRightFrontWheel);

		//Llanta trasera izquierda del Lambo
		glm::mat4 modelMatrixLamboLeftRearWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboLeftRearWheel = glm::translate(modelMatrixLamboLeftRearWheel, glm::vec3(-0.934885f, 0.397321f, -1.60302f));
		modelMatrixLamboLeftRearWheel = glm::rotate(modelMatrixLamboLeftRearWheel, rotWheelsXLambo, glm::vec3(1, 0, 0));
		modelMatrixLamboLeftRearWheel = glm::translate(modelMatrixLamboLeftRearWheel, glm::vec3(0.934885f, -0.397321f, 1.60302f));
		modelLamboRearLeftWheel.render(modelMatrixLamboLeftRearWheel);

		//Llanta trasera derecha del lambo
		glm::mat4 modelMatrixLamboRightRearWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboRightRearWheel = glm::translate(modelMatrixLamboRightRearWheel, glm::vec3(0.936413f, 0.397321f, -1.60302f));
		modelMatrixLamboRightRearWheel = glm::rotate(modelMatrixLamboRightRearWheel, rotWheelsXLambo, glm::vec3(1, 0, 0));
		modelMatrixLamboRightRearWheel = glm::translate(modelMatrixLamboRightRearWheel, glm::vec3(-0.936413f, -0.397321f, 1.60302f));
		modelLamboRearRightWheel.render(modelMatrixLamboRightRearWheel);
		// Se regresa el cull faces IMPORTANTE para las puertas
		glEnable(GL_CULL_FACE);

		// Dart lego
		// Se deshabilita el cull faces IMPORTANTE para la capa
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixDartBody = glm::mat4(modelMatrixDart);
		modelMatrixDartBody = glm::scale(modelMatrixDartBody, glm::vec3(0.5, 0.5, 0.5));
		modelDartLegoBody.render(modelMatrixDartBody);
		glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDartBody);
		modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
		modelDartLegoHead.render(modelMatrixDartHead);
		modelDartLegoMask.render(modelMatrixDartHead);
		glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
		modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
		glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
		modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
		glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
		modelDartLegoRightArm.render(modelMatrixDartRightArm);
		glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
		modelDartLegoRightHand.render(modelMatrixDartRightHand);
		glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
		modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
		modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
		glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
		modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
		modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);

		glm::mat4 modelMatrixBuzzBody = glm::mat4(modelMatrixBuzz);
		modelMatrixBuzzBody = glm::scale(modelMatrixBuzzBody, glm::vec3(2.5f, 2.5f, 2.5f));
		modelBuzzTorso.render(modelMatrixBuzzBody);
		glm::mat4 modelMatrixBuzzLeftArm = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm, glm::vec3(0.183784f, 0.579864f, -0.027445f));
		modelMatrixBuzzLeftArm = glm::rotate(modelMatrixBuzzLeftArm, rotBuzzLeftArm, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixBuzzLeftArm = glm::rotate(modelMatrixBuzzLeftArm, glm::radians(-65.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm, glm::vec3(-0.183784f, -0.579864f, 0.027445f));
		modelBuzzLeftArm.render(modelMatrixBuzzLeftArm);
		glm::mat4 modelMatrixBuzzLeftForearm = glm::mat4(modelMatrixBuzzLeftArm);
		modelBuzzLeftForearm.render(modelMatrixBuzzLeftForearm);
		glm::mat4 modelMatrixBuzzLeftHand = glm::mat4(modelMatrixBuzzLeftForearm);
		modelBuzzLeftHand.render(modelMatrixBuzzLeftHand);
		glm::mat4 modelMatrixBuzzHead = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzHead = glm::rotate(modelMatrixBuzzHead, rotBuzzHead, glm::vec3(0.0f, 1.0f, 0.0f));
		modelBuzzHead.render(modelMatrixBuzzHead);

		glm::mat4 modelMatrixHip = glm::mat4(modelMatrixBuzzBody);
		modelMatrixHip = glm::rotate(modelMatrixHip, rotBuzzHip, glm::vec3(0.0f, 1.0f, 0.0f));
		modelBuzzHip.render(modelMatrixHip);

		glm::mat4 modelMatrixBuzzRightArm = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm, glm::vec3(-0.179974f, 0.577592f, -0.022103f));
		modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, rotBuzzRightArm1, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, rotBuzzRightArm2, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm, glm::vec3(0.179974f, -0.577592f, 0.022103f));
		modelBuzzRightArm.render(modelMatrixBuzzRightArm);

		glm::mat4 modelMatrixBuzzRightForearm = glm::mat4(modelMatrixBuzzRightArm);
		modelMatrixBuzzRightForearm = glm::translate(modelMatrixBuzzRightForearm, glm::vec3(-0.298368f, 0.583773f, 0.008465f));
		//modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, rotBuzzRightArm1, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixBuzzRightForearm = glm::rotate(modelMatrixBuzzRightForearm, rotBuzzRightForeArm, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixBuzzRightForearm = glm::translate(modelMatrixBuzzRightForearm, glm::vec3(0.298368f, -0.583773f, -0.008465f));
		modelBuzzRightForearm.render(modelMatrixBuzzRightForearm);

		glm::mat4 modelMatrixBuzzRightHand = glm::mat4(modelMatrixBuzzRightForearm);
		modelMatrixBuzzRightHand = glm::translate(modelMatrixBuzzRightHand, glm::vec3(-0.416066f, 0.587046f, 0.076258f));
		modelMatrixBuzzRightHand = glm::rotate(modelMatrixBuzzRightHand, rotBuzzRightHand, glm::vec3(1.0f, 0.0f, 0.0f));
		//modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, rotBuzzRightArm2, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrixBuzzRightHand = glm::translate(modelMatrixBuzzRightHand, glm::vec3(0.416066f, -0.587046f, -0.076258f));
		modelBuzzRightHand.render(modelMatrixBuzzRightHand);

		//Muslo Izquierdo
		glm::mat4 modelMatrixBuzzLeftThigh = glm::mat4(modelMatrixHip);
		modelMatrixBuzzLeftThigh = glm::translate(modelMatrixBuzzLeftThigh, glm::vec3(0.05847f, 0.3973f, 0.009103f));
		modelMatrixBuzzLeftThigh = glm::rotate(modelMatrixBuzzLeftThigh, rotBuzzLeftThigh, glm::vec3(-1.0f, 0.0f, 0.0f));
		modelMatrixBuzzLeftThigh = glm::translate(modelMatrixBuzzLeftThigh, glm::vec3(-0.05847f, -0.3973f, -0.009103f));
		modelBuzzLeftThigh.render(modelMatrixBuzzLeftThigh);

		//Muslo Derecho
		glm::mat4 modelMatrixBuzzRightThigh = glm::mat4(modelMatrixHip);
		modelMatrixBuzzRightThigh = glm::translate(modelMatrixBuzzRightThigh, glm::vec3(-0.06079f, 0.3958f, 0.009026f));
		modelMatrixBuzzRightThigh = glm::rotate(modelMatrixBuzzRightThigh, rotBuzzRightThigh, glm::vec3(-1.0f, 0.0f, 0.0f));
		modelMatrixBuzzRightThigh = glm::translate(modelMatrixBuzzRightThigh, glm::vec3(0.06079f, -0.3958f, -0.009026f));
		modelBuzzRightThigh.render(modelMatrixBuzzRightThigh);

		//Pantorrilla Izquierda
		glm::mat4 modelMatrixBuzzLeftCalf = glm::mat4(modelMatrixBuzzLeftThigh);
		modelBuzzLeftCalf.render(modelMatrixBuzzLeftCalf);

		//Pantorrilla Derecha
		glm::mat4 modelMatrixBuzzRightCalf = glm::mat4(modelMatrixBuzzRightThigh);
		modelBuzzRightCalf.render(modelMatrixBuzzRightCalf);

		//Pie izquierdo
		glm::mat4 modelMatrixBuzzLeftFoot = glm::mat4(modelMatrixBuzzLeftCalf);
		modelBuzzLeftFoot.render(modelMatrixBuzzLeftFoot);

		//Pie Derecho
		glm::mat4 modelMatrixBuzzRightFoot = glm::mat4(modelMatrixBuzzRightCalf);
		modelBuzzRightFoot.render(modelMatrixBuzzRightFoot);

		//Alas Izquierdas
		glm::mat4 modelMatrixBuzzLeftWing1 = glm::mat4(modelMatrixBuzzBody);
		modelBuzzLeftWing1.render(modelMatrixBuzzLeftWing1);
		glm::mat4 modelMatrixBuzzLeftWing2 = glm::mat4(modelMatrixBuzzBody);
		modelBuzzLeftWing2.render(modelMatrixBuzzLeftWing2);

		//Alas derechas
		glm::mat4 modelMatrixBuzzRightWing1 = glm::mat4(modelMatrixBuzzBody);
		modelBuzzRightWing1.render(modelMatrixBuzzRightWing1);
		glm::mat4 modelMatrixBuzzRightWing2 = glm::mat4(modelMatrixBuzzBody);
		modelBuzzRightWing2.render(modelMatrixBuzzRightWing2);


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

		//Para salvar los frames
		if (record && modelSelected == 1) {
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);

			if (saveFrame) {
				appendFrame(myfile, matrixDartJoints);
				saveFrame = false;
			}

		}
		else if (keyFramesDartJoints.size() > 0) {
			//Para reproducir los frames
			interpolationDartJoints = numPasosDartJoints / (float)maxNumPasosDartJoints;
			numPasosDartJoints++;
			if (interpolationDartJoints > 1.0) {
				numPasosDartJoints = 0;
				interpolationDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}

			if (indexFrameDartJointsNext > keyFramesDartJoints.size() - 1)
				indexFrameDartJointsNext = 0;

			rotDartHead = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 6, interpolationDartJoints);

		}
		if (record && modelSelected == 2) {
			matrixDart.push_back(modelMatrixDart);
			if (saveFrame) {
				appendFrame(myfile, matrixDart);
				saveFrame = false;
			}

		}
		else if (keyFramesDart.size() > 0) {
			interpolationDart = numPasosDart / (float)maxNumPasosDart;
			numPasosDart++;
			if (interpolationDart > 1.0) {
				numPasosDart = 0;
				interpolationDart = 0;
				indexFrameDart = indexFrameDartNext;
				indexFrameDartNext++;
			}
			if (indexFrameDartNext > keyFramesDart.size() - 1)
				indexFrameDartNext = 0;
			modelMatrixDart = interpolate(keyFramesDart, indexFrameDart, indexFrameDartNext, 0, interpolationDart);

		}

		// ************************ Frames para el Buzz
		// Para salvar el frame
		if (record && modelSelected == 3) {
			matrixBuzzJoints.push_back(rotBuzzHead);
			matrixBuzzJoints.push_back(rotBuzzHip);
			matrixBuzzJoints.push_back(rotBuzzRightArm1);
			matrixBuzzJoints.push_back(rotBuzzRightArm2);
			matrixBuzzJoints.push_back(rotBuzzRightForeArm);
			matrixBuzzJoints.push_back(rotBuzzRightHand);
			if (saveFrame) {
				appendFrame(myfile, matrixBuzzJoints);
				saveFrame = false;
			}
		}
		else if (keyFramesBuzzJoints.size() > 0) {
			// Para reproducir el frame
			interpolationBuzzJoints = numPasosBuzzJoints / (float)maxNumPasosBuzzJoints;
			numPasosBuzzJoints++;
			if (interpolationBuzzJoints > 1.0) {
				numPasosBuzzJoints = 0;
				interpolationBuzzJoints = 0;
				indexFrameBuzzJoints = indexFrameBuzzJointsNext;
				indexFrameBuzzJointsNext++;
			}
			if (indexFrameBuzzJointsNext > keyFramesBuzzJoints.size() - 1)
				indexFrameBuzzJointsNext = 0;

			rotBuzzHead = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 0, interpolationBuzzJoints);
			rotBuzzHip = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 1, interpolationBuzzJoints);
			rotBuzzRightArm1 = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 2, interpolationBuzzJoints);
			rotBuzzRightArm2 = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 3, interpolationBuzzJoints);
			rotBuzzRightForeArm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 4, interpolationBuzzJoints);
			rotBuzzRightHand = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 5, interpolationBuzzJoints);
		}


		// ************************* //

		//Movimiento helices copia
		//// Constantes de animaciones
		//rotHelHelY += 0.5;
		//rotHeliTra += 0.2;

		/***********MAQUINAS DE ESTADO***************/

		switch (state) {
		case 0:
			if (numberAdvance == 0)
				maxAdvance = 65.0;
			else if (numberAdvance == 1)
				maxAdvance = 49.0;
			else if (numberAdvance == 2)
				maxAdvance = 44.5;
			else if (numberAdvance == 3)
				maxAdvance = 49.0;
			else if (numberAdvance == 4)
				maxAdvance = 44.5;
			state = 1;
			break;

		case 1:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, 0.1f));
			//modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0f, 0.0f, 0.1f));

			advanceCount += 0.1f;
			rotWheelsX += 0.05f;
			rotWheelsY -= 0.02f;
			if (rotWheelsY < 0)
				rotWheelsY = 0;
			if (advanceCount > maxAdvance) {
				advanceCount = 0;
				numberAdvance++;
				state = 2;
			}
			break;

		case 2:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, 0.025f));
			modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
			rotCount += 0.5f;
			rotWheelsX += 0.05f;
			rotWheelsY += 0.02f;
			if (rotWheelsY > 0.25)
				rotWheelsY = 0.25;

			if (rotCount > 90.0) {
				rotCount = 0.0f;
				state = 0;
				if (numberAdvance > 4)
					numberAdvance = 1;
			}
			break;


		}



		//Máquina de estados helicoptero
		switch (stateHeli) {
		case 0:
			if (heliMov > 0) {
				modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(0.0f, -0.025f, 0.0f));
				heliMov -= 0.05;
				// Constantes de animaciones
				rotHelHelY += 0.5;
				rotHeliTra += 8.0;

			}
			else {
				stateHeli = 1;
			}
			break;

		case 1:
			if (tiempoHelices > 0.0f) {
				// Constantes de animaciones
				rotHelHelY += 0.15;
				rotHeliTra += 3.0;
				tiempoHelices -= 0.05;
			}
			else {
				stateHeli = 2;
				tiempoHelices = 20.0f;
			}
			break;
		case 2:
			if (tiempoHelices > 0.0f) {
				// Constantes de animaciones
				rotHelHelY += 0.1;
				rotHeliTra += 1.0;
				tiempoHelices -= 0.05;
			}
			else {
				stateHeli = 3;
				tiempoHelices = 20.0f;
			}
			break;
		case 3:
			if (tiempoHelices > 0.0f) {
				// Constantes de animaciones
				rotHelHelY += 0.05;
				rotHeliTra += 0.5;
				tiempoHelices -= 0.05;
			}
			else {
				stateHeli = 4;
				tiempoHelices = 20.0f;
			}
			break;
		case 4:
			if (tiempoHelices > 0.0f) {
				// Constantes de animaciones
				rotHelHelY += 0.025;
				rotHeliTra += 0.25;
				tiempoHelices -= 0.05;
			}
			else {
				stateHeli = 5;
				tiempoHelices = 15.0f;
			}
			break;
		case 5:
			if (tiempoHelices > 0.0f) {
				// Constantes de animaciones
				rotHelHelY += 0.01;
				rotHeliTra += 0.05;
				tiempoHelices -= 0.05;
			}
			else {
				stateHeli = 6;
				tiempoHelices = 10.0f;
			}
			break;
		}

		//Maquina de estados de Lambo
		switch (stateLambo) {
		case 0:
			if (numberAdvanceLambo == 0)
				maxAdvanceLambo = 5.0;
			else if (numberAdvanceLambo == 1)
				maxAdvanceLambo = 39.0;
			else if (numberAdvanceLambo == 2)
				maxAdvanceLambo = 34.5;
			else if (numberAdvanceLambo == 3)
				maxAdvanceLambo = 39.0;
			else if (numberAdvanceLambo == 4)
				maxAdvanceLambo = 34.5;
			stateLambo = 1;
			break;

		case 1:
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0f, 0.0f, 0.1f));

			advanceCountLambo += 0.1f;
			rotWheelsXLambo += 0.05f;
			rotWheelsYLambo -= 0.02f;
			if (rotWheelsYLambo < 0)
				rotWheelsYLambo = 0.0;
			if (advanceCountLambo > maxAdvanceLambo) {
				advanceCountLambo = 0;
				numberAdvanceLambo++;
				stateLambo = 2;
			}
			break;

		case 2:
			if (numberAdvanceLambo > 4) {
				stateLambo = 3;
				break;
			}
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0f, 0.0f, 0.025f));
			modelMatrixLambo = glm::rotate(modelMatrixLambo, glm::radians(-0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
			rotCountLambo += 0.5f;
			rotWheelsXLambo += 0.05f;
			rotWheelsYLambo += 0.02f;
			if (rotWheelsYLambo > 0.25)
				rotWheelsYLambo = 0.25;

			if (rotCountLambo > 90.0) {
				rotCountLambo = 0.0f;
				stateLambo = 0;
				if (numberAdvanceLambo > 4) {
					numberAdvanceLambo = 1;
				}
			}
			break;

		case 3:
			//Máquina de estados para la puerta del coche

			switch (stateDoor) {
			case 1:
				dorRotCount += 0.5f;
				if (dorRotCount > 75.0f)
					stateDoor = 2;
				break;

			case 2:
				dorRotCount -= 0.5f;
				if (dorRotCount < 0.0f) {
					dorRotCount = 0.0f;
					stateDoor = 3;
				}
				break;

			}

		}



		glfwSwapBuffers(window);
	}
}

int main(int argc, char** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
