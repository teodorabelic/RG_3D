#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// render state 
extern bool useTex;
extern bool transparent;

extern bool depthTestOn;
extern bool cullFaceOn;


// kamera
extern bool firstMouse;
extern float lastX, lastY;
extern float yaw, pitch;
extern float fov;

extern glm::vec3 cameraPos;
extern glm::vec3 cameraUp;
extern glm::vec3 cameraFront;


// klima 
enum class AppState {
    Idle,
    AC_On_Filling,
    AC_Off_Full,
    Bowl_In_Hands_Full,
    Bowl_In_Hands_Empty
};

extern AppState state;
extern bool acOn;
extern float waterLevel;


// pozicije
extern glm::vec3 acPos;
extern glm::vec3 lampPos;
extern float lampRadius;
extern glm::vec3 bowlPos;
extern float bowlRadius;
extern glm::vec3 wcPos;


// kapljice
struct Drop {
    glm::vec3 p, v;
    bool alive;
};
extern std::vector<Drop> drops;


// gaze/daljinski
extern bool lampGazeLatch;


// flap/precaga
extern float flapAngle;
extern const float flapMax;
extern const float flapSpeed;


// temperatura
extern int desiredTemp;
extern int measuredTemp;
extern float tempTimer;


// shader
extern GLint uM;
extern GLint overrideColorLoc;


// ekrani
extern unsigned int texHeat;
extern unsigned int texCool;
extern unsigned int texOk;
extern unsigned int texMinus;
extern unsigned int texC;
extern unsigned int texCreator;
extern unsigned int screenVAO;
extern unsigned int screenVBO;
extern unsigned int texDigits[10];


// svetlo
extern glm::vec3 lightPos;
extern glm::vec3 lightColor;
