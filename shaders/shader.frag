#version 450

layout(location = 0) in vec3 fragWorldPos;
layout(location = 1) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
    vec3 lightDir;
    vec4 lightColor;
    vec3 eyePos;
} gubo;

void main() {
    vec3 albedo = vec3(1.0, 0.5, 0.0);  // base orange color

    vec3 N = normalize(fragNormal);
    vec3 L = normalize(-gubo.lightDir);              // toward the light
    vec3 V = normalize(gubo.eyePos - fragWorldPos);  // toward the camera
    vec3 H = normalize(L + V);                       // halfway vector

    // Diffuse term: bright on surfaces facing the light
    vec3 diffuse = albedo * max(dot(N, L), 0.0);

    // Specular term: shiny highlight where reflection points at camera
    float specStrength = pow(max(dot(N, H), 0.0), 32.0);
    vec3 specular = vec3(0.4) * specStrength;

    // Ambient: dim baseline so shadows aren't pitch black
    vec3 ambient = albedo * 0.15;

    vec3 result = (diffuse + specular) * gubo.lightColor.rgb + ambient;
    outColor = vec4(result, 1.0);
}