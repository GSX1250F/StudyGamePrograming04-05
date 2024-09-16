Public Class SomeSpriteComponent
	Inherits SpriteComponent

	' テクスチャ用の画像ファイルリスト
	Public TextureFiles As List(Of String)

	Sub New(ByRef owner As Actor, ByVal drawOrder As Integer)
		MyBase.New(owner, drawOrder)

	End Sub

	Public Sub SetSomeTextures(ByVal texturefiles As List(Of String))
		For Each file In texturefiles
			SetTexture(mOwner.GetGame().GetRenderer().GetTexture(file))
		Next
	End Sub
	Public Sub SelectTexture(ByVal filename As String)
		SetTexture(mOwner.GetGame().GetRenderer.GetTexture(filename))
	End Sub
End Class