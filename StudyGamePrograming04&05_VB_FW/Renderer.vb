Imports OpenTK
Imports OpenTK.Graphics
Imports OpenTK.Graphics.OpenGL

Public Class Renderer
	Implements IDisposable      '明示的にクラスを開放するために必要
	Private disposedValue As Boolean
	Private mGame As Game
	Private mWindow As GLControl
	Private mScreenWidth As Integer
	Private mScreenHeight As Integer
	Private mTextures As New Dictionary(Of String, Texture)   'ファイル名→OpenGLのテクスチャのインデックスの連想配列
	Private mSprites As New List(Of SpriteComponent)    'スプライトコンポーネントの配列

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
		' 画面初期化
		GL.Viewport(0, 0, screenWidth, screenHeight)

		mView = Matrix4.LookAt(Vector3.Zero, Vector3.UnitX, Vector3.UnitZ)
		mProjection = Matrix4.CreatePerspectiveFieldOfView(MathHelper.Pi * 0.5, mScreenWidth / mScreenHeight, 0.01, 5000.0)

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
		'画面のクリア
		GL.ClearColor(0.3, 0.3, 0.3, 1.0)
		GL.Clear(ClearBufferMask.ColorBufferBit Or ClearBufferMask.DepthBufferBit)
		GL.Enable(EnableCap.DepthTest)
		'アルファブレンディング
		GL.Enable(EnableCap.Blend)
		GL.BlendFunc(BlendingFactor.SrcAlpha, BlendingFactor.OneMinusSrcAlpha)

		'ビュー＆射影変換行列を作成
		mViewProj = mView * mProjection

		'すべてのスプライトコンポーネントを描画
		For Each sprite In mSprites
			If sprite.GetVisible() = True Then
				sprite.Draw(mViewProj)
			End If
		Next
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
		mProjection = matrix
	End Sub

	Private mView As Matrix4
	Private mProjection As Matrix4
	Private mViewProj As Matrix4
End Class
