#version 450

layout(location = 0) in vec3 fragWorldPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

struct PointLight {
    vec3 position;
    vec3 color;
};

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
    vec3 lightDir;
    vec4 lightColor;
    vec3 eyePos;
    PointLight torches[4];
    float time;
} gubo;

layout(set = 1, binding = 1) uniform sampler2D diffuseTex;

// ----- Flicker function -----
//Create a pulsing effect from torches
float flicker(float t, float seed) {
    return 1.0
    + 0.10 * sin(t *  7.3 + seed)
    + 0.05 * sin(t * 13.7 + seed * 2.1);
}

void main() {
    vec3 albedo = texture(diffuseTex, fragUV).rgb;

    vec3 N = normalize(fragNormal);
    vec3 L = normalize(-gubo.lightDir);              // toward the light

    vec3 V = normalize(gubo.eyePos - fragWorldPos);  // toward the camera
    vec3 H = normalize(L + V);                       // halfway vector
    //--------Base light ----------
    // Diffuse term: bright on surfaces facing the light
    vec3 diffuse  = albedo * max(dot(N, L), 0.0) * 0.3;

    // Specular term: shiny highlight where reflection points at camera
    float specStrength = pow(max(dot(N, H), 0.0), 32.0);
    vec3 specular = vec3(0.4) * specStrength * 0.01;

    // Ambient: dim baseline so shadows aren't pitch black
    vec3 ambient  = albedo * 0.02;

    vec3 baseColor = (diffuse + specular) * gubo.lightColor.rgb + ambient;

    // ----- Torch point lights -----
    vec3 torchContribution = vec3(0.0);

    for(int i = 0; i< 4; i++){
        vec3 toLight = gubo.torches[i].position - fragWorldPos;
        float distance = length(toLight);
        vec3 L_torch = toLight/distance;

        //lambert diff
        float diff = max(dot(N, L_torch), 0.0);

        //attenuation, tweak these
        float attenuation = 1.0 / (1.0 + 0.1 * distance + 0.05 * distance * distance);

        // Flicker (each torch has different seeds)
        float f = flicker(gubo.time, float(i));

        torchContribution += albedo * gubo.torches[i].color * diff * attenuation * f;



    }
    vec3 result = baseColor + torchContribution;
    outColor = vec4(result, 1.0);
}