Public Class Tile
    Inherits Actor

    Public Enum TileState
        EDefault
        EWall
        EStart
        EGoal
    End Enum

    Sub New(ByRef game As Game)
        MyBase.New(game)
        mTileState = TileState.EDefault
        mParent = Nothing
        'スプライトコンポーネントを作成
        sc = New SpriteComponent(Me, 10)
        sc.SetTexture(game.GetRenderer().GetTexture("Assets/Wall.png"))
        mTexSize = sc.GetTexWidth()

        'CircleComponent作成
        cc = New CircleComponent(Me, 100)
    End Sub
    Public Overrides Sub UpdateActor(deltaTime As Double)
        UpdateTexture()
    End Sub
    Public Sub UpdateTexture()
        Select Case mTileState
            Case TileState.EWall
                sc.SetVisible(True)
            Case Else
                sc.SetVisible(False)
        End Select
    End Sub
    Public Function GetSprite() As SpriteComponent
        Return sc
    End Function
    Public Function GetCircle() As CircleComponent
        Return cc
    End Function
    Public Function GetTileState() As TileState
        Return mTileState
    End Function
    Public Function GetTexSize() As Double
        Return mTexSize
    End Function
    Public Sub SetTileState(ByVal state As TileState)
        mTileState = state
        UpdateTexture()
    End Sub
    Public Sub SetAdjacent(ByRef tile As Tile)
        mAdjacent.Add(tile)
    End Sub
    Public Function GetAdjacent() As List(Of Tile)
        Return mAdjacent
    End Function
    Public Sub SetParent(ByRef tile As Tile)
        mParent = tile
    End Sub
    Public Function GetParent() As Tile
        Return mParent
    End Function
    Public Sub ClearAdjacent()
        mAdjacent.Clear()
    End Sub

    Private mAdjacent As New List(Of Tile)
    Private mParent As Tile
    Private sc As SpriteComponent
    Private cc As CircleComponent
    Private mTileState As TileState
    Private mTexSize As Double
End Class
