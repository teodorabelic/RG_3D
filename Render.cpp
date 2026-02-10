#include "Globals.h"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Render.h"

static void drawCube()
{
    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
}

static void drawCircle(float radius, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);

    for (int i = 0; i <= segments; i++)
    {
        float a = 2.0f * 3.1415926f * i / segments;
        glVertex3f(
            cos(a) * radius,
            sin(a) * radius,
            0.0f
        );
    }
    glEnd();
}

void renderAC(unsigned int shader)
{
    glm::mat4 M(1.0f);
    M = glm::translate(M, acPos);
    M = glm::scale(M, glm::vec3(3.0f, 1.2f, 1.0f));
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4f(overrideColorLoc, 0.85f, 0.85f, 0.85f, 1.0f);
    glUniform1i(glGetUniformLocation(shader, "acOn"), 0);
    drawCube();
}


void renderLamp(unsigned int shader)
{
    glm::mat4 M(1.0f);
    M = glm::translate(M, lampPos);
    M = glm::scale(
        M,
        glm::vec3(
            lampRadius,
            lampRadius, 
            lampRadius * 0.2f
        )
    );
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4f(overrideColorLoc, 0.25f, 0.25f, 0.25f, 1.0f);
    glUniform1i(glGetUniformLocation(shader, "acOn"), acOn);
    drawCircle(lampRadius, 32);
}

static void drawHollowCylinder(
    float outerR,
    float innerR,
    float height,
    int segments
) {
    float halfH = height * 0.5f;

    // spoljasnji zid
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float a = 2.0f * 3.1415926f * i / segments;
        float x = cos(a);
        float z = sin(a);

        glVertex3f(x * outerR, -halfH, z * outerR);
        glVertex3f(x * outerR, halfH, z * outerR);
    }
    glEnd();

    // unutrasnji zid
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float a = 2.0f * 3.1415926f * i / segments;
        float x = cos(a);
        float z = sin(a);

        glVertex3f(x * innerR, halfH, z * innerR);
        glVertex3f(x * innerR, -halfH, z * innerR);
    }
    glEnd();

    // dno
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, -halfH, 0.0f);
    for (int i = 0; i <= segments; i++) {
        float a = 2.0f * 3.1415926f * i / segments;
        glVertex3f(
            cos(a) * outerR,
            -halfH,
            sin(a) * outerR
        );
    }
    glEnd();
}

void renderBowl(unsigned int shader)
{
    glm::vec3 bowlDrawPos = bowlPos;

    if (state == AppState::Bowl_In_Hands_Full ||
        state == AppState::Bowl_In_Hands_Empty)
    {
        glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
        bowlDrawPos =
            cameraPos +
            cameraFront * 1.0f +
            right * 0.35f +
            cameraUp * (-0.35f);
    }

    glm::mat4 M(1.0f);
    M = glm::translate(M, bowlDrawPos);
    M = glm::scale(M, glm::vec3(1.0f, 1.0f, 1.0f));

    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4f(overrideColorLoc, 0.6f, 0.6f, 0.67f, 1.0f);
    glUniform1i(glGetUniformLocation(shader, "acOn"), 0);

    drawHollowCylinder(
        0.6f,
        0.43f,
        0.5f,
        32
    );

    // voda unutra
    if (waterLevel > 0.0f)
    {
        float waterH = 0.25f * waterLevel;

        glm::mat4 MW(1.0f);
        MW = glm::translate(
            MW,
            bowlDrawPos + glm::vec3(0.0f, -0.25f + waterH * 0.5f + 0.001f, 0.0f)
        );

        MW = glm::scale(MW, glm::vec3(1.0f));

        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(MW));
        glUniform4f(overrideColorLoc, 0.0f, 0.35f, 0.8f, 0.75f);

        drawHollowCylinder(
            0.40f,
            0.0f,
            waterH,
            32
        );
    }
}


