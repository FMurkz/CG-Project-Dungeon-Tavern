#version 450

// Inputs from vertex buffer (match Vertex struct)
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

// Outputs to fragment shader
layout(location = 0) out vec3 fragWorldPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragUV;

// Uniforms
layout(binding = 0) uniform UBO { mat4 mvp; mat4 model; } ubo;

void main() {
    gl_Position = ubo.mvp * vec4(inPos, 1.0);
    fragWorldPos = vec3(ubo.model * vec4(inPos, 1.0));
    fragNormal = mat3(ubo.model) * inNormal;
    fragUV = inUV;
}