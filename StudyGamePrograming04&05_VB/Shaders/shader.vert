#version 330 core

// ���͕ϐ��̐錾
layout (location = 0) in vec3 inVertPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inVertColor;

// uniform�ϐ�
uniform mat4 uWorldTransform;	//���[���h�ϊ��s��
uniform mat4 uViewProj;		//�r���[�ˉe�ϊ��s��

// �o��
out vec2 fragTexCoord;		//�e�N�X�`�����W
out vec4 fragVertColor;		//���_�J���[

void main()
{
	// ���[���h�ϊ��ƃr���[�ˉe�ϊ������s�B
	gl_Position = vec4(inVertPos, 1.0) * uWorldTransform * uViewProj;
	fragTexCoord = inTexCoord;
	fragVertColor = inVertColor;
}