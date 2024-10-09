#version 460 core

layout(location = 0) out vec4 color;
layout(location = 1) in vec2 fragPosition;

uniform int resolution;
uniform bool drawing;
uniform vec4 drawColor;
uniform sampler2D inputTexture;
uniform vec2 to;
uniform vec2 from;

const float PI = 3.14159265;
const float TAU = 2.0 * PI;
const int rayCount = 4;
const int maxSteps = 400;

bool outOfBounds(vec2 uv) {
    return uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0;
}

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

// Draw a line shape!
float sdfLineSquared(vec2 p, vec2 _from, vec2 _to) {
    vec2 toStart = p - from;
    vec2 line = to - from;
    float lineLengthSquared = dot(line, line);
    float t = clamp(dot(toStart, line) / lineLengthSquared, 0.0, 1.0);
    vec2 closestVector = toStart - line * t;
    return dot(closestVector, closestVector);
}

vec4 raymarch(vec2 vUv) {
    vec4 light = texture(inputTexture, vUv);

    if (light.a > 0.1) {
        return light;
    }

    float oneOverRayCount = 1.0 / float(rayCount);
    float tauOverRayCount = TAU * oneOverRayCount;

    //Distinct random value for every pixel
    float noise = rand(vUv);

    vec4 radiance = vec4(0.0);

    for (int i = 0; i < rayCount; i++) {
        float angle = tauOverRayCount * (float(i) + noise);
        vec2 rayDirectionUv = vec2(cos(angle), -sin(angle)) / resolution;

        // Our current position, plus one step.
        vec2 sampleUv = vUv + rayDirectionUv;

        for (int step = 0; step < maxSteps; step++) {
            if (outOfBounds(sampleUv)) break;

            vec4 sampleLight = texture(inputTexture, sampleUv);
            if (sampleLight.a > 0.1) {
                radiance += sampleLight;
                break;
            }

            sampleUv += rayDirectionUv;
        }
    }

    return radiance * oneOverRayCount;
}

void main()
{
    float radiusSquared = 16;
    vec2 vUv = (fragPosition + 1.0) / 2.0;
    vec4 current = texture(inputTexture, vUv);

    if (drawing) {
        vec2 coord = vUv * resolution;
        if (sdfLineSquared(coord, from, to) <= radiusSquared) {
            current = vec4(drawColor.rgb, 1.0);
        }
        color = current;
    } else {
        color = vec4(raymarch(vUv).rgb, 1.0);
    }
}
