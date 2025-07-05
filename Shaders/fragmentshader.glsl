#version 330 core

in vec2 outTexCoord;
in vec3 fragNormal;

out vec4 outColor;

uniform sampler2D modelTexture;
uniform bool useTexture;
uniform bool useNormal;

uniform vec3 lightDirection = vec3(0.5, 0.5, 1.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 objectColor = vec3(0.8, 0.5, 0.3);

void main()
{
    vec3 color = objectColor;

    if (useTexture) {
        color = texture(modelTexture, outTexCoord).rgb;
    }

    if (useNormal) {
        vec3 norm = normalize(fragNormal);
        vec3 lightDir = normalize(-lightDirection);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        color = diffuse * color;
    }

    outColor = vec4(color, 1.0);
}
