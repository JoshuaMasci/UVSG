#version 130

in vec3 out_Color;
in vec3 out_FragPos;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

void main(void) 
{
	float dist = length(out_FragPos) * 3.0;
	float brightness = (1.0 / (dist * dist) - 0.1) * 0.7;
	
	fragmentColor = vec4(out_Color * brightness, 1.0f);	
	//fragmentColor = vec4(out_Color, 1.0f);
}