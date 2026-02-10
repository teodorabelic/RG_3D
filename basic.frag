#version 330 core

in vec4 vColor;
in vec2 vTex;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

uniform bool acOn;
uniform vec4 overrideColor;

uniform vec3 lightPos = vec3(2.0, 4.0, 2.0);
uniform vec3 lightColor = vec3(1.0);

uniform vec3 viewPos;

uniform sampler2D uTex;
uniform bool useTex;

void main()
{
    vec4 baseColor;

    // =========================
    // IZBOR BOJE / TEKSTURE
    // =========================
    if (useTex)
    {
        baseColor = texture(uTex, vTex);

        // PNG providnost (ikonice)
        if (baseColor.a < 0.1)
            discard;
    }
    else
    {
        baseColor = (overrideColor.a > 0.0)
            ? overrideColor
            : vColor;
    }

    // =========================
    // OSVETLJENJE
    // =========================
    vec3 color = baseColor.rgb;

    if (!useTex) // ?? teksture NE osvetljavamo (ekrani svetle sami)
    {
        vec3 norm = normalize(vNormal);
        vec3 lightDir = normalize(lightPos - vFragPos);

        float diff = max(dot(norm, lightDir), 0.0);

        vec3 viewDir = normalize(viewPos - vFragPos);
        vec3 reflectDir = reflect(-lightDir, norm);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
        vec3 specular = 0.08 * spec * lightColor;

        vec3 ambient = 0.18 * lightColor;
        vec3 diffuse = 0.75 * diff * lightColor;

        color = baseColor.rgb * (ambient + diffuse) + specular;


        // samo sijalica reaguje na acOn
        bool isLamp =
            baseColor.r < 0.3 &&
            baseColor.g < 0.3 &&
            baseColor.b < 0.3;

        if (acOn && isLamp)
            color += vec3(0.8, 0.0, 0.0);
    }

    FragColor = vec4(color, baseColor.a);
}
