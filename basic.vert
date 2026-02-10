#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec4 vColor;
out vec2 vTex;
out vec3 vNormal;
out vec3 vFragPos;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

void main()
{
    vec4 worldPos = uM * vec4(aPos, 1.0);
    vFragPos = worldPos.xyz;
    vNormal = mat3(transpose(inverse(uM))) * aNormal;
    vColor = aColor;
    vTex = aTex;
    gl_Position = uP * uV * worldPos;
}