void renderDrops(unsigned int shader)
{
    for (const auto& d : drops)
    {
        if (!d.alive) continue;

        glm::mat4 M(1.0f);
        M = glm::translate(M, d.p);
        M = glm::scale(M, glm::vec3(0.08f));

        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
        glUniform4f(overrideColorLoc, 0.3f, 0.6f, 1.0f, 0.7f);
        glUniform1i(glGetUniformLocation(shader, "acOn"), 0);

        for (int i = 0; i < 6; i++)
            glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
}

void renderWC(unsigned int shader)
{
    float lift = 1.0f;

    glm::vec3 basePos = wcPos + glm::vec3(0.0f, lift, 0.0f);

    glUniform1i(glGetUniformLocation(shader, "acOn"), 0);

    {
        glm::mat4 M(1.0f);
        M = glm::translate(M, basePos + glm::vec3(0.0f, -0.25f, 0.0f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));

        glUniform4f(overrideColorLoc, 0.92f, 0.92f, 0.92f, 1.0f);

        drawHollowCylinder(
            0.45f,
            0.0f,
            0.35f,
            32
        );
    }

    {
        glm::mat4 M(1.0f);
        M = glm::translate(M, basePos + glm::vec3(0.0f, 0.05f, 0.0f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));

        glUniform4f(overrideColorLoc, 0.95f, 0.95f, 0.95f, 1.0f);

        drawHollowCylinder(
            0.55f,
            0.30f,
            0.35f,
            32
        );
    }

    {
        glm::mat4 M(1.0f);
        M = glm::translate(M, basePos + glm::vec3(0.0f, 0.05f, 0.0f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));

        glUniform4f(overrideColorLoc, 0.65f, 0.65f, 0.65f, 1.0f);

        drawHollowCylinder(
            0.30f,
            0.15f,
            0.30f,
            32
        );
    }

    {
        glm::mat4 M(1.0f);
        M = glm::translate(M, basePos + glm::vec3(0.0f, -0.02f, 0.0f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));

        glUniform4f(overrideColorLoc, 0.3f, 0.6f, 0.9f, 0.85f);

        drawHollowCylinder(
            0.26f,
            0.0f,
            0.05f,
            32
        );
    }

    {
        glm::mat4 M(1.0f);
        M = glm::translate(M, basePos + glm::vec3(0.0f, 0.25f, 0.0f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));

        glUniform4f(overrideColorLoc, 0.97f, 0.97f, 0.97f, 1.0f);

        drawHollowCylinder(
            0.58f,
            0.50f,
            0.05f,
            32
        );
    }
}


void renderCrosshair(unsigned int shader)
{
    bool prevDepth = depthTestOn;
    glDisable(GL_DEPTH_TEST);

    glm::vec3 centerPos = cameraPos + cameraFront * 0.5f;

    // horizontalna
    {
        glm::mat4 M(1.0f);
        M = glm::translate(M, centerPos);
        M = glm::scale(M, glm::vec3(0.03f, 0.002f, 0.005f));

        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
        glUniform4f(overrideColorLoc, 0.0f, 0.0f, 0.0f, 0.8f);
        glUniform1i(glGetUniformLocation(shader, "acOn"), 0);

        for (int i = 0; i < 6; i++)
            glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }

    // vertikalna
    {
        glm::mat4 M(1.0f);
        M = glm::translate(M, centerPos);
        M = glm::scale(M, glm::vec3(0.002f, 0.03f, 0.005f));

        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
        glUniform4f(overrideColorLoc, 0.0f, 0.0f, 0.0f, 0.8f);
        glUniform1i(glGetUniformLocation(shader, "acOn"), 0);

        for (int i = 0; i < 6; i++)
            glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }

    if (prevDepth) glEnable(GL_DEPTH_TEST);
}

void renderRemote(unsigned int shader)
{
    bool handsFree =
        (state != AppState::Bowl_In_Hands_Full &&
            state != AppState::Bowl_In_Hands_Empty);

    if (!handsFree) return;

    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));

    glm::vec3 remotePos =
        cameraPos +
        cameraFront * 0.55f +
        right * 0.20f + 
        cameraUp * (-0.15f);


    glm::mat4 M(1.0f);
    M = glm::translate(M, remotePos);
    M = glm::scale(M, glm::vec3(0.06f, 0.09f, 0.018f));

    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4f(overrideColorLoc, 0.2f, 0.2f, 0.2f, 1.0f);
    glUniform1i(glGetUniformLocation(shader, "acOn"), 0);

    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
}


void renderFlap(unsigned int shader)
{
    glm::mat4 M(1.0f);

    M = glm::translate(M, acPos + glm::vec3(0.0f, -0.35f, 0.55f));

    M = glm::rotate(M, glm::radians(-flapAngle), glm::vec3(1, 0, 0));

    M = glm::scale(M, glm::vec3(2.8f, 0.2f, 0.1f));

    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4f(overrideColorLoc, 0.8f, 0.8f, 0.8f, 1.0f);
    glUniform1i(glGetUniformLocation(shader, "acOn"), 0);

    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
}


