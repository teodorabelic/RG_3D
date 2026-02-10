#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Util.h"
#include "Camera.h"
#include "Gaze.h"
#include "Simulation.h"
#include "Globals.h"
#include "Mesh.h"
#include "Render.h"
#include "Interaction.h"

void handleInput(GLFWwindow* window, float deltaTime)
{
    static bool depthLatch = false;
    static bool cullLatch = false;

    bool key1 = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
    if (key1 && !depthLatch) {
        depthLatch = true;
        depthTestOn = !depthTestOn;
        depthTestOn ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }
    if (!key1) depthLatch = false;

    bool key2 = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
    if (key2 && !cullLatch) {
        cullLatch = true;
        cullFaceOn = !cullFaceOn;
        cullFaceOn ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    }
    if (!key2) cullLatch = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void update(float dt)
{
    updateGaze(dt);
    updateWater(dt);
    updateDrops(dt);
    updateFlap(dt);
    updateTemperature(dt);
}


void renderScene(unsigned int shader, unsigned int screenShader)
{
    glUniform1i(glGetUniformLocation(shader, "acOn"), 0);
    renderRoom(shader);
    renderAC(shader);
    glUniform1i(glGetUniformLocation(shader, "acOn"), acOn);
    renderLamp(shader);
    glUniform1i(glGetUniformLocation(shader, "acOn"), 0);
    renderBowl(shader);
    renderDrops(shader);
    renderWC(shader);
    renderCrosshair(shader);
    renderRemote(shader);
    renderAirVent(shader);
    renderFlap(shader);
    renderTemperatureScreens(screenShader);
}

void fixTexture(unsigned int tex)
{
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec3 sceneLightPos = acPos + glm::vec3(0.0f, 2.0f, 2.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(mon);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Klima", mon, NULL);
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwFocusWindow(window);

    glewInit();
    initScreenQuad();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int shader = createShader("basic.vert", "basic.frag");
    glUseProgram(shader);

    glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(lightColor));

    uM = glGetUniformLocation(shader, "uM"); // globalni uM za basic stvari
    overrideColorLoc = glGetUniformLocation(shader, "overrideColor");

    unsigned int screenShader =createShader("screen.vert", "screen.frag");
    unsigned int signatureShader = createShader("signature.vert", "signature.frag");

    texHeat = loadImageToTexture("res/fire.png");
    texCool = loadImageToTexture("res/snow.png");
    texOk = loadImageToTexture("res/ok.png");
    texMinus = loadImageToTexture("res/minus.png");
    texC = loadImageToTexture("res/celsius.png");
    texCreator = loadImageToTexture("res/creator.png");

    fixTexture(texHeat);
    fixTexture(texCool);
    fixTexture(texOk);
    fixTexture(texMinus);
    fixTexture(texC);
    fixTexture(texCreator);

    texDigits[0] = loadImageToTexture("res/0.png");
    texDigits[1] = loadImageToTexture("res/1.png");
    texDigits[2] = loadImageToTexture("res/2.png");
    texDigits[3] = loadImageToTexture("res/3.png");
    texDigits[4] = loadImageToTexture("res/4.png");
    texDigits[5] = loadImageToTexture("res/5.png");
    texDigits[6] = loadImageToTexture("res/6.png");
    texDigits[7] = loadImageToTexture("res/7.png");
    texDigits[8] = loadImageToTexture("res/8.png");
    texDigits[9] = loadImageToTexture("res/9.png");

    for (int i = 0; i < 10; i++)
        fixTexture(texDigits[i]);

    glUniform1i(glGetUniformLocation(shader, "uTex"), 0);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cubeStride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, cubeStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, cubeStride, (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, cubeStride, (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    unsigned int uV = glGetUniformLocation(shader, "uV");
    unsigned int uP = glGetUniformLocation(shader, "uP");
    overrideColorLoc = glGetUniformLocation(shader, "overrideColor");

    bool clickLatch = false;

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        auto frameStart = std::chrono::steady_clock::now();

        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        handleInput(window, deltaTime);
        update(deltaTime);
        updateInteraction(window, deltaTime);

        // render petlja

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);

        glUniform3fv(
            glGetUniformLocation(shader, "lightPos"),
            1,
            glm::value_ptr(lightPos)
        );

        glUniform3fv(
            glGetUniformLocation(shader, "lightColor"),
            1,
            glm::value_ptr(lightColor)
        );

        glUniform3fv(
            glGetUniformLocation(shader, "viewPos"),
            1,
            glm::value_ptr(cameraPos)
        );

        glBindVertexArray(VAO);

        glm::mat4 V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 P = glm::perspective(
            glm::radians(60.0f),
            (float)mode->width / mode->height,
            0.3f,
            100.0f
        );

        // basic shader
        glUseProgram(shader);
        glUniformMatrix4fv(uV, 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(uP, 1, GL_FALSE, glm::value_ptr(P));
        glUniform1i(glGetUniformLocation(shader, "acOn"), acOn);

        // screen shader
        glUseProgram(screenShader);
        GLint uV_s = glGetUniformLocation(screenShader, "uV");
        GLint uP_s = glGetUniformLocation(screenShader, "uP");
        glUniformMatrix4fv(uV_s, 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(uP_s, 1, GL_FALSE, glm::value_ptr(P));

        glUseProgram(shader);
        glBindVertexArray(VAO);
        renderScene(shader, screenShader);

        renderSignature(signatureShader);

        glfwSwapBuffers(window);
        glfwPollEvents();

        auto frameEnd = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = frameEnd - frameStart;
        if (elapsed.count() < 1.0 / 75.0)
            std::this_thread::sleep_for(
                std::chrono::duration<double>(1.0 / 75.0 - elapsed.count()));
    }

    glfwTerminate();
    return 0;
}
;