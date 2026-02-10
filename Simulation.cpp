#include "Globals.h"
#include "Simulation.h"

void updateWater(float dt) {
    if (state == AppState::AC_On_Filling && acOn) {
        waterLevel += 0.1f * dt;
        if (waterLevel > 1.5f) {
            waterLevel = 1.5f;
            acOn = false;
            state = AppState::AC_Off_Full;
        }
    }
}

void updateDrops(float dt)
{
    static float dropTimer = 0.0f;
    dropTimer += dt;

    // spawn
    if (state == AppState::AC_On_Filling && acOn && dropTimer > 0.2f)
    {
        dropTimer = 0.0f;

        Drop d;
        d.p = acPos + glm::vec3(0.0f, -0.35f, 0.2f);
        d.v = glm::vec3(0.0f, -1.2f, 0.0f);
        d.alive = true;
        drops.push_back(d);
    }

    // update
    for (auto& d : drops)
    {
        if (!d.alive) continue;
        d.v.y -= 9.81f * dt;
        d.p += d.v * dt;

        if (d.p.y < bowlPos.y + 0.2f)
            d.alive = false;
    }

    // cleanup
    drops.erase(
        std::remove_if(drops.begin(), drops.end(),
            [](const Drop& d) { return !d.alive; }),
        drops.end()
    );
}

void updateTemperature(float dt)
{
    if (!acOn)
        return;

    tempTimer += dt;

    if (tempTimer >= 1.0f)
    {
        tempTimer = 0.0f;

        if (measuredTemp < desiredTemp)
            measuredTemp++;
        else if (measuredTemp > desiredTemp)
            measuredTemp--;
    }
}


void updateFlap(float dt) {
    if (acOn)
        flapAngle = std::min(flapAngle + flapSpeed * dt, flapMax);
    else
        flapAngle = std::max(flapAngle - flapSpeed * dt, 0.0f);
}
