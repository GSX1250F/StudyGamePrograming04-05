// GLSL3.3を要求
#version 330

// カラーバッファへの出力色
out vec4 outColor;

// テクスチャ座標をバーテックスシェーダーから受け取る
in vec2 fragTexCoord;

// テクスチャ座標にしたがってテクスチャから色を取得するサンプリング用変数
uniform sampler2D uTexture;

void main()
{
	// テクスチャから色をサンプリングする
	outColor = texture(uTexture, fragTexCoord);
}