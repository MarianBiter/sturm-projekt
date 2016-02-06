#version 330 core

out vec4 outputColor;
uniform vec4 mousePosition;

void main()
{
	vec2 mouse = vec2(mousePosition.x, mousePosition.y);
	vec2 pixel = vec2(gl_FragCoord.x,gl_FragCoord.y);

	float dist = distance(mouse,pixel);
	const vec3 red = vec3(1.0f,0.0f,0.0f);
	const vec3 white = vec3(1.0f,1.0f,1.0f);
	vec3 renderColor = vec3(0.0f);
	const float clipDist = 100.0f;
	if (dist <= clipDist)
	{
		renderColor =  red * (1 - dist/clipDist) + white* (dist/clipDist);
		outputColor = vec4(renderColor.x,renderColor.y,renderColor.z,1.0f);
	}
	else
	{
		outputColor =  vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}