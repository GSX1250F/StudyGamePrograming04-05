#version 330 core

// 入力変数の宣言
layout (location = 0) in vec3 inVertPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inVertColor;

// uniform変数
uniform mat4 uWorldTransform;	//ワールド変換行列
uniform mat4 uViewProj;		//ビュー射影変換行列

// 出力
out vec2 fragTexCoord;		//テクスチャ座標
out vec4 fragVertColor;		//頂点カラー

void main()
{
	// ワールド変換とビュー射影変換を実行。
	gl_Position = vec4(inVertPos, 1.0) * uWorldTransform * uViewProj;
	fragTexCoord = inTexCoord;
	fragVertColor = inVertColor;
}