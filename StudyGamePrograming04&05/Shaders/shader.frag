#version 330

out vec4 outColor;

in vec2 fragTexCoord;
in vec4 fragVertColor;

uniform sampler2D uTexture;

void main()
{
	outColor = texture(uTexture, fragTexCoord);	
}