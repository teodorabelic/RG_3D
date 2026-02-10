#include "Interaction.h"
#include "Globals.h"
#include "Gaze.h"
#include <iostream>

void updateInteraction(GLFWwindow* window, float dt)
{
    // orijentacija
    glm::vec3 toAC = glm::normalize(acPos - cameraPos);
    float dotVal = glm::dot(glm::normalize(cameraFront), toAC);

    bool facingAC = dotVal > 0.8f;
    bool facingAway = dotVal < -0.8f;

    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));

    // klik
    static bool clickLatch = false;
    int mouse = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    bool clickEvent = (mouse == GLFW_PRESS && !clickLatch);
    if (mouse == GLFW_PRESS) clickLatch = true;
    if (mouse == GLFW_RELEASE) clickLatch = false;

    if (clickEvent)
    {
        glm::vec3 rayOrigin = cameraPos;
        glm::vec3 rayDir = glm::normalize(cameraFront);

        bool handsFree =
            state != AppState::Bowl_In_Hands_Full &&
            state != AppState::Bowl_In_Hands_Empty;

        if (handsFree &&
            state == AppState::AC_Off_Full &&
            raySphereHit(rayOrigin, rayDir, bowlPos, bowlRadius))
        {
            state = AppState::Bowl_In_Hands_Full;
            std::cout << "[CLICK] Lavor uzet\n";
        }
    }

    // strelice
    static bool upLatch = false;
    static bool downLatch = false;

    bool upPressed = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    bool downPressed = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;

    if (acOn)
    {
        if (upPressed && !upLatch)
        {
            upLatch = true;
            desiredTemp++;
            if (desiredTemp > 40) desiredTemp = 40;
            std::cout << "[TEMP] Desired = " << desiredTemp << "\n";
        }

        if (downPressed && !downLatch)
        {
            downLatch = true;
            desiredTemp--;
            if (desiredTemp < -10) desiredTemp = -10;
            std::cout << "[TEMP] Desired = " << desiredTemp << "\n";
        }
    }

    if (!upPressed) upLatch = false;
    if (!downPressed) downLatch = false;


    // space
    static bool spaceLatch = false;
    bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

    if (spacePressed && !spaceLatch)
    {
        spaceLatch = true;

        if (state == AppState::Bowl_In_Hands_Full && facingAway)
        {
            waterLevel = 0.0f;
            state = AppState::Bowl_In_Hands_Empty;
        }
        else if (state == AppState::Bowl_In_Hands_Empty && facingAC)
        {
            state = AppState::Idle;
        }
    }

    if (!spacePressed)
        spaceLatch = false;
}
