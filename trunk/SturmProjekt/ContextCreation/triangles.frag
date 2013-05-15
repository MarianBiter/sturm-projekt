#version 430 core

out vec4 fColor;

void main()
{
	float interpolValue =  gl_FragCoord.y/650.0;

	fColor = mix(vec4(0.0,0.0,1.0,1.0),vec4(1.0,0.0,0.0,1.0),interpolValue);
}