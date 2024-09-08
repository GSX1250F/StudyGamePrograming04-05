#pragma once
class VertexArray
{
public:
	VertexArray(const float* verts, unsigned int numVerts,
				const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// このバーテックス配列をアクティブにする（描画できるようにする）
	void SetActive();

	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	// バーテックスバッファにあるバーテックス属性の数
	unsigned int mNumVerts;
	// インデックスバッファにあるインデックスの数
	unsigned int mNumIndices;
	// バーテックスバッファのOpenGL ID
	unsigned int mVertexBuffer;
	// インデックスバッファのOpenGL ID
	unsigned int mIndexBuffer;
	// バーテックス配列オブジェクトのOpenGL ID
	unsigned int mVertexArray;
};