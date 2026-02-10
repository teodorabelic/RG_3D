#include <iostream>

#include "Gaze.h"
#include "Globals.h"
#include "Camera.h"


bool raySphereHit(glm::vec3 ro, glm::vec3 rd, glm::vec3 c, float r)
{
    glm::vec3 oc = ro - c;
    float b = glm::dot(oc, rd);
    float c2 = glm::dot(oc, oc) - r * r;
    return b * b - c2 >= 0.0f;
}

void updateGaze(float /*deltaTime*/)
{
    bool handsFree =
        (state != AppState::Bowl_In_Hands_Full &&
         state != AppState::Bowl_In_Hands_Empty);

    if (!handsFree)
        return;

    glm::vec3 toLamp = glm::normalize(lampPos - cameraPos);
    float gazeDot = glm::dot(glm::normalize(cameraFront), toLamp);

    const float GAZE_ON = 0.95f;
    const float GAZE_OFF = 0.85f;

    bool rayHitLamp = raySphereHit(
        cameraPos,
        glm::normalize(cameraFront),
        lampPos,
        lampRadius
    );

    bool stronglyLookingAtLamp = rayHitLamp && gazeDot > GAZE_ON;
    bool clearlyNotLookingAtLamp = !rayHitLamp || gazeDot < GAZE_OFF;

    if (stronglyLookingAtLamp && !lampGazeLatch)
    {
        lampGazeLatch = true;

        if (!acOn)
        {
            acOn = true;
            state = AppState::AC_On_Filling;
            std::cout << "[GAZE] Klima ON\n";
        }
        else
        {
            acOn = false;
            std::cout << "[GAZE] Klima OFF\n";
        }
    }

    if (clearlyNotLookingAtLamp)
        lampGazeLatch = false;
}
