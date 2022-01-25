#version 330 core

layout(location=0) in vec3 aPosition;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec3 Direction;

void main()
{
	gl_Position = u_Projection * u_View * vec4(aPosition, 1.0);
	Direction = aPosition;
}
