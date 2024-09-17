Imports System.IO
Imports OpenTK
Imports OpenTK.Graphics
Imports OpenTK.Graphics.OpenGL
Imports StbImageSharp

Public Class Texture
    Implements IDisposable      '明示的にクラスを開放するために必要
    Private disposedValue As Boolean
    Sub New()
        mTextureID = 0
        mTexWidth = 0
        mTexHeight = 0
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

    Public Function Load(ByRef filename As String) As Boolean
        StbImage.stbi_set_flip_vertically_on_load(1)
        Dim image As ImageResult = ImageResult.FromStream(File.OpenRead(filename), ColorComponents.RedGreenBlueAlpha)
        mTexWidth = image.Width
        mTexHeight = image.Height

        'テクスチャをOpenGLに生成し、そのIDをメンバ変数mTextureIDに保存する。
        GL.GenTextures(1, mTextureID)
        GL.BindTexture(TextureTarget.Texture2D, mTextureID)
        GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, image.Width, image.Height, 0, PixelFormat.Rgba, PixelType.UnsignedByte, image.Data)
        'OpenGLに登録が完了したら画像データを開放する。
        image = Nothing
        'バイリニアフィルタリングを有効化
        GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, Int(TextureMinFilter.Linear))
        GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, Int(TextureMagFilter.Linear))

        Return True
    End Function
    Public Sub Unload()
        GL.DeleteTextures(1, mTextureID)
    End Sub
    Public Sub SetActive()
        GL.BindTexture(TextureTarget.Texture2D, mTextureID)
    End Sub
    Public Function GetTexWidth() As Integer
        Return mTexWidth
    End Function
    Public Function GetTexHeight() As Integer
        Return mTexHeight
    End Function
    Public Function GetTextureID() As UInteger
        Return mTextureID
    End Function

    Private mTextureID As UInteger
    Private mTexWidth As Integer
    Private mTexHeight As Integer
End Class
