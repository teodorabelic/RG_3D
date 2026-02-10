#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

out vec2 TexCoord;

void main()
{
    TexCoord = aTex;
    gl_Position = uP * uV * uM * vec4(aPos, 1.0);
}
