#include "VertexInfo.h"
#include <glew.h>

VertexInfo::VertexInfo(const float* vertPos, const float* texCoord,
	const float* vertColor, const unsigned int* indices,
	unsigned int numVerts, unsigned int numIndices)
	: mNumVerts(numVerts), mNumIndices(numIndices)
{
	// バーテックス配列オブジェクトをOpenGLに生成し、そのIDをメンバー変数mVertexArrayに保存する
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// インデックスバッファをOpenGLに生成し、そのIDをメンバー変数mIndexBufferに保存する
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,			// インデックスバッファの指定
		mNumIndices * sizeof(unsigned int),	// データのサイズ
		indices,							// コピー元の配列
		GL_STATIC_DRAW						// このデータを1回だけロードしてその後頻繁に使うとき
	);

	//VertexAttribute layout0 = position
	unsigned int cnt = 3;					//要素数	
	glGenBuffers(1, &mVertPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertPosBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,							// バッファの種類にバーテックスバッファを指定
		cnt * mNumVerts * sizeof(float),		// コピーするバイト数
		vertPos,									// コピー元の配列
		GL_STATIC_DRAW								// このデータを1回だけロードしてその後頻繁に使うとき
	);
	glVertexAttribPointer(
		0,									// 属性インデックス（１つ目はゼロ）
		cnt,									// 要素数
		GL_FLOAT,							// 要素の型
		GL_FALSE,							// 整数型のみ使用する。
		cnt * sizeof(float),					// ストライド（通常は各バーテックス属性のデータ数
		0									// 頂点データの開始位置からこの属性までのオフセット
	);
	glEnableVertexAttribArray(0);

	// VertexAttribute layout1 = texCoord
	cnt = 2;		//テクスチャ座標配列の要素数
	glGenBuffers(1, &mTexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,					// バッファの種類にバーテックスバッファを指定
		cnt * mNumVerts * sizeof(float),	//コピーするバイト数
		texCoord,							//コピー元の配列
		GL_STATIC_DRAW						//このデータを1回だけロードしてその後頻繁に使うとき
	);
	glVertexAttribPointer(
		1,									// 属性インデックス（１つ目はゼロ）
		cnt,								// 要素数
		GL_FLOAT,							// 要素の型
		GL_FALSE,							// 整数型のみ使用する。
		cnt * sizeof(float),				// ストライド（通常は各バーテックス属性のデータ数
		0									// 頂点データの開始位置からこの属性までのオフセット
	);
	glEnableVertexAttribArray(1);

	// VertexAttribute layout2 = vertColor
	cnt = 4;								//頂点カラー配列の要素数
	glGenBuffers(1, &mVertColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertColorBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,					// バッファの種類にバーテックスバッファを指定
		cnt * mNumVerts * sizeof(float),		//コピーするバイト数
		vertColor,							//コピー元の配列
		GL_STATIC_DRAW						//このデータを1回だけロードしてその後頻繁に使うとき
	);
	glVertexAttribPointer(
		2,									// 属性インデックス（１つ目はゼロ）
		cnt,									// 要素数
		GL_FLOAT,							// 要素の型
		GL_FALSE,							// 整数型のみ使用する。
		cnt * sizeof(float),					// ストライド（通常は各バーテックス属性のデータ数
		0									// 頂点データの開始位置からこの属性までのオフセット
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