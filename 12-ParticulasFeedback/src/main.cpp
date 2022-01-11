#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <sstream>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

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
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//Font rendering
#include "Headers/FontTypeRendering.h"

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

// Include musica de fondo
#include "Headers/irrKlang.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);

// Models complex instances
//Rocas
Model modelRoca;
Model modelRoca2;

//Model arbol autumn
Model modelAutumnTree;

//Model arbol frondoso 
Model modelArbolFrondoso;

//Dulces para la velocidad 
Model modelBasicCandy;
Model modelColorBomb;
Model modelLolipop;

//Radish para bajar velocidad solo hay 5 en todo el mapa
Model modelRadish;

//Calaverita
Model modelCalaverita;

//Modelo de la casita
Model modelCasa;

// Lamps
Model modelLampMed;

// Hierbas
Model modelMultipleGrass;
// Fountain
Model modelFountain;
// Model animate instance
// Boy personaje principal
Model boyModelAnimate;
//Spook
Model spookAnimate;

//variables para la musica de fondo
bool musicFondo = true;

using namespace irrklang;

// Terrain model instance
Terrain terrain(-1, -1, 200, 8, "../Textures/hmapP.png");

GLuint textureCespedID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;

FontTypeRendering::FontTypeRendering *modelText, *modelTimer;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/forest/skybox_ft.tga",
		"../Textures/forest/skybox_bk.tga",
		"../Textures/forest/skybox_up.tga",
		"../Textures/forest/skybox_dn.tga",
		"../Textures/forest/skybox_rt.tga",
		"../Textures/forest/skybox_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 matrixModelRoca = glm::mat4(1.0);
glm::mat4 modelMatrixBoy = glm::mat4(1.0f);
glm::mat4 modelMatrixSpook = glm::mat4(1.0f);
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);
glm::mat4 modelMatrixLolipop = glm::mat4(1.0f);
glm::mat4 modelMatrixBomb = glm::mat4(1.0f);
glm::mat4 modelMatrixCandy = glm::mat4(1.0f);

int animationIndex = 0;
int modelSelected = 2;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Lamps positions
std::vector<glm::vec3> lampMedPosition = { glm::vec3(-4.88, 0.0, 20.11), glm::vec3(-15.43, 0.0, -5.07), glm::vec3(-26.75, 0.0, 12.7), glm::vec3(-25.58, 0.0, -32.61),
			glm::vec3(-6.64, 0.0, -61.13), glm::vec3(10.35, 0.0, -26.56), glm::vec3(41.4, 0.0, -53.51), glm::vec3(10.74, 0.0, 31.83), glm::vec3(16.24, 0.0, 56.31),
			glm::vec3(38.86, 0.0, 28.51), glm::vec3(60.15, 0.0, 33.2), glm::vec3(75.97, 0.0, -0.97), glm::vec3(-5.07, 0.0, 75.97), glm::vec3(-22.46, 0.0, 88.28), 
			glm::vec3(-39.64, 0.0, 54.88), glm::vec3(-64.84, 0.0, -39.64)};

std::vector<float> lampMedOrientation = {-20, 20, -20, 20, -20, 20, -20, 20, -20, 20, -20, 20, -20, 20, -20, 20};

//Pastos position
std::vector<glm::vec3> pastoMultiplePosition = { glm::vec3(-7.5, 0.0, -50.5), glm::vec3(37.0, 0.0, -30.0), glm::vec3(-3.5, 0.0, 20.5),
										glm::vec3(-27.0, 0.0, 67.0), glm::vec3(-1.5, 0.0, 1.5) };
//Pastos orientation
std::vector<float> pastoMultipleOrientation = { 25.0, 45.0, 0.0, -90.0, -12.0 };

//Roca 2 position
std::vector<glm::vec3> roca2Position = { glm::vec3(-49.21,0.0, -84.37), glm::vec3(35.5, 0.0, -6.38), glm::vec3(48.82, 0.0, -9.76),
										glm::vec3(55.66, 0.0, 82.03), glm::vec3(65.23, 0.0, -25.59) };
//Roca 2 orientation
std::vector<float> roca2Orientation = { 3.0, 15.0, -87.0, 127.0, -120.0 };

//Autumn tree position
std::vector<glm::vec3> autumnPosition = { glm::vec3(-92.19, 0.0, -94.4), glm::vec3(3.12, 0.0, -95.1), glm::vec3(75.01, 0.0, 94.9), glm::vec3(-87.0, 0.0, -62.9),
						glm::vec3(-37.5, 0.0, -59.37), glm::vec3(12.2, 0.0, -51.07), glm::vec3(92.19, 0.0, 94.4), glm::vec3(-3.12, 0.0, 95.1), glm::vec3(-75.01, 0.0, -94.9),
						glm::vec3(87.0, 0.0, 62.9), glm::vec3(37.5, 0.0, 59.37), glm::vec3(-12.2, 0.0, 51.07) };
//Autumn tree orientation
std::vector<float> autumnOrientation = { -12.0, 45.0, 30.0, -45.0, -25.0, 90.0, -12.0, -57.0, -35.0, -5.0, 25.0, 93.0 };

//Arbol frondoso position
std::vector<glm::vec3> arbFronPosition = { glm::vec3(-87.19, 1.3, -89.4), glm::vec3(-3.12, 1.3, -89.1), glm::vec3(65.01, 1.3, 88.9), glm::vec3(-80.0, 1.3, -68.9),
						 glm::vec3(87.19, 1.3, 89.4), glm::vec3(3.12, 1.3, 89.1), glm::vec3(-65.01, 1.3, -88.9), glm::vec3(80.0, 1.3, 68.9) };
//Arbol frondoso orientation
std::vector<float> arbFronOrientation = { -15.0, 45.0, 36.0, -45.0, -25.0, 70.0, -12.0, -53.0, -37.0, -5.0, 28.0, 67.0 };

//Dulces
//Basic Candy
std::vector<glm::vec3> basCandyPosition = { glm::vec3(-24.21, 5.5, 24.21),glm::vec3(-14.25, 5.5, 7.22), glm::vec3(-24.02, 5.5, -17.38), glm::vec3(-9.57, 5.5, -31.44) };
std::vector<float> basCandyOrientation = { 30.0, 45.0, 60.0, 75.0 };

