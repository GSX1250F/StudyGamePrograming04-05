#version 330

in vec2 inVertPos;

void main()
{
	gl_Position = vec4(inVertPos, 0.0, 1.0);
}