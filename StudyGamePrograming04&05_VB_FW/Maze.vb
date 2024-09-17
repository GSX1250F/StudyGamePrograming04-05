Imports OpenTK

Public Class Maze
    Inherits Actor
    Sub New(ByRef game As Game, ByVal mapWidth As Integer, ByVal mapHeight As Integer)
        MyBase.New(game)
        mMapWidth = mapWidth
        mMapHeight = mapHeight
        gameStart = False
        gameClear = False
        resetStart = True
        resetPending = False
        resetEnd = False
        '横幅、縦幅をともに7以上の奇数にする。
        While (mMapWidth < 7 Or mMapWidth Mod 2 = 0)
            mMapWidth += 1
        End While
        While (mMapHeight < 7 Or mMapHeight Mod 2 = 0)
            mMapHeight += 1
        End While
        'Tileをインスタンス, mTiles配列とmapIndex配列を同じ要素数にする

        For i As Integer = 0 To mMapWidth - 1
            Dim tiles As New List(Of Tile)
            Dim indice As New List(Of Integer)
            For j As Integer = 0 To mMapHeight - 1
                Dim tile As New Tile(game)
                tiles.Add(tile)
                indice.Add(0)
            Next
            mTiles.Add(tiles)
            mMapIndex.Add(indice)
        Next
        mTileSize = mTiles(0)(0).GetTexSize()
        mBrave = New Brave(game)            'プレイヤー
        mTreasure = New Treasure(game)      'ゴール
    End Sub
    Public Overrides Sub ActorInput(ByVal keyState As Boolean())
        If (GetGame().GetRKeyUpEvent()) Then resetStart = True
    End Sub
    Public Overrides Sub UpdateActor(ByVal deltaTime As Double)
        If (gameClear = True) Then
        End If
        If (resetStart = True) Then
            mBrave.SetState(State.EPaused)
            mBrave.GetSprite().SetVisible(False)
            mTreasure.SetState(State.EPaused)
            mTreasure.GetSprite().SetVisible(False)
            For i As Integer = 0 To mTiles.Count - 1
                For j As Integer = 0 To mTiles(i).Count - 1
                    mTiles(i)(j).SetState(State.EPaused)
                    mTiles(i)(j).GetSprite().SetVisible(False)
                    mTiles(i)(j).ClearAdjacent()
                    mTiles(i)(j).SetParent(Nothing)
                    mMapIndex(i)(j) = 0
                Next
            Next
            resetStart = False
            resetPending = True
            gameStart = False
            gameClear = False
        ElseIf (resetPending = True) Then
            GenerateMap()
            resetPending = False
            resetEnd = True
        ElseIf (resetEnd = True) Then
            mBrave.SetState(State.EActive)
            'mBrave.GetSprite().SetVisible(True)
            mBrave.SetPosition(GetTilePos(starti, startj))
            mTreasure.SetState(State.EActive)
            mTreasure.GetSprite().SetVisible(True)
            mTreasure.SetPosition(GetTilePos(goali, goalj))
            For Each ctiles In mTiles
                For Each tile In ctiles
                    If tile.GetTileState = Tile.TileState.EWall Then
                        tile.SetState(State.EActive)
                        tile.GetSprite().SetVisible(True)
                    End If
                Next
            Next
            '隣接ノード作成
            MakeGraphNodes(mTiles)
            '道順探索 (逆順)
            If (FindPath(GetGoalTile(), GetStartTile())) Then
            End If
            resetEnd = False
            gameStart = True
        End If
    End Sub
    Public Function GetGameStart() As Integer
        Return gameStart
    End Function
    Public Sub SetGameClear(ByVal value As Integer)
        gameClear = value
    End Sub
    Public Sub GenerateMap()
        '迷路作成
        Dim mazeNG As Boolean = True
        While (mazeNG)
            mMapIndex = MazeCreate(mMapWidth, mMapHeight)
            For i As Integer = 0 To mMapWidth - 1
                For j As Integer = 0 To mMapHeight - 1
                    Select Case mMapIndex(i)(j)
                        Case 2      'スタート
                            starti = i
                            startj = j
                        Case 3      'ゴール
                            goali = i
                            goalj = j
                    End Select
                Next
            Next
            '簡単すぎないかチェック
            If ((goali > Int(mMapWidth / 2)) And (goalj > Int(mMapHeight / 2))) Then mazeNG = False
        End While
        For i As Integer = 0 To mMapWidth - 1
            For j As Integer = 0 To mMapHeight - 1
                Select Case mMapIndex(i)(j)
                    Case 0      '通路
                        mTiles(i)(j).SetTileState(Tile.TileState.EDefault)
                    Case 1      '壁
                        mTiles(i)(j).SetTileState(Tile.TileState.EWall)
                    Case 2      'スタート
                        mTiles(i)(j).SetTileState(Tile.TileState.EStart)
                    Case 3      'ゴール
                        mTiles(i)(j).SetTileState(Tile.TileState.EGoal)
                End Select
                mTiles(i)(j).SetPosition(GetTilePos(i, j))
            Next
        Next
    End Sub
    Public Function GetTilePos(ByVal i As Integer, ByVal j As Integer) As Vector3
        Dim pos As Vector2 = (New Vector2((i + 1) * 1.0, (j + 1) * 1.0) + New Vector2(-mMapWidth / 2 - 1, -mMapHeight / 2 - 1)) * mTileSize
        Return New Vector3(pos.X, pos.Y, 0.0)
    End Function
    Public Function GetStartTile() As Tile
        Return mTiles(starti)(startj)
    End Function
    Public Function GetGoalTile() As Tile
        Return mTiles(goali)(goalj)
    End Function
    Public Function GetTiles() As List(Of List(Of Tile))
        Return mTiles
    End Function
    Public Function GetTreasure() As Treasure
        Return mTreasure
    End Function


    Private gameStart, gameClear, resetStart, resetPending, resetEnd As Boolean
    Private mMapIndex As New List(Of List(Of Integer))
    Private mMapWidth, mMapHeight, starti, startj, goali, goalj As Integer
    Private mTileSize As Double
    Private mTile As Tile
    Private mTiles As New List(Of List(Of Tile))
    Private mBrave As Brave
    Private mTreasure As Treasure
    Private Sub MakeGraphNodes(ByRef tiles As List(Of List(Of Tile)))
        For i As Integer = 0 To tiles.Count - 1
            For j As Integer = 0 To tiles(i).Count - 1
                If (tiles(i)(j).GetTileState() <> Tile.TileState.EWall) Then
                    If (i > 0) Then
                        If (tiles(i - 1)(j).GetTileState() <> Tile.TileState.EWall) Then
                            tiles(i)(j).SetAdjacent(tiles(i - 1)(j))
                        End If
                    End If
                    If (i + 1 <= tiles.Count - 1) Then
                        If (tiles(i + 1)(j).GetTileState() <> Tile.TileState.EWall) Then
                            tiles(i)(j).SetAdjacent(tiles(i + 1)(j))
                        End If
                    End If
                    If (j > 0) Then
                        If (tiles(i)(j - 1).GetTileState() <> Tile.TileState.EWall) Then
                            tiles(i)(j).SetAdjacent(tiles(i)(j - 1))
                        End If
                    End If
                    If (j + 1 <= tiles(i).Count - 1) Then
                        If (tiles(i)(j + 1).GetTileState() <> Tile.TileState.EWall) Then
                            tiles(i)(j).SetAdjacent(tiles(i)(j + 1))
                        End If
                    End If
                End If
            Next
        Next
    End Sub
    Private Function FindPath(ByRef start As Tile, ByRef goal As Tile)
        '幅優先経路探索（BFS）
        Dim outMap As New Dictionary(Of Tile, Tile)         'ノードから親へのマップ
        Dim pathFound As Boolean = False        '経路を見つけたか？
        Dim q As New Queue(Of Tile)     '検討するノード
        '最初のノードをキューにいれる。
        q.Enqueue(start)
        While (q.Count > 0)
            'ノードを１つキューから出す
            Dim current As Tile = q.Dequeue()
            If (current Is goal) Then
                pathFound = True
                Exit While
            End If
            'まだキューに入っていない隣接ノードをエンキューする。
            For Each node As Tile In current.GetAdjacent()
                '検討中のノードの隣接ノードのすべてについて親があるかを調べる。
                'outMapは、ノードと親の関係マップ
                'もし親がnullptrなら、まだキューに追加されていない。(ただし開始ノードは例外)
                If (outMap.ContainsKey(key:=node) = False And node IsNot start) Then
                    'このノードのエンキューと親の設定をする。
                    outMap.Add(node, current)
                    node.SetParent(current)
                    q.Enqueue(node)
                End If
            Next
        End While
        Return pathFound
    End Function
End Class