//Color bomb
std::vector<glm::vec3> colBombPosition = { glm::vec3(30.66, 0.9, -44.72), glm::vec3(59.37, 0.9, -48.43), glm::vec3(22.07, 0.9, 34.76), glm::vec3(41.8, 0.9, 37.9) };
std::vector<float> colBombOrientation = { 30.0, 45.0, 60.0, 75.0 };

//Lolipop
std::vector<glm::vec3> lolipopPosition = { glm::vec3(67.18, 0.0, 15.23), glm::vec3(76.95, 0.0, 47.65), glm::vec3(-12.5, 0.0, 5.86), glm::vec3(-27.93, 0.0, 61.13) };
std::vector<float> lolipopOrientation = { 180, 180, -180, -180 };

//Radish RadishPosition
std::vector<glm::vec3> RadishPosition = { glm::vec3(-13.28, 0.0, 31.83), glm::vec3(9.37, 0.0, 42.96), glm::vec3(-13.28, 0.0, 72.46), glm::vec3(-11.91, 0.0, -35.35),
										glm::vec3(68.75, 0.0, 22.65) };
std::vector<float> RadishOrientation = { 180, -180, 180, -180, -180 };

//Calaverita de Azucar 5
std::vector<glm::vec3> calavPosition = { glm::vec3(-65.82, -0.5, -60.15), glm::vec3(57.03, -0.5, -67.18), glm::vec3(11.32, -0.5, 13.28), glm::vec3(-61.32, -0.5, -20.11),
										glm::vec3(-60.54, -0.5, 46.48) };
std::vector<float> calavOrientation = { 45.0, -45.0, 30.0, -30.0, 60.0 };

//Casita
std::vector<glm::vec3> casaPosition = { glm::vec3(23.57, 0.0, 47.64), glm::vec3(-42.57, 0.0, 39.45) };
std::vector<float> casaOrientation = { 233.55, 37.41 };

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"fountain", glm::vec3(5.0, 0.0, -40.0)},
		{"fire", glm::vec3(0.0, 0.0, 7.0)}
};

double deltaTime;
double currTime, lastTime;
double seg, min;

//Variables para cuando se recogen dulces y vegetales
bool candyCollider = false;
bool veggieCollider = false;
bool spookHelp = false;
bool enterPress = false;
bool calaveritaAzucarCollider = false;
std::string elemento;
int numElemento = -1;

//contador para calaveras y dulces
int countCalav = 0, countCandy = 0;
float velocidad = 0.02;

//musica
ISoundEngine* engine = createIrrKlangDevice();

//if (!engine)
	//return 0;

ISound* ambiental = engine->play2D("../sounds/song.wav", true);

// Jump variables
bool isJump = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.5, particleLifetime = 3.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for (unsigned int i = 0; i < nParticlesFire; i++) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine{};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
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
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_fog.vs", "../Shaders/multipleLights_fog.fs");
	shaderTerrain.initialize("../Shaders/terrain_fog.vs", "../Shaders/terrain_fog.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", { "Position", "Velocity", "Age" });

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

	modelRoca.loadModel("../models/roca/roca.obj");
	modelRoca.setShader(&shaderMulLighting);

	modelRoca2.loadModel("../models/roca2/roca2.obj");
	modelRoca2.setShader(&shaderMulLighting);

	modelBasicCandy.loadModel("../models/BasicCandy/BasicCandy.obj");
	modelBasicCandy.setShader(&shaderMulLighting);

	modelColorBomb.loadModel("../models/ColorBomb/ColorBomb.obj");
	modelColorBomb.setShader(&shaderMulLighting);

	modelLolipop.loadModel("../models/LolipopHammer/LolipopHammer.obj");
	modelLolipop.setShader(&shaderMulLighting);

	modelRadish.loadModel("../models/radish/radish.obj");
	modelRadish.setShader(&shaderMulLighting);

	modelCalaverita.loadModel("../models/calaveritaAzucar/calaveritaAzucar.obj");
	modelCalaverita.setShader(&shaderMulLighting);

	modelCasa.loadModel("../models/casaMedieval/casaMed.obj");
	modelCasa.setShader(&shaderMulLighting);

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	//Lamp models
	modelLampMed.loadModel("../models/lamparaMed/lamparaMed.obj");
	modelLampMed.setShader(&shaderMulLighting);

	//Model arbol autumn tree
	modelAutumnTree.loadModel("../models/Autumn Tree/Autumn Tree.obj");
	modelAutumnTree.setShader(&shaderMulLighting);

	//Model modelArbolFrondoso
	modelArbolFrondoso.loadModel("../models/pino/pino.obj");
	modelArbolFrondoso.setShader(&shaderMulLighting);

	//Grass
	modelMultipleGrass.loadModel("../models/Pastos/multipleGrass.obj");
	modelMultipleGrass.setShader(&shaderMulLighting);

	//Fountain
	modelFountain.loadModel("../models/Fountain/fountain.obj");
	modelFountain.setShader(&shaderMulLighting);

	//Boy
	boyModelAnimate.loadModel("../models/boy/boyAnimation.fbx");
	boyModelAnimate.setShader(&shaderMulLighting);

	//Boy
	spookAnimate.loadModel("../models/spook/spook.fbx");
	spookAnimate.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

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
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
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
	Texture textureCesped("../Textures/grassP.png");
	bitmap = textureCesped.loadImage();
	data = textureCesped.convertToData(bitmap, imageWidth,
		imageHeight);
	glGenTextures(1, &textureCespedID);
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCesped.freeImage(bitmap);


	// Definiendo la textura a utilizar
	Texture textureTerrainBackground("../Textures/grassP.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainR("../Textures/blooms.png");
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
	Texture textureTerrainG("../Textures/asred.png");
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
	Texture textureTerrainB("../Textures/streets.png");
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
	Texture textureTerrainBlendMap("../Textures/bmapP.png");
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

	Texture textureParticlesFountain("../Textures/aguaFuente.png");
	bitmap = textureParticlesFountain.loadImage();
	data = textureParticlesFountain.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFountainID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticlesFountain.freeImage(bitmap);

	Texture textureParticleFire("../Textures/fuego.png");
	bitmap = textureParticleFire.loadImage();
	data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for (int i = 0; i < randData.size(); i++) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f, 0.1f, 0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0, 1, 0);
	n = glm::cross(glm::vec3(1, 0, 0), v);
	if (glm::length(n) < 0.00001f) {
		n = glm::cross(glm::vec3(0, 1, 0), v);
	}
	u = glm::cross(v, n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	//Se inicializa el model de texeles para dibujar texto
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelText->Initialize();
	modelTimer = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelTimer->Initialize();
	/*******************************************
	 * Inicializacion de los buffers de la fuente
	 *******************************************/
	initParticleBuffers();

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffersFire();
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
	shaderParticlesFountain.destroy();
	//shaderParticlesFire.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Custom objects Delete
	modelRoca.destroy();
	modelRoca2.destroy();
	modelBasicCandy.destroy();
	modelColorBomb.destroy();
	modelLolipop.destroy();
	modelRadish.destroy();
	modelCalaverita.destroy();
	modelCasa.destroy();
	modelAutumnTree.destroy();
	modelArbolFrondoso.destroy();
	modelLampMed.destroy();
	modelMultipleGrass.destroy();
	modelFountain.destroy();

	// Custom objects animate
	boyModelAnimate.destroy();
	spookAnimate.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fountain particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
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

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		enableCountSelected = false;
		modelSelected++;
		if (modelSelected > 2)
			modelSelected = 0;
		if (modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
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
	}
	if (availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		saveFrame = true;
		availableSave = false;
	}if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		modelMatrixBoy = glm::rotate(modelMatrixBoy, glm::radians(2.0f), glm::vec3(0, 1, 0));
		boyModelAnimate.setAnimationIndex(animationIndex);
	}
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		modelMatrixBoy = glm::rotate(modelMatrixBoy, glm::radians(-2.0f), glm::vec3(0, 1, 0));
		boyModelAnimate.setAnimationIndex(animationIndex);
	}if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		modelMatrixBoy = glm::translate(modelMatrixBoy, glm::vec3(0, 0, 0.02));
		boyModelAnimate.setAnimationIndex(animationIndex);
	}
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		modelMatrixBoy = glm::translate(modelMatrixBoy, glm::vec3(0, 0, -0.02));
		boyModelAnimate.setAnimationIndex(animationIndex);
	} 
	else if ((modelSelected == 2 && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)) {
		enterPress = true;
	}

	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if (!isJump && keySpaceStatus) {
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
	}

	glfwPollEvents();
	return continueApplication;
}


