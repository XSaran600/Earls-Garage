#version 420

// Remember to bind the two textures to different texture units!
layout(binding = 0) uniform sampler2D u_bloom; // bloom scene image
layout(binding = 1) uniform sampler2D u_scene; // non bloom scene image
layout(binding = 2) uniform sampler2D u_blurredBrightPass; // Blurred brightPass

layout(binding = 3) uniform sampler2D u_depthScene; // depth for non bloom image
layout(binding = 4) uniform sampler2D u_depthBloom; // depth for bloom image


// Fragment Shader Inputs
in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec4 colour;
	vec3 posEye;
} vIn;

layout(location = 0) out vec4 FragColor;

void main()
{
	vec4 bloom = texture(u_bloom, vIn.texCoord.xy);
	vec4 scene = texture(u_scene, vIn.texCoord.xy);

	float depthScene = texture(u_depthScene, vIn.texCoord.xy).x;
	float depthBloom = texture(u_depthBloom, vIn.texCoord.xy).x;

	float blurredBrightPass = texture(u_blurredBrightPass, vIn.texCoord.xy).x;

	if(depthScene > depthBloom) // If bloom object is infront of non-bloom object
	{
		FragColor = bloom;
	}
	else // If non-bloom object is infront of bloom object
	{
		FragColor = scene + blurredBrightPass;
	}

}