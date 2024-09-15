#include "VertexInfo.h"
#include <glew.h>

VertexInfo::VertexInfo(const float* vertPos, const float* texCoord, const float* vertColor,	const unsigned int* indices,
					   unsigned int numVerts, unsigned int numIndex)
	: mNumVerts(numVerts)
{
	// �o�[�e�b�N�X�z��I�u�W�F�N�g��OpenGL�ɐ������A����ID�������o�[�ϐ�mVertexArray�ɕۑ�����
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// �C���f�b�N�X�o�b�t�@��OpenGL�ɐ������A����ID�������o�[�ϐ�mIndexBuffer�ɕۑ�����
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,			// �C���f�b�N�X�o�b�t�@�̎w��
		numIndex * sizeof(unsigned int),	// �f�[�^�̃T�C�Y
		indices,							// �R�s�[���̔z��
		GL_STATIC_DRAW						// ���̃f�[�^��1�񂾂����[�h���Ă��̌�p�ɂɎg���Ƃ�
	);

	//VertexAttribute layout0 = position	
	glGenBuffers(1, &mVertPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertPosBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,					// �o�b�t�@�̎�ނɃo�[�e�b�N�X�o�b�t�@���w��
		numVerts * 3 * sizeof(float),		// �R�s�[����o�C�g��
		vertPos,							// �R�s�[���̔z��
		GL_STATIC_DRAW						// ���̃f�[�^��1�񂾂����[�h���Ă��̌�p�ɂɎg���Ƃ�
	);
	glVertexAttribPointer(
		0,									// �����C���f�b�N�X�i�P�ڂ̓[���j
		3,									// �v�f��
		GL_FLOAT,							// �v�f�̌^
		GL_FALSE,							// �����^�̂ݎg�p����B
		3 * sizeof(float),					// �X�g���C�h�i�ʏ�͊e�o�[�e�b�N�X�����̃f�[�^��
		0									// ���_�f�[�^�̊J�n�ʒu���炱�̑����܂ł̃I�t�Z�b�g
	);
	glEnableVertexAttribArray(0);

	// VertexAttribute layout1 = texCoord
	glGenBuffers(1, &mTexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,					// �o�b�t�@�̎�ނɃo�[�e�b�N�X�o�b�t�@���w��
		numVerts * 2 * sizeof(float),		//�R�s�[����o�C�g��
		texCoord,							//�R�s�[���̔z��
		GL_STATIC_DRAW						//���̃f�[�^��1�񂾂����[�h���Ă��̌�p�ɂɎg���Ƃ�
	);
	glVertexAttribPointer(
		1,									// �����C���f�b�N�X�i�P�ڂ̓[���j
		2,									// �v�f��
		GL_FLOAT,							// �v�f�̌^
		GL_FALSE,							// �����^�̂ݎg�p����B
		2 * sizeof(float),					// �X�g���C�h�i�ʏ�͊e�o�[�e�b�N�X�����̃f�[�^��
		0									// ���_�f�[�^�̊J�n�ʒu���炱�̑����܂ł̃I�t�Z�b�g
	);
	glEnableVertexAttribArray(1);

	// VertexAttribute layout2 = vertColor
	glGenBuffers(1, &mVertColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertColorBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,					// �o�b�t�@�̎�ނɃo�[�e�b�N�X�o�b�t�@���w��
		numVerts * 4 * sizeof(float),		//�R�s�[����o�C�g��
		vertColor,							//�R�s�[���̔z��
		GL_STATIC_DRAW						//���̃f�[�^��1�񂾂����[�h���Ă��̌�p�ɂɎg���Ƃ�
	);
	glVertexAttribPointer(
		2,									// �����C���f�b�N�X�i�P�ڂ̓[���j
		4,									// �v�f��
		GL_FLOAT,							// �v�f�̌^
		GL_FALSE,							// �����^�̂ݎg�p����B
		4 * sizeof(float),					// �X�g���C�h�i�ʏ�͊e�o�[�e�b�N�X�����̃f�[�^��
		0									// ���_�f�[�^�̊J�n�ʒu���炱�̑����܂ł̃I�t�Z�b�g
	);
	glEnableVertexAttribArray(2);

}


VertexInfo::~VertexInfo()
{
	glDeleteBuffers(1, &mVertPosBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteBuffers(1, &mTexCoordBuffer);
	glDeleteBuffers(1, &mVertColorBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexInfo::SetActive()
{
	glBindVertexArray(mVertexArray);
}