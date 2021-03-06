#version 330 core

layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTextureCoordinates;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 FragmentPosition;
out vec3 Normal;
out vec2 TextureCoordinates;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPosition, 1.0);
	FragmentPosition = vec3(u_Model * vec4(aPosition, 1.0));
	Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	TextureCoordinates = aTextureCoordinates;
}
