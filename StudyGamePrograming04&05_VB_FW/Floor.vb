Imports OpenTK

Public Class Floor
	Inherits Actor
	Sub New(ByRef game As Game)
		MyBase.New(game)
		Dim sc = New SpriteComponent(Me, 100)
		sc.SetTexture(game.GetRenderer.GetTexture("Assets/Floor.png"))
		Dim q = Quaternion.FromAxisAngle(Vector3.UnitX, MathHelper.PiOver2)
		SetRotation(q)
	End Sub
End Class