void renderAirVent(unsigned int shader)
{
    float openFactor = flapAngle / flapMax;

    if (openFactor < 0.05f)
        return;

    glm::mat4 M(1.0f);

    M = glm::translate(
        M,
        acPos + glm::vec3(
            0.0f,
            -0.40f - openFactor * 0.05f, 
            0.55f
        )
    );

    M = glm::scale(
        M,
        glm::vec3(
            2.8f,
            0.02f + openFactor * 0.15f,
            0.05f
        )
    );

    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));

    glUniform4f(overrideColorLoc, 0.05f, 0.05f, 0.05f, 1.0f);
    glUniform1i(glGetUniformLocation(shader, "acOn"), 0);

    drawCube();
}


void renderTemperatureScreens(unsigned int screenShader)
{
    bool prevDepth = depthTestOn;
    glDisable(GL_DEPTH_TEST);

    glUseProgram(screenShader);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(screenShader, "uTex"), 0);

    GLint uM = glGetUniformLocation(screenShader, "uM");

    bool screensOn = acOn;

    glm::mat4 M;

    glBindTexture(GL_TEXTURE_2D, 0); 
    glUniform4f(overrideColorLoc, 0.0f, 0.0f, 0.0f, 1.0f);

    M = glm::translate(glm::mat4(1), acPos + glm::vec3(-0.8f, 0.2f, 0.62f));
    M = glm::scale(M, glm::vec3(0.60f, 0.32f, 0.01f));
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    // prvi ekran - zeljena temperatura
    if (screensOn)
    {
        int temp = desiredTemp;
        bool neg = temp < 0;
        int absT = abs(temp);
        int tens = absT / 10;
        int ones = absT % 10;

        float x = -0.95f;

        if (neg)
        {
            glBindTexture(GL_TEXTURE_2D, texMinus);
            M = glm::translate(glm::mat4(1), acPos + glm::vec3(x, 0.2f, 0.62f));
            M = glm::scale(M, glm::vec3(0.15f, 0.30f, 0.01f));
            glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            x += 0.14f;
        }

        if (tens > 0)
        {
            glBindTexture(GL_TEXTURE_2D, texDigits[tens]);
            M = glm::translate(glm::mat4(1), acPos + glm::vec3(x, 0.2f, 0.62f));
            M = glm::scale(M, glm::vec3(0.25f, 0.30f, 0.01f));
            glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            x += 0.22f;
        }

        glBindTexture(GL_TEXTURE_2D, texDigits[ones]);
        M = glm::translate(glm::mat4(1), acPos + glm::vec3(x, 0.2f, 0.62f));
        M = glm::scale(M, glm::vec3(0.25f, 0.30f, 0.01f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // °C
        glBindTexture(GL_TEXTURE_2D, texC);
        M = glm::translate(
            glm::mat4(1),
            acPos + glm::vec3(
                x + 0.15f,
                0.2f + 0.12f,
                0.62f
            )
        );
        M = glm::scale(M, glm::vec3(0.12f, 0.16f, 0.01f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        M = glm::translate(glm::mat4(1), acPos + glm::vec3(-0.8f, 0.2f, 0.62f));
        M = glm::scale(M, glm::vec3(0.45f, 0.30f, 0.01f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // crna pozadina
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform4f(overrideColorLoc, 0.0f, 0.0f, 0.0f, 1.0f);

    M = glm::translate(glm::mat4(1), acPos + glm::vec3(0.0f, 0.2f, 0.62f));
    M = glm::scale(M, glm::vec3(0.60f, 0.32f, 0.01f));
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    // drugi ekran - izmerena temperatura
    if (screensOn)
    {
        int temp = measuredTemp;
        bool neg = temp < 0;
        int absT = abs(temp);
        int tens = absT / 10;
        int ones = absT % 10;

        float x = -0.15f;

        if (neg)
        {
            glBindTexture(GL_TEXTURE_2D, texMinus);
            M = glm::translate(glm::mat4(1), acPos + glm::vec3(x, 0.2f, 0.62f));
            M = glm::scale(M, glm::vec3(0.15f, 0.30f, 0.01f));
            glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            x += 0.14f;
        }

        if (tens > 0)
        {
            glBindTexture(GL_TEXTURE_2D, texDigits[tens]);
            M = glm::translate(glm::mat4(1), acPos + glm::vec3(x, 0.2f, 0.62f));
            M = glm::scale(M, glm::vec3(0.25f, 0.30f, 0.01f));
            glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            x += 0.22f;
        }

        glBindTexture(GL_TEXTURE_2D, texDigits[ones]);
        M = glm::translate(glm::mat4(1), acPos + glm::vec3(x, 0.2f, 0.62f));
        M = glm::scale(M, glm::vec3(0.25f, 0.30f, 0.01f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // °C
        glBindTexture(GL_TEXTURE_2D, texC);
        M = glm::translate(
            glm::mat4(1),
            acPos + glm::vec3(
                x + 0.15f,
                0.2f + 0.12f,
                0.62f
            )
        );
        M = glm::scale(M, glm::vec3(0.12f, 0.16f, 0.01f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        M = glm::translate(glm::mat4(1), acPos + glm::vec3(0.0f, 0.2f, 0.62f));
        M = glm::scale(M, glm::vec3(0.45f, 0.30f, 0.01f));
        glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // crna pozadina
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform4f(overrideColorLoc, 0.0f, 0.0f, 0.0f, 1.0f);

    M = glm::translate(glm::mat4(1), acPos + glm::vec3(0.8f, 0.2f, 0.62f));
    M = glm::scale(M, glm::vec3(0.60f, 0.32f, 0.01f));
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    // treci ekran - ikonica
    if (screensOn)
    {
        unsigned int tex = texOk;
        if (desiredTemp > measuredTemp) tex = texHeat;
        else if (desiredTemp < measuredTemp) tex = texCool;

        glBindTexture(GL_TEXTURE_2D, tex);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    M = glm::translate(glm::mat4(1), acPos + glm::vec3(0.8f, 0.2f, 0.62f));
    M = glm::scale(M, glm::vec3(0.35f, 0.30f, 0.01f));
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    if (prevDepth)
        glEnable(GL_DEPTH_TEST);

}


void renderRoom(unsigned int shader)
{
    bool prevCull = cullFaceOn;
    glDisable(GL_CULL_FACE);

    glUniform1i(glGetUniformLocation(shader, "acOn"), 0);

    float W = 14.0f;
    float H = 6.0f;
    float D = 14.0f;

    glm::mat4 M;

    glUniform1i(glGetUniformLocation(shader, "useTex"), 0);
    glUniform4f(overrideColorLoc, 0.88f, 0.88f, 0.88f, 1.0f);

    // pod
    M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -D / 2));
    M = glm::scale(M, glm::vec3(W, 0.05f, D));
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4f(overrideColorLoc, 0.92f, 0.92f, 0.92f, 1.0f);
    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);

    // plafon
    M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, H - 2.0f, -D / 2));
    M = glm::scale(M, glm::vec3(W, 0.05f, D));
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4f(overrideColorLoc, 0.88f, 0.88f, 0.88f, 1.0f);
    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);

    // zadnji zid 
    M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, H / 2 - 2.0f, -D));
    M = glm::scale(M, glm::vec3(W, H, 0.05f));
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4f(overrideColorLoc, 0.95f, 0.95f, 0.95f, 1.0f);
    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);

    // levi zid 
    M = glm::translate(glm::mat4(1.0f), glm::vec3(-W / 2, H / 2 - 2.0f, -D / 2));
    M = glm::scale(M, glm::vec3(0.05f, H, D));
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4f(overrideColorLoc, 0.90f, 0.90f, 0.90f, 1.0f);
    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);

    // desni zid 
    M = glm::translate(glm::mat4(1.0f), glm::vec3(W / 2, H / 2 -2.0f, -D / 2));
    M = glm::scale(M, glm::vec3(0.05f, H, D));
    glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4f(overrideColorLoc, 0.90f, 0.90f, 0.90f, 1.0f);
    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);

    if (prevCull) glEnable(GL_CULL_FACE);

}

void initScreenQuad()
{
    float quadVertices[] = {
        // pozicija       // textura
        -1.0f, -1.0f,      0.0f, 0.0f,
        -0.6f, -1.0f,      1.0f, 0.0f,
        -0.6f, -0.70f,     1.0f, 1.0f,
        -1.0f, -0.70f,     0.0f, 1.0f,
    };

    glGenVertexArrays(1, &screenVAO);
    glGenBuffers(1, &screenVBO);

    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

void renderSignature(unsigned int overlayShader)
{
    glUseProgram(overlayShader);

    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texCreator);
    glUniform1i(glGetUniformLocation(overlayShader, "uTex"), 0);

    glBindVertexArray(screenVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    if (depthTestOn)
        glEnable(GL_DEPTH_TEST);
}
