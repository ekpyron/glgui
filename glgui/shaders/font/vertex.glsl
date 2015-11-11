#version 330 core

layout (location = 0) in ivec2 vCorner;
layout (location = 1) in vec4 vPositions;
layout (location = 2) in vec4 vColor;
layout (location = 3) in ivec3 vInfo;

out gl_PerVertex {
    vec4 gl_Position;
};

flat out ivec2 fNominalSize;
out vec2 fCoord;
flat out int fBufferOffset;
out vec4 fColor;

uniform mat4 mat;

void main (void)
{
	gl_Position = mat * vec4 (vPositions.xy + vCorner * vPositions.zw, 0, 1);
	fNominalSize = vInfo.yz;
	fBufferOffset = vInfo.x;
	fCoord = vec2 (vCorner * vInfo.yz);
	fColor = vColor;
}
