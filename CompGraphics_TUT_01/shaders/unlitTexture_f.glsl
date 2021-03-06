// Code from intermediate CG tutorial

#version 420

// Note: Uniform bindings
// This lets you specify the texture unit directly in the shader!
layout(binding = 0) uniform sampler2D u_rgb; // rgb texture

// Fragment Shader Inputs
in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec4 colour;
	vec3 posEye;
} vIn;

// Multiple render targets!
layout(location = 0) out vec4 FragColor;

void main()
{
	vec4 color = texture(u_rgb, vIn.texCoord.xy);
	FragColor = color;
}