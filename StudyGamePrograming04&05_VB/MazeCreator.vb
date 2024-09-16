Imports System.Numerics
Imports System.Security.Cryptography

Public Module MazeCreator
    Private Enum Direction
        up = 0
        right = 1
        down = 2
        left = 3
    End Enum

    Private mWidth, mHeight, digCount As Integer
    Private mCell, startCell As Vector2
    Private startCells As New List(Of Vector2)
    Private maze As New List(Of List(Of Integer))
    Private Const start As Integer = 2
    Private Const goal As Integer = 3
    Private Const path As Integer = 0
    Private Const wall As Integer = 1

    Public Function MazeCreate(ByRef width As Integer, ByRef height As Integer) As List(Of List(Of Integer))
        maze.Clear()
        '横幅、縦幅をともに奇数にする。
        If (width Mod 2 = 0) Then width += 1
        If (height Mod 2 = 0) Then height += 1
        '迷路情報を初期化
        mWidth = width
        mHeight = height
        '迷路情報を配列化
        For i As Integer = 0 To mWidth - 1
            Dim indice As New List(Of Integer)
            For j As Integer = 0 To mHeight - 1
                indice.Add(1)
            Next j
            maze.Add(indice)
        Next i
        '5未満のサイズや偶数では生成できない
        If (width >= 5 And height >= 5) Then
            '全てを壁で埋める
            '穴掘り開始候補(x,yともに偶数)座標を保持しておく
            For y As Integer = 0 To height - 1
                For x As Integer = 0 To width - 1
                    If (x = 0 Or y = 0 Or x = width - 1 Or y = height - 1) Then
                        maze(x)(y) = path  '外壁は判定の為通路にしておく(最後に戻す)
                    Else
                        maze(x)(y) = wall
                    End If
                Next
            Next
            'スタート位置をランダムに設定。できるだけ左上にしておく。最初のCellがスタート位置とする。
            '1,3,5の奇数にする
            startCell.X = RandomNumberGenerator.GetInt32(3) * 2 + 1
            startCell.Y = RandomNumberGenerator.GetInt32(3) * 2 + 1
            mCell = startCell
            '穴掘り開始
            digCount = 0
            Dig(mCell)
            '外壁を戻す
            For y As Integer = 0 To mHeight - 1
                For x As Integer = 0 To mWidth - 1
                    If (x = 0 Or y = 0 Or x = mWidth - 1 Or y = mHeight - 1) Then
                        maze(x)(y) = wall
                    End If
                Next
            Next
        Else
            '全部通路の情報を返す（迷路にならない）スタートは左上、ゴールは右下
            For i As Integer = 0 To mWidth - 1
                For j As Integer = 0 To mHeight - 1
                    maze(i)(j) = path
                Next
            Next
            maze(0)(0) = start
            maze(mWidth - 1)(mHeight - 1) = goal
        End If
        Return maze
    End Function

    '座標(x, y)に穴を掘る
    Private Sub Dig(ByVal Cell As Vector2)
        '指定座標から掘れなくなるまで堀り続ける
        While (True)
            '掘り進めることができる方向のリストを作成
            Dim directions As New List(Of Direction)
            Dim x As Integer = Int(Cell.X)
            Dim y As Integer = Int(Cell.Y)
            If (y >= 2) Then
                If (maze(Cell.X)(Cell.Y - 1) = wall And maze(Cell.X)(Cell.Y - 2) = wall) Then directions.Add(Direction.up)
            End If
            If (x + 2 <= mWidth - 1) Then
                If (maze(Cell.X + 1)(Cell.Y) = wall And maze(Cell.X + 2)(Cell.Y) = wall) Then directions.Add(Direction.right)
            End If
            If (y + 2 <= mHeight - 1) Then
                If (maze(Cell.X)(Cell.Y + 1) = wall And maze(Cell.X)(Cell.Y + 2) = wall) Then directions.Add(Direction.down)
            End If
            If (x >= 2) Then
                If (maze(Cell.X - 1)(Cell.Y) = wall And maze(Cell.X - 2)(Cell.Y) = wall) Then directions.Add(Direction.left)
            End If
            '掘り進められない場合、ループを抜ける。掘り進めが１回目ならそこをゴールとする。
            If (directions.Count = 0) Then
                If (digCount = 0) Then maze(x)(y) = goal
                digCount += 1
                Exit While
            Else
                '指定座標を通路とし穴掘り候補座標から削除
                SetPath(Cell)
                '掘り進められる場合はランダムに方向を決めて掘り進める
                Dim dirIndex As Integer = RandomNumberGenerator.GetInt32(directions.Count())
                '決まった方向に先2マス分を通路とする
                Select Case directions(dirIndex)
                    Case Direction.up
                        Cell.Y -= 1
                        SetPath(Cell)
                        Cell.Y -= 1
                        SetPath(Cell)
                    Case Direction.right
                        Cell.X += 1
                        SetPath(Cell)
                        Cell.X += 1
                        SetPath(Cell)
                    Case Direction.down
                        Cell.Y += 1
                        SetPath(Cell)
                        Cell.Y += 1
                        SetPath(Cell)
                    Case Direction.left
                        Cell.X -= 1
                        SetPath(Cell)
                        Cell.X -= 1
                        SetPath(Cell)
                End Select
            End If
            directions.Clear()
        End While
        'どこにも掘り進められない場合、穴掘り開始候補座標から掘りなおし
        '候補座標が存在しないとき、穴掘り完了
        Dim flag As Boolean = GetStartCell()
        If flag Then
            Dig(mCell)
        Else
            'start位置を設定
            maze(Int(startCell.X))(Int(startCell.Y)) = start
        End If
    End Sub

    '座標を通路とする(穴掘り開始座標候補の場合は保持)
    Private Sub SetPath(ByVal Cell As Vector2)
        maze(Int(Cell.X))(Int(Cell.Y)) = path
        If ((Int(Cell.X) Mod 2 = 1) And (Int(Cell.Y) Mod 2 = 1)) Then
            '穴掘り候補座標
            startCells.Add(Cell)
        End If
    End Sub

    Private Function GetStartCell() As Boolean
        If (startCells.Count() = 0) Then
            Return False
        Else
            'ランダムに開始座標を取得する
            Dim Index As Integer = RandomNumberGenerator.GetInt32(0, startCells.Count())
            mCell = startCells(Index)
            startCells.RemoveAt(Index)
            Return True
        End If
    End Function

End Module