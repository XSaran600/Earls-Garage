#version  420

in VertexData
{
	vec3 normals;
	vec2 texCoord;
} vIn;

out vec4 outColor;

void main(){
	outColor = vec4(vIn.normals,1.0f);


}