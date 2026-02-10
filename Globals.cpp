#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Globals.h"

// render state
bool useTex = false;
bool transparent = false;

bool depthTestOn = true;
bool cullFaceOn = false;


// kamera
bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

glm::vec3 cameraPos(0.0f, 1.3f, 1.5f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);


// klima
AppState state = AppState::Idle;
bool acOn = false;
float waterLevel = 0.0f;


// pozicije
glm::vec3 acPos(0.0f, 1.2f, -4.0f);
glm::vec3 lampPos(1.3f, 1.6f, -3.3f);
float lampRadius = 0.25f;

glm::vec3 bowlPos(0.0f, -0.8f, -4.5f);
float bowlRadius = 0.6f;

glm::vec3 wcPos(0.0f, -0.8f, 3.5f);


// kapljice
std::vector<Drop> drops;


// gaze/daljinski
bool lampGazeLatch = false;


// flap/precaga
float flapAngle = 0.0f;
const float flapMax = 45.0f;
const float flapSpeed = 60.0f;


// temperatura
int desiredTemp = 24;
int measuredTemp = 30.0f;
float tempTimer = 0.0f;


// shader
GLint uM = -1;
GLint overrideColorLoc = -1;

// ekrani
unsigned int texHeat = 0;
unsigned int texCool = 0;
unsigned int texOk = 0;
unsigned int texMinus = 0;
unsigned int texC = 0;
unsigned int texCreator = 0;
unsigned int screenVAO = 0;
unsigned int screenVBO = 0;
unsigned int texDigits[10];

// svetlo
glm::vec3 lightPos = glm::vec3(0.0f, 4.5f, 1.5f); // PLAFON
glm::vec3 lightColor = glm::vec3(1.0f);


