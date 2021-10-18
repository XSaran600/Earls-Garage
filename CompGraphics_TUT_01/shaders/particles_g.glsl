// Code from intermediate CG tutorial
// Geometry shader which turns points into billboarded quads

#version 400

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 u_mvp;
uniform mat4 u_mv;
uniform mat4 u_proj;

// Input from Vertex shader
in VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec4 colour;
	vec3 posEye;
} vIn[]; // array size = num vertices in primitve

// Output from geometry shader
out VertexData
{
	vec3 normal;
	vec3 texCoord;
	vec4 colour;
	vec3 posEye;
} vOut; // array size depends on max_vertices


// Creates a quad of specified size around point p
void PointToQuadBillboarded(vec4 p, float size)
{
	//////////////////////////////////////////////////////////////////////////
	//	TODO: WATCH VIDEO ON PARTICLE EMITTERS
	//////////////////////////////////////////////////////////////////////////
	float halfSize = size * 0.5;

	// Put vertex into eye space
	vec4 pEye = u_mv * p;

	// Points
	// Shift points in eye space to get billboard effect
	vec4 topLeft = vec4(pEye.xy + vec2(-halfSize, halfSize), pEye.z, 1.0);
	vec4 bottomLeft = vec4(pEye.xy + vec2(-halfSize, -halfSize), pEye.z, 1.0);
	vec4 topRight = vec4(pEye.xy + vec2(halfSize, halfSize), pEye.z, 1.0);
	vec4 bottomRight = vec4(pEye.xy + vec2(halfSize, -halfSize), pEye.z, 1.0);

	gl_Position = u_proj * bottomLeft;
	vOut.texCoord.xy = vec2(0.0, 0.0);
	vOut.colour = vIn[0].colour;
	EmitVertex();

	gl_Position = u_proj * topLeft;
	vOut.texCoord.xy = vec2(0.0, 1.0);
	vOut.colour = vIn[0].colour;
	EmitVertex();

	gl_Position = u_proj * bottomRight;
	vOut.texCoord.xy = vec2(1.0, 0.0);
	vOut.colour = vIn[0].colour;
	EmitVertex();

	gl_Position = u_proj * topRight;
	vOut.texCoord.xy = vec2(1.0, 1.0);
	vOut.colour = vIn[0].colour;
	EmitVertex();

	EndPrimitive();
}

void main()
{
	//PointToQuadBillboarded(gl_in[0].gl_Position, (3.0 - vIn[0].colour.x));
	PointToQuadBillboarded(gl_in[0].gl_Position, 0.5f);

}