int colisionesObjetos(std::string tipoElemento) {
	tipoElemento = elemento;
	char numC = tipoElemento.back();
	int num = numC - 48;
	std::cout << "Numero " << num << std::endl;
	return num;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	matrixModelRoca = glm::translate(matrixModelRoca, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixBoy = glm::translate(modelMatrixBoy, glm::vec3(11.32f, 0.05f, 51.96f));
	modelMatrixBoy = glm::rotate(modelMatrixBoy, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixSpook = glm::translate(modelMatrixSpook, glm::vec3(2.54, 0.0, 64.45));
	modelMatrixSpook = glm::rotate(modelMatrixSpook, glm::radians(246.7f), glm::vec3(0, 1, 0));

	modelMatrixFountain = glm::translate(modelMatrixFountain, glm::vec3(5.0, 0.0, -40.0));
	modelMatrixFountain[3][1] = terrain.getHeightTerrain(modelMatrixFountain[3][0], modelMatrixFountain[3][2]) + 0.2;
	modelMatrixFountain = glm::scale(modelMatrixFountain, glm::vec3(10.0f, 10.0f, 10.0f));

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

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

		glm::vec3 camera_pos = camera->getPosition();

		std::map<std::string, bool> collisionDetection;


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);

		if (modelSelected == 1) {
			//camara primera
		}
		else {
			axis = glm::axis(glm::quat_cast(modelMatrixBoy));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixBoy));
			target = modelMatrixBoy[3];
		}

		if (std::isnan(angleTarget))
			angleTarget = 0.0;
		if (axis.y < 0)
			angleTarget = -angleTarget;
		if (modelSelected == 1)
			angleTarget -= glm::radians(90.0f);
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
		// Settea la matriz de vista y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setFloat("density", 0.04);
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setFloat("density", 0.04);
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderSkybox.setFloat("lowerLimit", 0.0);
		shaderSkybox.setFloat("upperLimit", 0.04);

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.05, 0.05, 0.05)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, -1.0, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.05, 0.05, 0.05)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		glm::vec3 spotPosition = glm::vec3(0.32437, 0.226053, 1.79149);
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderTerrain.setInt("spotLightCount", 1);
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.3, 0.2)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.074);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.03);
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.3, 0.2)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		shaderTerrain.setFloat("spotLights[0].linear", 0.074);
		shaderTerrain.setFloat("spotLights[0].quadratic", 0.03);
		shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", lampMedPosition.size());
		shaderTerrain.setInt("pointLightCount", lampMedPosition.size());
		for (int i = 0; i < lampMedPosition.size(); i++) {
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lampMedPosition[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lampMedOrientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.5, 0.5, 0.5));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0, 6.8, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.3, 0.26, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.46, 0.35, 0.04)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.956, 0.913, 0.243)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.3, 0.26, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.46, 0.35, 0.04)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.956, 0.913, 0.243)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
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
		// Se activa la textura de tierra
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
		shaderTerrain.setInt("rTexture", 1);
		// Se activa la textura de hierba
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
		shaderTerrain.setInt("gTexture", 2);
		// Se activa la textura del camino
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
		shaderTerrain.setInt("bTexture", 3);
		// Se activa la textura del blend map
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
		shaderTerrain.setInt("blendMapTexture", 4);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
		terrain.render();
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Custom objects obj
		 *******************************************/
		 //Rock render
		matrixModelRoca[3][1] = terrain.getHeightTerrain(matrixModelRoca[3][0], matrixModelRoca[3][2]);
		modelRoca.render(matrixModelRoca);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		//Render arbol autumnn modelAutumnTree
		for (int i = 0; i < autumnPosition.size(); i++) {
			autumnPosition[i].y = terrain.getHeightTerrain(autumnPosition[i].x, autumnPosition[i].z);
			modelAutumnTree.setPosition(autumnPosition[i]);
			modelAutumnTree.setScale(glm::vec3(0.15, 0.15, 0.15));
			modelAutumnTree.setOrientation(glm::vec3(0, autumnOrientation[i], 0));
			modelAutumnTree.render();
		}

		//Render arbol frondoso 
		for (int i = 0; i < arbFronPosition.size(); i++) {
			arbFronPosition[i].y = terrain.getHeightTerrain(arbFronPosition[i].x, arbFronPosition[i].z);
			modelArbolFrondoso.setPosition(glm::vec3(arbFronPosition[i].x, 0.3, arbFronPosition[i].z));
			modelArbolFrondoso.setScale(glm::vec3(2.5, 2.5, 2.5));
			modelArbolFrondoso.setOrientation(glm::vec3(0, arbFronOrientation[i], 0));
			modelArbolFrondoso.render();
		}

		//Render de los dulces
		//Basic Candy
		for (int i = 0; i < basCandyPosition.size(); i++) {
			basCandyPosition[i].y = terrain.getHeightTerrain(basCandyPosition[i].x, basCandyPosition[i].z);
			if (candyCollider == true && i == numElemento) {
				modelBasicCandy.setPosition(glm::vec3(basCandyPosition[i].x, 5.2, basCandyPosition[i].z));
				countCandy++;
				velocidad += 0.01;
				candyCollider = false;
			}
			else {
				modelBasicCandy.setPosition(glm::vec3(basCandyPosition[i].x, 1.2, basCandyPosition[i].z));
			}
			
			modelBasicCandy.setScale(glm::vec3(0.0015, 0.0015, 0.0015));
			modelBasicCandy.setOrientation(glm::vec3(basCandyOrientation[i], 0, 0));
			modelBasicCandy.render();
		}

		//Color bomb
		for (int i = 0; i < colBombPosition.size(); i++) {
			colBombPosition[i].y = terrain.getHeightTerrain(colBombPosition[i].x, colBombPosition[i].z);
			if (candyCollider == true && i == numElemento) {
				modelColorBomb.setPosition(glm::vec3(colBombPosition[i].x, -5.9, colBombPosition[i].z));
				countCandy++;
				velocidad += 0.01;
				candyCollider = false;
			}
			else {
				modelColorBomb.setPosition(glm::vec3(colBombPosition[i].x, 0.9, colBombPosition[i].z));
			}
			modelColorBomb.setScale(glm::vec3(0.065, 0.065, 0.065));
			modelColorBomb.setOrientation(glm::vec3(colBombOrientation[i], 0, 0));
			modelColorBomb.render();
		}

		//Lolipop hammer
		for (int i = 0; i < lolipopPosition.size(); i++) {
			lolipopPosition[i].y = terrain.getHeightTerrain(lolipopPosition[i].x, lolipopPosition[i].z);
			if (candyCollider == true && i == numElemento) {
				modelLolipop.setPosition(glm::vec3(lolipopPosition[i].x, -5.5, lolipopPosition[i].z));
				countCandy++;
				velocidad += 0.01;
				candyCollider = false;
			}
			else {
				modelLolipop.setPosition(glm::vec3(lolipopPosition[i].x, 0.5, lolipopPosition[i].z));
			}
			modelLolipop.setScale(glm::vec3(0.03, 0.03, 0.03));
			modelLolipop.setOrientation(glm::vec3(0, lolipopOrientation[i], 0));
			modelLolipop.render();
		}

		//Radish render
		for (int i = 0; i < RadishPosition.size(); i++) {
			RadishPosition[i].y = terrain.getHeightTerrain(RadishPosition[i].x, RadishPosition[i].z);
			if (veggieCollider == true && i == numElemento) {
				modelRadish.setPosition(glm::vec3(RadishPosition[i].x, -5.5, RadishPosition[i].z));
				countCalav++;
				velocidad -= 0.01;
				veggieCollider = false;
			}
			else {
				modelRadish.setPosition(glm::vec3(RadishPosition[i].x, 0.5, RadishPosition[i].z));
			}
			modelRadish.setScale(glm::vec3(0.3, 0.3, 0.3));
			modelRadish.setOrientation(glm::vec3(0, RadishOrientation[i], 0));
			modelRadish.render();
		}
		/*************CUENTA DE DULCES ***********/
		if ((countCandy >= 0) && (countCandy < 5))
			animationIndex = 1;
		if ((countCandy >= 5) && (countCandy < 9))
			animationIndex = 2;
		if ((countCandy >= 9) && (countCandy < 13))
			animationIndex = 3;

		//Calaverita render
		for (int i = 0; i < calavPosition.size(); i++) {
			calavPosition[i].y = terrain.getHeightTerrain(calavPosition[i].x, RadishPosition[i].z);
			if (calaveritaAzucarCollider == true && i == numElemento) {
			modelCalaverita.setPosition(glm::vec3(calavPosition[i].x, 5.4, calavPosition[i].z));
			} else {
				modelCalaverita.setPosition(glm::vec3(calavPosition[i].x, 0.4, calavPosition[i].z));
			}
			modelCalaverita.setScale(glm::vec3(0.02, 0.02, 0.02));
			modelCalaverita.setOrientation(glm::vec3(0, calavOrientation[i], 0));
			modelCalaverita.render();
		}
		//Casa render
		for (int i = 0; i < casaPosition.size(); i++) {
			lolipopPosition[i].y = terrain.getHeightTerrain(casaPosition[i].x, casaPosition[i].z);
			modelCasa.setPosition(casaPosition[i]);
			modelCasa.setScale(glm::vec3(2500, 2500, 2500));
			modelCasa.setOrientation(glm::vec3(0, casaOrientation[i], 0));
			modelCasa.render();
		}

		// Render the lamps
		for (int i = 0; i < lampMedPosition.size(); i++) {
			lampMedPosition[i].y = terrain.getHeightTerrain(lampMedPosition[i].x, lampMedPosition[i].z);
			modelLampMed.setPosition(lampMedPosition[i]);
			modelLampMed.setScale(glm::vec3(0.5, 0.5, 0.5));
			modelLampMed.setOrientation(glm::vec3(0, lampMedOrientation[i], 0));
			modelLampMed.render();
		}

	
		//Render de la roca 2
		for (int i = 0; i < roca2Position.size(); i++) {
			roca2Position[i].y = terrain.getHeightTerrain(roca2Position[i].x, roca2Position[i].z);
			modelRoca2.setPosition(roca2Position[i]);
			modelRoca2.setScale(glm::vec3(0.5, 0.5, 0.5));
			modelRoca2.setOrientation(glm::vec3(0, roca2Orientation[i], 0));
			modelRoca2.render();
		}

		//Render del pasto multiple
		glDisable(GL_CULL_FACE);
		for (int i = 0; i < pastoMultiplePosition.size(); i++) {
			pastoMultiplePosition[i].y = terrain.getHeightTerrain(pastoMultiplePosition[i].x, pastoMultiplePosition[i].z);
			modelMultipleGrass.setPosition(pastoMultiplePosition[i]);
			modelMultipleGrass.setScale(glm::vec3(0.3, 0.3, 0.3));
			modelMultipleGrass.setOrientation(glm::vec3(0, pastoMultipleOrientation[i], 0));
			modelMultipleGrass.render();
		}
		glEnable(GL_CULL_FACE);

		// Fountain
		glDisable(GL_CULL_FACE);
		modelFountain.render(modelMatrixFountain);
		modelFountain.setScale(glm::vec3(0.5, 0.5, 0.5));
		glEnable(GL_CULL_FACE);


		/*******************************************
		 * Custom Anim objects obj
		 *******************************************/
		modelMatrixBoy[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixBoy[3][0], modelMatrixBoy[3][2]);
		tmv = currTime - startTimeJump;
		if (modelMatrixBoy[3][1] < terrain.getHeightTerrain(modelMatrixBoy[3][0], modelMatrixBoy[3][2])) {
			isJump = false;
			modelMatrixBoy[3][1] = terrain.getHeightTerrain(modelMatrixBoy[3][0], modelMatrixBoy[3][2]);
		}
		
		glm::mat4 modelMatrixBodyBody = glm::mat4(modelMatrixBoy);
		modelMatrixBodyBody = glm::scale(modelMatrixBodyBody, glm::vec3(0.005, 0.005, 0.005));
		boyModelAnimate.render(modelMatrixBodyBody);
		boyModelAnimate.setAnimationIndex(0);

		glm::mat4 modelMatrixBodySpook = glm::mat4(modelMatrixSpook);
		modelMatrixBodySpook = glm::scale(modelMatrixBodySpook, glm::vec3(0.015, 0.015, 0.015));
		spookAnimate.render(modelMatrixBodySpook);

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
		 /*
		 // Update the aircraft
		blendingUnsorted.find("aircraft")->second = glm::vec3(modelMatrixAircraft[3]);
		// Update the helicopter
		blendingUnsorted.find("heli")->second = glm::vec3(modelMatrixHeli[3]);
		*/
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

			if (it->second.first.compare("fountain") == 0) {
				/**********
				 * Init Render particles systems
				 */
				glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0);
				modelMatrixParticlesFountain = glm::translate(modelMatrixParticlesFountain, it->second.second);
				modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(modelMatrixParticlesFountain[3][0], modelMatrixParticlesFountain[3][2]) + 0.36 * 10.0;
				modelMatrixParticlesFountain = glm::scale(modelMatrixParticlesFountain, glm::vec3(3.0, 3.0, 3.0));
				currTimeParticlesAnimation = TimeManager::Instance().GetTime();
				if (currTimeParticlesAnimation - lastTimeParticlesAnimation > 10.0)
					lastTimeParticlesAnimation = currTimeParticlesAnimation;
				//glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);
				// Set the point size
				glPointSize(10.0f);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
				shaderParticlesFountain.turnOn();
				shaderParticlesFountain.setFloat("Time", float(currTimeParticlesAnimation - lastTimeParticlesAnimation));
				shaderParticlesFountain.setFloat("ParticleLifetime", 3.5f);
				shaderParticlesFountain.setInt("ParticleTex", 0);
				shaderParticlesFountain.setVectorFloat3("Gravity", glm::value_ptr(glm::vec3(0.0f, -0.3f, 0.0f)));
				shaderParticlesFountain.setMatrix4("model", 1, false, glm::value_ptr(modelMatrixParticlesFountain));
				glBindVertexArray(VAOParticles);
				glDrawArrays(GL_POINTS, 0, nParticles);
				glDepthMask(GL_TRUE);
				//glEnable(GL_DEPTH_TEST);
				shaderParticlesFountain.turnOff();
				/**********
				 * End Render particles systems
				 */
			}
			else if (it->second.first.compare("fire") == 0) {
				/**********
				 * Init Render particles systems
				 */
				lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
				currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

				shaderParticlesFire.setInt("Pass", 1);
				shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
				shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_1D, texId);
				glEnable(GL_RASTERIZER_DISCARD);
				glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
				glBeginTransformFeedback(GL_POINTS);
				glBindVertexArray(particleArray[1 - drawBuf]);
				glVertexAttribDivisor(0, 0);
				glVertexAttribDivisor(1, 0);
				glVertexAttribDivisor(2, 0);
				glDrawArrays(GL_POINTS, 0, nParticlesFire);
				glEndTransformFeedback();
				glDisable(GL_RASTERIZER_DISCARD);

				shaderParticlesFire.setInt("Pass", 2);
				glm::mat4 modelFireParticles = glm::mat4(1.0);
				modelFireParticles = glm::translate(modelFireParticles, it->second.second);
				modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
				shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

				shaderParticlesFire.turnOn();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
				glDepthMask(GL_FALSE);
				glBindVertexArray(particleArray[drawBuf]);
				glVertexAttribDivisor(0, 1);
				glVertexAttribDivisor(1, 1);
				glVertexAttribDivisor(2, 1);
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
				glBindVertexArray(0);
				glDepthMask(GL_TRUE);
				drawBuf = 1 - drawBuf;
				shaderParticlesFire.turnOff();

				/**********
				 * End Render particles systems
				 */
			}

		}
		glEnable(GL_CULL_FACE);

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/

		 //Collider del la rock
		AbstractModel::SBB rockCollider;
		glm::mat4 modelMatrixColliderRock = glm::mat4(matrixModelRoca);
		modelMatrixColliderRock = glm::scale(modelMatrixColliderRock, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderRock = glm::translate(modelMatrixColliderRock, modelRoca.getSbb().c);
		rockCollider.c = glm::vec3(modelMatrixColliderRock[3]);
		rockCollider.ratio = modelRoca.getSbb().ratio * 1.0;
		addOrUpdateColliders(collidersSBB, "rock", rockCollider, matrixModelRoca);

		//Collider de la fuente
		AbstractModel::OBB fountainCollider;
		glm::mat4 modelMatrixColliderFountain = glm::mat4(modelMatrixFountain);
		//modelMatrixColliderFountain = glm::rotate(modelMatrixColliderFountain, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		fountainCollider.u = glm::quat_cast(modelMatrixColliderFountain);
		modelMatrixColliderFountain = glm::scale(modelMatrixColliderFountain, glm::vec3(4.9, 10.0, 4.9));
		modelMatrixColliderFountain = glm::translate(modelMatrixFountain, modelFountain.getObb().c);
		fountainCollider.e = modelFountain.getObb().e * glm::vec3(4.9, 10.0, 4.9);
		fountainCollider.c = modelMatrixColliderFountain[3];
		addOrUpdateColliders(collidersOBB, "fountain", fountainCollider, modelMatrixFountain);


		//Colliders de la roca 2
		for (int i = 0; i < roca2Position.size(); i++) {
			AbstractModel::SBB rock2Collider;
			glm::mat4 modelMatrixColliderRock2 = glm::mat4(1.0);
			modelMatrixColliderRock2 = glm::translate(modelMatrixColliderRock2, roca2Position[i]);
			modelMatrixColliderRock2 = glm::rotate(modelMatrixColliderRock2, glm::radians(roca2Orientation[i]),
				glm::vec3(0, 1, 0));
			rock2Collider.c = glm::vec3(modelMatrixColliderRock2[3]);
			rock2Collider.ratio = modelRoca2.getSbb().ratio * 0.6;
			addOrUpdateColliders(collidersSBB, "rock2 no. -" + std::to_string(i), rock2Collider, modelMatrixColliderRock2);
		}
		//Colliders de los dulces
		//Basic Candy
		for (int i = 0; i < basCandyPosition.size(); i++) {
			AbstractModel::SBB basCandyCollider;
			glm::mat4 modelMatrixColliderBasCandy = glm::mat4(1.0);
			if (candyCollider == true && i == numElemento) {
				modelMatrixColliderBasCandy = glm::translate(modelMatrixColliderBasCandy, glm::vec3(basCandyPosition[i].x, -6.2, basCandyPosition[i].z));
			}
			else {
				modelMatrixColliderBasCandy = glm::translate(modelMatrixColliderBasCandy, glm::vec3(basCandyPosition[i].x, 1.2, basCandyPosition[i].z));
			}
			
			modelMatrixColliderBasCandy = glm::rotate(modelMatrixColliderBasCandy, glm::radians(basCandyOrientation[i]),
				glm::vec3(0, 1, 0));
			basCandyCollider.c = glm::vec3(modelMatrixColliderBasCandy[3]);
			basCandyCollider.ratio = modelBasicCandy.getSbb().ratio * 0.002;
			addOrUpdateColliders(collidersSBB, "BasicCandy" + std::to_string(i), basCandyCollider, modelMatrixColliderBasCandy);
		}

		//Color bomb
		for (int i = 0; i < basCandyPosition.size(); i++) {
			AbstractModel::SBB colBombCollider;
			glm::mat4 modelMatrixColliderColBomb = glm::mat4(1.0);
			if (candyCollider == true && i == numElemento) {
				modelMatrixColliderColBomb = glm::translate(modelMatrixColliderColBomb, glm::vec3(colBombPosition[i].x, -7.0, colBombPosition[i].z));
			}
			else {
				modelMatrixColliderColBomb = glm::translate(modelMatrixColliderColBomb, glm::vec3(colBombPosition[i].x, 1.0, colBombPosition[i].z));
			}
			modelMatrixColliderColBomb = glm::rotate(modelMatrixColliderColBomb, glm::radians(colBombOrientation[i]),
				glm::vec3(1, 0, 0));
			colBombCollider.c = glm::vec3(modelMatrixColliderColBomb[3]);
			colBombCollider.ratio = modelColorBomb.getSbb().ratio * 0.07;
			addOrUpdateColliders(collidersSBB, "ColorBomb" + std::to_string(i), colBombCollider, modelMatrixColliderColBomb);
		}

		//Lolipop colliders
		for (int i = 0; i < lolipopPosition.size(); i++) {
			AbstractModel::OBB lolipopCollider;
			glm::mat4 modelMatrixColliderLolipop = glm::mat4(1.0);
			if (candyCollider == true && i == numElemento) {
				modelMatrixColliderLolipop = glm::translate(modelMatrixColliderLolipop, glm::vec3(lolipopPosition[i].x, -5.5, lolipopPosition[i].z));
			}
			else {
				modelMatrixColliderLolipop = glm::translate(modelMatrixColliderLolipop, glm::vec3(lolipopPosition[i].x, 0.5, lolipopPosition[i].z));
			}
			modelMatrixColliderLolipop = glm::rotate(modelMatrixColliderLolipop, glm::radians(lolipopOrientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "Lolipop" + std::to_string(i), lolipopCollider, modelMatrixColliderLolipop);
			// Set the orientation of collider before doing the scale
			lolipopCollider.u = glm::quat_cast(modelMatrixColliderLolipop);
			modelMatrixColliderLolipop = glm::scale(modelMatrixColliderLolipop, glm::vec3(0.03, 0.03, 0.015));
			modelMatrixColliderLolipop = glm::translate(modelMatrixColliderLolipop, modelLolipop.getObb().c);
			lolipopCollider.c = glm::vec3(modelMatrixColliderLolipop[3]);
			lolipopCollider.e = modelLolipop.getObb().e * glm::vec3(0.03, 0.03, 0.015);
			std::get<0>(collidersOBB.find("Lolipop" + std::to_string(i))->second) = lolipopCollider;
		}

		//Radish colliders
		for (int i = 0; i < RadishPosition.size(); i++) {
			AbstractModel::OBB RadishCollider;
			glm::mat4 modelMatrixColliderRadish = glm::mat4(1.0);
			if (veggieCollider == true && i == numElemento) {
				modelMatrixColliderRadish = glm::translate(modelMatrixColliderRadish, glm::vec3(RadishPosition[i].x, -5.5, RadishPosition[i].z));
			}
			else {
				modelMatrixColliderRadish = glm::translate(modelMatrixColliderRadish, glm::vec3(RadishPosition[i].x, 0.5, RadishPosition[i].z));
			}
			modelMatrixColliderRadish = glm::rotate(modelMatrixColliderRadish, glm::radians(lolipopOrientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "Radish" + std::to_string(i), RadishCollider, modelMatrixColliderRadish);
			// Set the orientation of collider before doing the scale
			RadishCollider.u = glm::quat_cast(modelMatrixColliderRadish);
			modelMatrixColliderRadish = glm::scale(modelMatrixColliderRadish, glm::vec3(0.3, 0.3, 0.3));
			modelMatrixColliderRadish = glm::translate(modelMatrixColliderRadish, modelRadish.getObb().c);
			RadishCollider.c = glm::vec3(modelMatrixColliderRadish[3]);
			RadishCollider.e = modelRadish.getObb().e * glm::vec3(0.3, 0.3, 0.3);
			std::get<0>(collidersOBB.find("Radish" + std::to_string(i))->second) = RadishCollider;
		}
		//Collider de calaverita
		for (int i = 0; i < calavPosition.size(); i++) {
			AbstractModel::SBB calaveritaCollider;
			glm::mat4 modelMatrixColliderCalaverita = glm::mat4(1.0);
			if (calaveritaAzucarCollider == true && i == numElemento) {
				modelMatrixColliderCalaverita = glm::translate(modelMatrixColliderCalaverita, glm::vec3(calavPosition[i].x, -5.5, calavPosition[i].z));
			}
			else {
				modelMatrixColliderCalaverita = glm::translate(modelMatrixColliderCalaverita, glm::vec3(calavPosition[i].x, 0.5, calavPosition[i].z));
			}
			
			modelMatrixColliderCalaverita = glm::rotate(modelMatrixColliderCalaverita, glm::radians(calavOrientation[i]),
				glm::vec3(0, 1, 0));
			calaveritaCollider.c = glm::vec3(modelMatrixColliderCalaverita[3]);
			calaveritaCollider.ratio = modelCalaverita.getSbb().ratio * 0.03;
			addOrUpdateColliders(collidersSBB, "Calaverita" + std::to_string(i), calaveritaCollider, modelMatrixColliderCalaverita);
		}
		//Casa colliders
		for (int i = 0; i < casaPosition.size(); i++) {
			AbstractModel::OBB casaCollider;
			glm::mat4 modelMatrixCollidercasa = glm::mat4(1.0);
			modelMatrixCollidercasa = glm::translate(modelMatrixCollidercasa, casaPosition[i]);
			modelMatrixCollidercasa = glm::rotate(modelMatrixCollidercasa, glm::radians(casaOrientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "casa no. -" + std::to_string(i), casaCollider, modelMatrixCollidercasa);
			// Set the orientation of collider before doing the scale
			casaCollider.u = glm::quat_cast(modelMatrixCollidercasa);
			modelMatrixCollidercasa = glm::scale(modelMatrixCollidercasa, glm::vec3(2500, 2500, 2500));
			modelMatrixCollidercasa = glm::translate(modelMatrixCollidercasa, modelCasa.getObb().c);
			casaCollider.c = glm::vec3(modelMatrixCollidercasa[3]);
			casaCollider.e = modelCasa.getObb().e * glm::vec3(2300, 2300, 2300);
			std::get<0>(collidersOBB.find("casa no. -" + std::to_string(i))->second) = casaCollider;
		}

		// LampMed colliders
		for (int i = 0; i < lampMedPosition.size(); i++) {
			AbstractModel::OBB lampCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lampMedPosition[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lampMedOrientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "lampMed no. " + std::to_string(i), lampCollider, modelMatrixColliderLamp);
			// Set the orientation of collider before doing the scale
			lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(0.5, 0.5, 0.5));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLampMed.getObb().c);
			lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
			lampCollider.e = modelLampMed.getObb().e * glm::vec3(0.5, 0.5, 0.5);
			std::get<0>(collidersOBB.find("lampMed no. " + std::to_string(i))->second) = lampCollider;
		}

		// Autumn tree colliders
		for (int i = 0; i < autumnPosition.size(); i++) {
			AbstractModel::OBB autumnCollider;
			glm::mat4 modelMatrixColliderAutumn = glm::mat4(1.0);
			modelMatrixColliderAutumn = glm::translate(modelMatrixColliderAutumn, autumnPosition[i]);
			modelMatrixColliderAutumn = glm::rotate(modelMatrixColliderAutumn, glm::radians(autumnOrientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "Autumn tree no. " + std::to_string(i), autumnCollider, modelMatrixColliderAutumn);
			// Set the orientation of collider before doing the scale
			autumnCollider.u = glm::quat_cast(modelMatrixColliderAutumn);
			modelMatrixColliderAutumn = glm::scale(modelMatrixColliderAutumn, glm::vec3(0.015, 0.15, 0.015));
			modelMatrixColliderAutumn = glm::translate(modelMatrixColliderAutumn, modelAutumnTree.getObb().c);
			autumnCollider.c = glm::vec3(modelMatrixColliderAutumn[3]);
			autumnCollider.e = modelAutumnTree.getObb().e * glm::vec3(0.015, 0.15, 0.015);
			std::get<0>(collidersOBB.find("Autumn tree no. " + std::to_string(i))->second) = autumnCollider;
		}

		//Arbol frondoso colliders
		for (int i = 0; i < arbFronPosition.size(); i++) {
			AbstractModel::OBB arbFronCollider;
			glm::mat4 modelMatrixColliderFrondoso = glm::mat4(1.0);
			modelMatrixColliderFrondoso = glm::translate(modelMatrixColliderFrondoso, glm::vec3(arbFronPosition[i].x, 0.3, arbFronPosition[i].z));
			modelMatrixColliderFrondoso = glm::rotate(modelMatrixColliderFrondoso, glm::radians(autumnOrientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "Arbol frondoso no. - " + std::to_string(i), arbFronCollider, modelMatrixColliderFrondoso);
			// Set the orientation of collider before doing the scale
			arbFronCollider.u = glm::quat_cast(modelMatrixColliderFrondoso);
			modelMatrixColliderFrondoso = glm::scale(modelMatrixColliderFrondoso, glm::vec3(0.5, 2.5, 0.5));
			modelMatrixColliderFrondoso = glm::translate(modelMatrixColliderFrondoso, modelArbolFrondoso.getObb().c);
			arbFronCollider.c = glm::vec3(modelMatrixColliderFrondoso[3]);
			arbFronCollider.e = modelArbolFrondoso.getObb().e * glm::vec3(0.5, 2.5, 0.5);
			std::get<0>(collidersOBB.find("Arbol frondoso no. - " + std::to_string(i))->second) = arbFronCollider;
		}


		// Collider de Boy
		AbstractModel::OBB boyCollider;
		glm::mat4 modelmatrixColliderBoy = glm::mat4(modelMatrixBoy);
		modelmatrixColliderBoy = glm::rotate(modelmatrixColliderBoy,
			glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		boyCollider.u = glm::quat_cast(modelmatrixColliderBoy);
		modelmatrixColliderBoy = glm::scale(modelmatrixColliderBoy, glm::vec3(0.4, 0.5, 0.5));
		modelmatrixColliderBoy = glm::translate(modelmatrixColliderBoy,
			glm::vec3(boyModelAnimate.getObb().c.x,
				boyModelAnimate.getObb().c.y,
				boyModelAnimate.getObb().c.z));
		boyCollider.e = boyModelAnimate.getObb().e * glm::vec3(0.4, 0.5, 0.5);
		boyCollider.c = glm::vec3(modelmatrixColliderBoy[3]);
		addOrUpdateColliders(collidersOBB, "Boy", boyCollider, modelMatrixBoy);

		//Collider Spook
		AbstractModel::OBB spookCollider;
		glm::mat4 modelmatrixColliderSpook = glm::mat4(modelMatrixSpook);
		modelmatrixColliderSpook = glm::rotate(modelmatrixColliderSpook,
			glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		spookCollider.u = glm::quat_cast(modelmatrixColliderSpook);
		modelmatrixColliderSpook = glm::scale(modelmatrixColliderSpook, glm::vec3(1.5, 1.5, 1.5));
		modelmatrixColliderSpook = glm::translate(modelmatrixColliderSpook,
		glm::vec3(spookAnimate.getObb().c.x, spookAnimate.getObb().c.y, spookAnimate.getObb().c.z));
		spookCollider.e = spookAnimate.getObb().e * glm::vec3(1.5, 1.5, 1.5);
		spookCollider.c = glm::vec3(modelmatrixColliderSpook[3]);
		addOrUpdateColliders(collidersOBB, "Spook", spookCollider, modelMatrixSpook);

		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}

		// Esto es para ilustrar la transformacion inversa de los coliders
		/*glm::vec3 cinv = glm::inverse(BoyCollider.u) * glm::vec4(rockCollider.c, 1.0);
		glm::mat4 invColliderS = glm::mat4(1.0);
		invColliderS = glm::translate(invColliderS, cinv);
		invColliderS =  invColliderS * glm::mat4(BoyCollider.u);
		invColliderS = glm::scale(invColliderS, glm::vec3(rockCollider.ratio * 2.0, rockCollider.ratio * 2.0, rockCollider.ratio * 2.0));
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphereCollider.enableWireMode();
		sphereCollider.render(invColliderS);
		glm::vec3 cinv2 = glm::inverse(BoyCollider.u) * glm::vec4(BoyCollider.c, 1.0);
		glm::mat4 invColliderB = glm::mat4(1.0);
		invColliderB = glm::translate(invColliderB, cinv2);
		invColliderB = glm::scale(invColliderB, BoyCollider.e * 2.0f);
		boxCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		boxCollider.enableWireMode();
		boxCollider.render(invColliderB);
		// Se regresa el color blanco
		/*sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));*/

		/*******************************************
		 * Test Colisions
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it = collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt = collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt && testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with " << jt->first << std::endl;
					isCollision = true;
						if (isCollision) {
							elemento = it->first;
							numElemento = colisionesObjetos(elemento);
							veggieCollider = true;
							candyCollider = true;
							spookHelp = true;
						}
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it = collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt = collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt && testSphereSphereIntersection(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with " << jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it = collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with " << jt->first << std::endl;
					isCollision = true;
						if (isCollision) {
							elemento = it->first;
							numElemento = colisionesObjetos(elemento);
							candyCollider = true;
							calaveritaAzucarCollider = true;
						}
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end();
			colIt++) {
			std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.find(colIt->first);
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("Boy") == 0) {
						modelMatrixBoy = std::get<1>(jt->second);

					}
						
					
				}
			}
		}
		/**********************************
		**	TIMER
		**********************************/
		int minutos = 0, segundos = 0;
		std::string tiempoLeft = minutos + ":" + segundos;
			segundos++;

			if (segundos > 59) {
				minutos++;
				segundos = 0;

			}
			if (minutos > 10) {
				minutos = 0;
			}
			modelTimer->render(tiempoLeft, -0.7, 0.8, 20, 1.0, 0.0, 0.0);


		/*******************************************
		 * Textos
		 *******************************************/
		glm::vec3 distanciaSpook = modelMatrixSpook[3];
		if (glm::distance(camera_pos, distanciaSpook) < 10.0) {
			glEnable(GL_BLEND);
			modelText->render("Hola! Aqui un consejo...", -0.3, 0.8, 30, 0.4, 0.0, 0.8);
			modelText->render("Tienes que hallar todas las calaveritas, los dulces te ayudaran pero cuidado...",
				-0.9, -0.85, 20, 1.0, 0.6, 0.2);
			modelText->render("...no todos son dulces",
				-0.9, -0.9, 20, 1.0, 0.6, 0.2);
			glDisable(GL_BLEND);
		}
		std::string infoCalav = "Calaveras: " + std::to_string(countCalav)+"/6";
		modelText->render(infoCalav, -1.0, -1.0005, 20, 1.0, 0.0, 0.0);

		std::string infoCandy = "Dulces: " + std::to_string(countCandy);
		modelText->render(infoCandy, -1.0, -0.9, 20, 1.0, 0.0, 0.0);
		

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
