#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 fragWorldPos;
layout(location = 1) out vec3 fragNormal;

layout(set = 1, binding = 0) uniform UniformBufferObject {
    mat4 mvpMat;
    mat4 modelMat;   // NEW: needed to transform position to world space
    mat4 normalMat;  // NEW: needed to transform normals correctly
} ubo;

void main() {
    gl_Position  = ubo.mvpMat * vec4(inPos, 1.0);
    fragWorldPos = vec3(ubo.modelMat * vec4(inPos, 1.0));
    fragNormal   = mat3(ubo.normalMat) * inNormal;
}