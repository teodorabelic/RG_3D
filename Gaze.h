#pragma once
#include <glm/glm.hpp>

void updateGaze(float deltaTime);
bool raySphereHit(glm::vec3 ro, glm::vec3 rd, glm::vec3 c, float r);
