#version 330

// “ü—Í•Ï”‚ÌéŒ¾
layout (location = 0) in vec3 inVertPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inVertColor;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

out vec2 fragTexCoord;
out vec4 fragVertColor;

void main()
{
	vec4 pos = vec4(inVertPos, 1.0);
	gl_Position = pos * uWorldTransform * uViewProj;
	fragTexCoord = inTexCoord;
	fragVertColor = inVertColor;
}