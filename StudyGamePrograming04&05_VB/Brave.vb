Imports System.Runtime.CompilerServices
Imports OpenTK.Mathematics
Imports OpenTK.Windowing.GraphicsLibraryFramework

Public Class Brave
	Inherits Actor
	Sub New(ByRef game As Game)
		MyBase.New(game)
		speed = 1000.0
		SetScale(0.85)

		sc = New SpriteComponent(Me, 100)
		sc.SetTexture(game.GetRenderer.GetTexture("Assets/ClearPict.png"))
		sc.SetVisible(False)

		'CircleComponent作成
		cc = New CircleComponent(Me, 100)

		'MoveComponent作成
		mc = New MoveComponent(Me, 10)
	End Sub
	Public Overrides Sub ActorInput(ByVal keyState As KeyboardState)
		If (GetGame().GetMaze().GetGameStart()) Then
			Dim forwardSpeed As Double = 0.0
			Dim angularSpeed As Double = 0.0
			If (keyState.IsKeyDown(Keys.Up)) Then
				forwardSpeed = speed
			ElseIf (keyState.IsKeyDown(Keys.Down)) Then
				forwardSpeed = -speed
			ElseIf (keyState.IsKeyDown(Keys.Left)) Then
				angularSpeed = -Math.PI
			ElseIf (keyState.IsKeyDown(Keys.Right)) Then
				angularSpeed = Math.PI
			End If
			mc.SetVelocity(forwardSpeed * GetForward())
			mc.SetRotSpeed(angularSpeed)
		End If
	End Sub
	Public Overrides Sub UpdateActor(ByVal deltaTime As Double)
		If (GetGame().GetMaze().GetGameStart()) Then
			If (Intersect(cc, GetGame().GetMaze().GetTreasure().GetCircle())) Then
				' ゴール
				GetGame().GetMaze().SetGameClear(True)
				sc.SetVisible(True)
			End If
		End If

		For Each tilecol In GetGame().GetMaze().GetTiles()
			For Each tile In tilecol
				If (tile.GetTileState() = Tile.TileState.EWall) Then
					'壁に衝突していたら離す。
					If (Intersect(cc, tile.GetCircle())) Then
						'２つの中心を結ぶ線分上で、２つの半径だけ離す
						Dim diff As Vector3 = Vector3.Normalize(Me.GetPosition() - tile.GetPosition())
						Me.SetPosition(tile.GetPosition() + diff * (Me.GetRadius() + tile.GetRadius()))
					End If
				End If
			Next
		Next

		Dim cameraPos As Vector3 = GetPosition()
		Dim target As Vector3 = GetPosition() + GetForward() * 300.0
		Dim up As Vector3 = -Vector3.UnitZ
		Dim view = Matrix4.LookAt(cameraPos, target, up)
		GetGame().GetRenderer().SetViewMatrix(view)


	End Sub

	Public Function GetSprite() As SpriteComponent
		Return sc
	End Function
	Public Function GetCircle() As CircleComponent
		Return cc
	End Function
	Public Function GetSpeed() As Double
		Return speed
	End Function

	Private sc As SpriteComponent
	Private cc As CircleComponent
	Private mc As MoveComponent
	Private speed As Double
End Class
