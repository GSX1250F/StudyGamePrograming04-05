Imports OpenTK
Imports OpenTK.Graphics
Imports OpenTK.Graphics.OpenGL
Public Class Renderer
	Implements IDisposable      '明示的にクラスを開放するために必要
	'public
	Sub New(ByRef game As Game)
		mGame = game
		mWindow = Nothing
	End Sub
	Protected disposed = False     '開放処理が実施済みかのフラグ
	Public Overloads Sub Dispose() Implements IDisposable.Dispose
		Dispose(True)
	End Sub
	Protected Overridable Overloads Sub Dispose(ByVal disposing As Boolean)
		If Not Me.disposed Then
			If disposing Then
				'*** アンマネージリソースの開放
			End If
			'*** マネージドリソースの開放
		End If
		disposed = True
	End Sub
	Protected Overrides Sub Finalize()
		MyBase.Finalize()
		Dispose(False)
	End Sub
	Public Function Initialize(ByVal screenWidth As Integer, ByVal screenHeight As Integer) As Boolean
		mScreenWidth = screenWidth
		mScreenHeight = screenHeight
		mWindow = mGame.GetGLControl()

		' 背景色の設定
		GL.ClearColor(0.3, 0.3, 0.3, 1.0)
		' 深度テスト有効化
		GL.Enable(EnableCap.DepthTest)

		'ビュー変換の初期化
		GL.MatrixMode(MatrixMode.Modelview)
		mView = Matrix4.Identity
		GL.LoadMatrix(mView)
		' ビューポート設定
		GL.Viewport(0, 0, mScreenWidth, mScreenHeight)
		'クリッピングウィンドウの設定
		GL.MatrixMode(MatrixMode.Projection)
		mProj = Matrix4.CreatePerspectiveFieldOfView(MathHelper.Pi / 2, mScreenWidth / mScreenHeight, 0.1, 10000)       '透過投影
		GL.LoadMatrix(mProj)
		'マトリックスモードを戻しておく
		GL.MatrixMode(MatrixMode.Modelview)

		Return True
	End Function
	Public Sub Shutdown()
		UnloadData()
		mWindow.Dispose()
		Me.Dispose()
	End Sub
	Public Sub UnloadData()
		mTextures.Clear()
	End Sub
	Public Sub Draw()
		'画面のクリア＆深度バッファクリア
		GL.Clear(ClearBufferMask.ColorBufferBit Or ClearBufferMask.DepthBufferBit)
		'アルファブレンディング
		GL.Enable(EnableCap.Blend)
		GL.BlendFunc(BlendingFactor.SrcAlpha, BlendingFactor.OneMinusSrcAlpha)

		'すべてのスプライトコンポーネントを描画
		For Each sprite In mSprites
			If sprite.GetVisible() = True Then
				sprite.Draw()
			End If
		Next
		'ビュー変換
		GL.MatrixMode(MatrixMode.Modelview)
		GL.LoadMatrix(mView)

		' ビューポート再設定
		GL.Viewport(0, 0, mScreenWidth, mScreenHeight)

		'クリッピングウィンドウの再設定
		GL.MatrixMode(MatrixMode.Projection)
		GL.LoadMatrix(mProj)
		GL.MatrixMode(MatrixMode.Modelview)

		mWindow.SwapBuffers()
		mWindow.Refresh()
	End Sub
	Public Sub AddSprite(ByRef sprite As SpriteComponent)
		Dim myDrawOrder As Integer = sprite.GetDrawOrder()
		Dim cnt As Integer = mSprites.Count     '配列の要素数
		Dim iter As Integer
		If cnt > 0 Then
			For iter = 0 To mSprites.Count - 1
				If myDrawOrder < mSprites(iter).GetDrawOrder() Then
					Exit For
				End If
			Next
		End If
		mSprites.Insert(iter, sprite)
	End Sub
	Public Sub RemoveSprite(ByRef sprite As SpriteComponent)
		Dim iter As Integer = mSprites.IndexOf(sprite)
		'見つからなかったら-1が返される。
		If iter >= 0 Then
			mSprites.RemoveAt(iter)
		End If
	End Sub

	Public Function GetTexture(ByRef filename As String) As Texture
		Dim tex As Texture = Nothing
		'画像ファイルが連想配列になければ追加する。
		Dim b As Boolean = mTextures.ContainsKey(filename)
		If b = True Then
			'すでに読み込み済みなので、そのデータを返す。
			tex = mTextures(filename)
		Else
			'Textureクラスを生成し、連想配列に追加する。
			tex = New Texture
			If (tex.Load(filename)) Then
				mTextures.Add(filename, tex)
			Else
				tex.Dispose()
				tex = Nothing
			End If
		End If
		Return tex
	End Function

	Public Function GetScreenWidth() As Double
		Return mScreenWidth
	End Function
	Public Function GetScreenHeight() As Double
		Return mScreenHeight
	End Function
	Public Sub SetViewMatrix(ByRef matrix As Matrix4)
		mView = matrix
	End Sub
	Public Sub SetProjectionMatrix(ByRef matrix As Matrix4)
		mProj = matrix
	End Sub

	'private
	Private disposedValue As Boolean
	Private mGame As Game
	Private mWindow As GLControl
	Private mScreenWidth As Integer
	Private mScreenHeight As Integer
	Private mTextures As New Dictionary(Of String, Texture)   'ファイル名→OpenGLのテクスチャのインデックスの連想配列
	Private mSprites As New List(Of SpriteComponent)    'スプライトコンポーネントの配列
	Private mView As Matrix4
	Private mProj As Matrix4
End Class
