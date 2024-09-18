Public Class Treasure
	Inherits Actor
	Sub New(ByRef game As Game)
		MyBase.New(game)
		'スプライトコンポーネント作成、テクスチャ設定
		sc = New SpriteComponent(Me, 20)
		sc.SetTexture(game.GetRenderer().GetTexture("Assets/Treasure.png"))

		'CircleComponent作成
		cc = New CircleComponent(Me, 100)
	End Sub
	Public Function GetSprite() As SpriteComponent
		Return sc
	End Function
	Public Function GetCircle() As CircleComponent
		Return cc
	End Function

	Private sc As SpriteComponent
	Private cc As CircleComponent
End Class
