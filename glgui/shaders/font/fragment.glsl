#version 330 core

layout (location = 0) out vec4 outcolor;

float glyphy_sdf (const vec2 p, const ivec2 nominal_size);

uniform samplerBuffer sdftex;

in vec2 fCoord;
flat in ivec2 fNominalSize;
flat in int fBufferOffset;

vec4 glyphy_texture1D_func (in int offset)
{
    return texelFetch (sdftex, fBufferOffset + offset);
}

layout (std140) uniform Uniforms {
    mat4 mat;
    vec4 color;
    float contrast;
    float boldness;
};

void main (void)
{
    vec2 dpdx = dFdx (fCoord);
    vec2 dpdy = dFdy (fCoord);
    float m = 0.70710678118654757 * length (vec2 (length (dpdx), length (dpdy)));

    float sdist = glyphy_sdf (fCoord, fNominalSize) / m * contrast;
    sdist -= boldness;
    if (sdist > 1.0) discard;
    outcolor = color;
    outcolor.a *= smoothstep (-.75, +.75, -sdist);
}
