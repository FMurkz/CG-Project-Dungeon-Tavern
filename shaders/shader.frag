#version 450

layout(location = 0) in vec3 fragWorldPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D albedoTex;
layout(set = 0, binding = 0) uniform GUBO {
    vec3 lightDir;
    vec4 lightColor;
    vec3 viewerPos;
} gubo;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(-gubo.lightDir);
    vec3 V = normalize(gubo.viewerPos - fragWorldPos);
    vec3 H = normalize(L + V);            // Blinn-Phong halfway vector

    vec3 albedo = texture(albedoTex, fragUV).rgb;

    vec3 diffuse  = albedo * max(dot(N, L), 0.0);
    vec3 specular = vec3(pow(max(dot(N, H), 0.0), 32.0));
    vec3 ambient  = albedo * 0.1;

    outColor = vec4((diffuse + specular) * gubo.lightColor.rgb + ambient, 1.0);
}