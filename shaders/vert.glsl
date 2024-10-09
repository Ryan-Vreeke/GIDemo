#version 460 core
layout(location = 0) in vec2 pos;
layout(location = 1) out vec2 fragPosition;

void main() {
    gl_Position = vec4(pos, 0, 1);
    fragPosition = pos;
}
