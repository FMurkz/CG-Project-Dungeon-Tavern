#version 450

// Input from the vertex buffer (matches your Vertex struct in C++)
layout(location = 0) in vec3 inPos;

// Per-object uniforms (matches UniformBufferObject struct)
layout(set = 1, binding = 0) uniform UniformBufferObject {
    mat4 mvpMat;
} ubo;

void main() {
    gl_Position = ubo.mvpMat * vec4(inPos, 1.0);
}