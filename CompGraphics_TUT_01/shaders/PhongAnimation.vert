#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform float T;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

layout(location = 3) in vec3 in_vert2;
layout(location = 4) in vec3 in_normal2;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

out vec3 new_in_vert;
out vec3 new_in_normal;

vec3 calculateLerp(vec3 v1, vec3 v2, float time)
{
	return (1 - time) * v1 + v2 * time;
}

void main(){
	//Lerp
	new_in_vert = calculateLerp(in_vert, in_vert2, T);
	new_in_normal = calculateLerp(in_normal, in_normal2, T);
	normalize(new_in_normal);

	texCoord = in_uv;
	normal = mat3(uView) * mat3(uModel) * new_in_normal;

	vec4 viewSpacePos = uView * uModel * vec4(new_in_vert, 1.0f);
	position = viewSpacePos.xyz;
	gl_Position = uProj * viewSpacePos;
}