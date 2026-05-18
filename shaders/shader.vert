#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 fragWorldPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragUV;

layout(set = 1, binding = 0) uniform UniformBufferObject {
    mat4 mvpMat;
    mat4 modelMat;
    mat4 normalMat;
} ubo;

void main() {
    gl_Position  = ubo.mvpMat * vec4(inPos, 1.0);
    fragWorldPos = vec3(ubo.modelMat * vec4(inPos, 1.0));
    fragNormal   = mat3(ubo.normalMat) * inNormal;
    fragUV       = inUV;
}