#version 420

// Remember to bind the two textures to different texture units!
layout(binding = 0) uniform sampler2D u_bright; // bright pass image
layout(binding = 1) uniform sampler2D u_scene; // original scene image


// Fragment Shader Inputs
in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec4 colour;
	vec3 posEye;
} vIn;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 FragColor2;


void main()
{
	//////////////////////////////////////////////////////////////////////////
	// COMPOSITE BLOOM HERE
	// - Sample from the two textures and add the colors together
	////////////////////////////////////////////////////////////////////////// 
	vec4 brightPass = texture(u_bright, vIn.texCoord.xy);
	vec4 scene = texture(u_scene, vIn.texCoord.xy);

	FragColor = brightPass + scene;
}