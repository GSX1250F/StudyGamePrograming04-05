// OpenGL3.3に対応するGLSLバージョンの指定

#version 330

// 入力変数の宣言（バーテックス座標とテクスチャ座標）
layout(location=0) in vec3 inPosition;
layout(location=1) in vec2 inTexCoord;

// uniform変数の宣言(const変数と意味は近い？)
uniform mat4 uWorldTransform;	//ワールド変換行列
uniform mat4 uViewProj;		//ビュー射影変換行列

// 出力にテクスチャ座標を追加
out vec2 fragTexCoord;

void main()
{
	// 3次元の入力変数を4次元に変換する
	vec4 pos = vec4(inPosition, 1.0);

	// ワールド変換とビュー射影変換を実行。
	// 行ベクトルの場合は、元の行ベクトル×ワールド変換行列×ビュー射影変換行列
	gl_Position = pos * uWorldTransform * uViewProj;

	// テクスチャ座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
}