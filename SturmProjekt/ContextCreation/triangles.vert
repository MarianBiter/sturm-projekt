#version 430 core

layout (location = 0) in vec4 vPosition;
uniform vec2 offset;
void main()
{
	vec4 aux = vec4(offset.x,offset.y,0.0,0.0);
    gl_Position = vPosition + aux;
}