#include "VertexArray.h"
#include <glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
						 const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts)
	, mNumIndices(numIndices)
{
	// バーテックス配列オブジェクトをOpenGLに生成し、そのIDをメンバー変数mVertexArrayに保存する
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// バーテックスバッファをOpenGLに生成し、そのIDをメンバー変数mVertexBufferに保存する
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,					// バッファの種類にバーテックスバッファを指定
		numVerts * 5 * sizeof(float),		//コピーするバイト数
		verts,								//コピー元の配列
		GL_STATIC_DRAW						//このデータを1回だけロードしてその後頻繁に使うとき
	);

	// インデックスバッファをOpenGLに生成し、そのIDをメンバー変数mIndexBufferに保存する
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,			//インデックスバッファの指定
		numIndices * sizeof(unsigned int),	//データのサイズ
		indices,							//コピー元の配列
		GL_STATIC_DRAW						//このデータを1回だけロードしてその後頻繁に使うとき
	);
	
	// バーテックスバッファのレイアウトを指定する。
	// 属性0はバーテックス座標
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,						// 属性インデックス（１つ目はゼロ）
		3,						// 要素数
		GL_FLOAT,				// 要素の型
		GL_FALSE,				// 整数型のみ使用する。
		sizeof(float) * 5,		// ストライド（通常は各バーテックス属性のデータ数
		0						// 頂点データの開始位置からこの属性までのオフセット
	);
	// 属性1はテクスチャ座標
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,						// 属性インデックス（１つ目はゼロ）
		2,						// 要素数
		GL_FLOAT,				// 要素の型
		GL_FALSE,				// 整数型のみ使用する。
		sizeof(float) * 5,		// ストライド（通常は各バーテックス属性のデータ数
		reinterpret_cast<void*>(sizeof(float) * 3)		// 頂点データの開始位置からこの属性までのオフセット
	);

}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}