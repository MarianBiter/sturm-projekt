#version 430 core

out vec4 fColor;

void main()
{
	float interpolValueY =  gl_FragCoord.y/650.0;
	float interpolValueX =  gl_FragCoord.x/650.0;
	vec4 redcolor = mix(vec4(0.0,0.0,0.0,1.0),vec4(1.0,0.0,0.0,1.0),interpolValueX);
	vec4 bluecolor = mix(vec4(0.0,0.0,0.0,1.0),vec4(0.0,0.0,1.0,1.0),interpolValueY);
	fColor = redcolor + bluecolor;
}