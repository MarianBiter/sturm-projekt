#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 color;
uniform vec2 offset;
uniform mat4 perspectiveMatrix;
uniform mat4 viewMatrix;
out vec4 theColor;
void main()
{
	vec4 aux = vec4(offset.x,offset.y,0.0,0.0);
    vec4 newPos = vPosition + aux;
	gl_Position = perspectiveMatrix * viewMatrix * newPos;
	theColor = color;
}