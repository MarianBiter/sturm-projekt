#version 330 core

layout (location = 0) in vec4 vPosition;
void main()
{
	vec4 scaledPosition = vPosition * vec4(0.5f,0.5f,1.0f,1.0f);
	scaledPosition.w =1.0f;
	gl_Position = scaledPosition;
}