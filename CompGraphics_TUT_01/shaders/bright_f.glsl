#version 420

layout(binding = 0) uniform sampler2D u_scene; // original scene image

uniform float u_bloomThreshold;

// Fragment Shader Inputs
in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec4 colour;
	vec3 posEye;
} vIn;

layout(location = 0) out vec4 FragColor;

vec4 PixelShader(vec2 texCoord, float BloomThreshold, sampler2D TextureSampler)
{
	vec4 c = texture(TextureSampler, texCoord);

	return clamp((c - BloomThreshold) / (1 - BloomThreshold), 0, 1);
}
void main()
{
	//////////////////////////////////////////////////////////////////////////
	// IMPLEMENT BRIGHT PASS HERE
	// - See pseudo code in lab document
	////////////////////////////////////////////////////////////////////////// 

	vec4 brightPass;
	brightPass = PixelShader(vIn.texCoord.xy, u_bloomThreshold, u_scene);

	FragColor = brightPass;
}