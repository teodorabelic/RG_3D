#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void renderAC(unsigned int shader);
void renderLamp(unsigned int shader);
static void drawHollowCylinder(float outerR,float innerR,float height,int segments);
void renderBowl(unsigned int shader);
void renderDrops(unsigned int shader);
void renderWC(unsigned int shader);
void renderCrosshair(unsigned int shader);
void renderRemote(unsigned int shader);
void renderFlap(unsigned int render);
void renderAirVent(unsigned int shader);
void renderTemperatureScreens(unsigned int shader);
void renderRoom(unsigned int shader);
void initScreenQuad();
void renderSignature(unsigned int screenShader);