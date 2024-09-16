#include "Shader.h"
#include <SDL.h>
#include <fstream>
#include <sstream>


Shader::Shader()
	: mShaderProgram(0)
	, mVertexShader(0)
	, mFragShader(0)
{}

Shader::~Shader()
{}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// �o�[�e�b�N�X�V�F�[�_�[�ƃt���O�����g�V�F�[�_�[���R���p�C������
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
	{
		return false;
	}

	// �o�[�e�b�N�X,�t���O�����g�V�F�[�_�[�������N����
	// �V�F�[�_�[�v���O���������
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	// �v���O�����������������N���ꂽ���Ƃ��m�F
	if (!IsValidProgram())
	{
		return false;
	}

	return true;

}

void Shader::Unload()
{
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	glUseProgram(mShaderProgram);
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	//�t�@�C�����J��
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		//���ׂẴe�L�X�g��1�̕�����ɓǂݍ���
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// �w�肳�ꂽ�^�C�v�̃V�F�[�_�[���쐬
		outShader = glCreateShader(shaderType);
		// �ǂݍ��񂾕�����ł̃R���p�C�������݂�
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("�V�F�[�_�[ %s �̃R���p�C���Ɏ��s���܂���", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("�V�F�[�_�[�t�@�C�� %s ��������܂���", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// �R���p�C����Ԃ�₢���킹��
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL�̃R���p�C�������s���܂����F\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;
	// �R���p�C����Ԃ�₢���킹��
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;

}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	//name�Ɠ���uniform�ϐ����V�F�[�_�[�v���O��������T���A����ID���󂯎��B
	GLuint uniformId = glGetUniformLocation(mShaderProgram, name);
	//matrix�ŏ㏑��
	glUniformMatrix4fv(
		uniformId,					// uniform�ϐ���ID
		1,							// �s��̐�
		GL_TRUE,					// �s�x�N�g����TRUE
		matrix.GetAsFloatPtr()		// �㏑������s��̃|�C���^
	);
}