// OpenGL3.3に対応するGLSLバージョンの指定

#version 330

// 入力変数（３次元）の位置情報の宣言
in vec3 inPosition;

void main()
{
	// inPositionをgl_Positionに、そのまま渡す
	gl_Position = vec4(inPosition, 1.0);
}