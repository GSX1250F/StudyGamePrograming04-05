Imports System.Runtime.InteropServices
Imports System.Threading
Imports OpenTK
Imports OpenTK.Graphics
Imports OpenTK.Graphics.OpenGL
Imports OpenTK.Mathematics
Imports OpenTK.Windowing.Common
Imports OpenTK.Windowing.Desktop
Imports OpenTK.Windowing.GraphicsLibraryFramework


Public Class Game
    Inherits GameWindow

    'public
    Public Sub New(width As Integer, height As Integer, title As String)
        MyBase.New(GameWindowSettings.Default, New NativeWindowSettings())
        MyBase.ClientSize = (width, height)
        MyBase.Title = title
        mWindowWidth = width
        mWindowHeight = height
        mRenderer = Nothing
        mIsRunning = True
        mTicksCount = 0
        mUpdatingActors = False
    End Sub
    Public Function Initialize() As Boolean
        'ストップウォッチ開始
        Ticks = New Stopwatch()
        Ticks.Start()
        mTicksCount = Ticks.ElapsedMilliseconds

        'レンダラー作成
        mRenderer = New Renderer(Me)
        If (mRenderer.Initialize(mWindowWidth, mWindowHeight)) = False Then
            mRenderer.Shutdown()
            Return False
        End If

        LoadData()

        Return True
    End Function

    Public Sub Shutdown()
        UnloadData()
        If mRenderer IsNot Nothing Then
            mRenderer.Shutdown()
        End If
        Ticks.Stop()
        Me.Close()
    End Sub

    Public Function GetRenderer() As Renderer
        Return mRenderer
    End Function

    Public Sub SetRunning(ByVal value As Boolean)
        mIsRunning = value
    End Sub

    Public Sub AddActor(ByRef actor As Actor)
        If mUpdatingActors Then
            mPendingActors.Add(actor)
        Else
            mActors.Add(actor)
        End If
    End Sub

    Public Sub RemoveActor(ByRef actor As Actor)
        '待ちアクターを検索し、消去
        Dim iter As Integer = mPendingActors.IndexOf(actor)
        '見つからなかったら-1が返される。
        If iter >= 0 Then
            mPendingActors.RemoveAt(iter)
        End If
        'アクターを検索し、消去
        iter = mActors.IndexOf(actor)
        If iter >= 0 Then
            mActors.RemoveAt(iter)
        End If
    End Sub

    Public mWindowWidth As Integer
    Public mWindowHeight As Integer

    'Game Specific
    Public Function GetMaze() As Maze
        Return mMaze
    End Function



    'private
    Private Sub ProcessInput()
        Dim keyState As KeyboardState = KeyboardState
        If (keyState.IsKeyDown(Keys.Escape)) Then
            mIsRunning = False
        End If
        mUpdatingActors = True
        For Each actor In mActors
            actor.ProcessInput(keyState)
        Next
        mUpdatingActors = False
    End Sub
    Private Sub UpdateGame()
        ' フレームレート調整（62.5fps)
        If (Ticks.ElapsedMilliseconds - mTicksCount < 16) Then
            Dim sleeptime As Integer = 16 - (Ticks.ElapsedMilliseconds - mTicksCount)
            System.Threading.Thread.Sleep(sleeptime)
        End If

        'デルタタイムの計算
        Dim deltaTime As Single = (Ticks.ElapsedMilliseconds - mTicksCount) / 1000

        'デルタタイムを最大値で制限する
        If deltaTime > 0.05 Then
            deltaTime = 0.05
        End If
        mTicksCount = Ticks.ElapsedMilliseconds

        'すべてのアクターを更新
        mUpdatingActors = True
        For Each actor In mActors
            actor.Update(deltaTime)
        Next
        mUpdatingActors = False

        '待ちアクターをmActorsに移動
        For Each pending In mPendingActors
            pending.ComputeWorldTransform()
            mActors.Add(pending)
        Next
        mPendingActors.Clear()

        '死んだアクターを一時配列に追加
        Dim deadActors As New List(Of Actor)
        For Each actor In mActors
            If actor.GetState() = Actor.State.EDead Then
                deadActors.Add(actor)
            End If
        Next

        '死んだアクターを削除
        For Each actor In deadActors
            actor.Dispose()
        Next
    End Sub
    Private Sub GenerateOutput()
        mRenderer.Draw()
    End Sub

    Private Sub LoadData()
        mMaze = New Maze(Me, 51, 29)        '迷路クラス
        'mMaze = New Maze(Me, 7, 7)        'テスト用
    End Sub

    Private Sub UnloadData()
        While mActors.Count > 0
            mActors.First.Dispose()
        End While
        If mRenderer IsNot Nothing Then
            mRenderer.UnloadData()
        End If
    End Sub

    Private mRenderer As Renderer
    Private Ticks As Stopwatch
    Private mTicksCount As Integer
    Private mIsRunning As Boolean
    Private mUpdatingActors As Boolean      'アクター更新中
    Private mActors As New List(Of Actor)   'すべてのアクター
    Private mPendingActors As New List(Of Actor)    'すべての待ちアクター

    'Game-specific
    Private mMaze As Maze



    'OpenTK original function
    'Protected Overrides Sub OnLoad()        'Loadされたら呼び出される。Initializeと同等？
    '    MyBase.OnLoad()
    'End Sub

    'Protected Overrides Sub OnRenderFrame(e As FrameEventArgs)         'フレーム毎の描画時に呼び出される。
    '    MyBase.OnRenderFrame(e)
    'End Sub

    Protected Overrides Sub OnUpdateFrame(e As FrameEventArgs)
        MyBase.OnUpdateFrame(e)
        '毎フレーム更新時に呼び出される。
        If mIsRunning Then
            ProcessInput()
            UpdateGame()
            GenerateOutput()
        Else
            Shutdown()
        End If
    End Sub

    'Protected Overrides Sub OnResize(e As ResizeEventArgs)          '画面サイズ変更時に呼び出される。
    '    MyBase.OnResize(e)
    'End Sub

    'Protected Overrides Sub OnUnload()
    '    MyBase.OnUnload()
    'End Sub

End Class
