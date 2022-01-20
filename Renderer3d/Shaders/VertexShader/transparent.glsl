#version 330 core

layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTextureCoord;

out vec2 TextureCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	TextureCoord = aTextureCoord;
	gl_Position = u_Projection * u_View * u_Model * vec4(aPosition, 1.0);
}
