#version 330 core

in vec3 Direction;
out vec4 FragColor;

uniform samplerCube u_Texture;

void main()
{
	FragColor = texture(u_Texture, Direction);
}
