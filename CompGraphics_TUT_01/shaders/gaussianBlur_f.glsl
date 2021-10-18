#version 420

layout(binding = 0) uniform sampler2D u_bright; // bright pass image

// (1.0 / windowWidth, 1.0 / windowHeight)
uniform vec4 u_texelSize; // Remember to set this!

// Fragment Shader Inputs
in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec4 colour;
	vec3 posEye;
} vIn;

layout(location = 0) out vec4 FragColor;

vec3 boxBlur(sampler2D tex)
{
	vec2 uv = (vIn.texCoord).xy;

	vec2 offsetCoordinates[9];

	// Left column
	offsetCoordinates[0] = vec2(-u_texelSize.x, u_texelSize.y) + uv;	// top left
	offsetCoordinates[1] = vec2(-u_texelSize.x, 0.0) + uv;				// middle left
	offsetCoordinates[2] = vec2(-u_texelSize.x, -u_texelSize.y) + uv;	// bottom left

	// Mid column
	offsetCoordinates[3] = vec2(-u_texelSize.x, u_texelSize.y) + uv;	// top middle
	offsetCoordinates[4] = uv;											// center
	offsetCoordinates[5] = vec2(0.0, -u_texelSize.y) + uv;				// bottom middle

	// right column
	offsetCoordinates[6] = vec2(u_texelSize.x, u_texelSize.y) + uv;		// top right
	offsetCoordinates[7] = vec2(u_texelSize.x, 0.0f) + uv;				// middle right
	offsetCoordinates[8] = vec2(u_texelSize.x, -u_texelSize.y) + uv;	// bottom right

	vec3 blurred = vec3(0.0);

	// Add all the neighbouring pixels
	blurred += texture(tex, offsetCoordinates[0]).rgb * 0.08;
	blurred += texture(tex, offsetCoordinates[1]).rgb * 0.12;
	blurred += texture(tex, offsetCoordinates[2]).rgb * 0.08;
													  
	blurred += texture(tex, offsetCoordinates[3]).rgb * 0.12;
	blurred += texture(tex, offsetCoordinates[4]).rgb * 0.2;
	blurred += texture(tex, offsetCoordinates[5]).rgb * 0.12;
													  
	blurred += texture(tex, offsetCoordinates[6]).rgb * 0.08;
	blurred += texture(tex, offsetCoordinates[7]).rgb * 0.12;
	blurred += texture(tex, offsetCoordinates[8]).rgb * 0.08;

	// Divide by the number of kernels 28 48 19
	// In this case it's 9
	//blurred /= 9.0;

	return blurred;
}

void main()
{
	//////////////////////////////////////////////////////////////////////////
	// IMPLEMENT 2D GAUSSIAN BLUR HERE
	// - Exact same convolution process as a box blur the only difference 
	//	is that you just need to scale each sample by a weight.
	////////////////////////////////////////////////////////////////////////// 
	vec3 blurred = boxBlur(u_bright);
	FragColor = vec4(blurred, 1.0);
}