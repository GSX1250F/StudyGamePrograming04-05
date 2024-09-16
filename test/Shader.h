#pragma once
#include <string>
#include <glew.h>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	//指定された名前のバーテックス,フラグメントシェーダーを読み込む
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	//アクティブなシェーダープログラムとして設定
	void SetActive();
	//uniform変数に代入する関数
	void SetMatrixUniform(const char* name, const Matrix4& matrix);

private:
	//シェーダーをコンパイルする
	bool CompileShader(const std::string& fileName,	GLenum shaderType, GLuint& outShader);
	//シェーダーのコンパイルに成功したかの判定
	bool IsCompiled(GLuint shader);
	//バーテックス,フラグメントプログラムのリンクを確認
	bool IsValidProgram();
	//シェーダーオブジェクトのIDを格納
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};