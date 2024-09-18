Imports System.IO
Imports OpenTK
Imports OpenTK.Graphics
Imports OpenTK.Graphics.OpenGL
Imports StbImageSharp

Public Class Texture
    Implements IDisposable      '明示的にクラスを開放するために必要
    Private disposedValue As Boolean
    Sub New()
        mTextureID = Nothing
        mTexWidth = Nothing
        mTexHeight = Nothing
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
        If System.IO.File.Exists(filename) Then
            StbImage.stbi_set_flip_vertically_on_load(1)
            mTextureID = GL.GenTexture()
            GL.BindTexture(TextureTarget.Texture2D, mTextureID)
            Using stream As Stream = File.OpenRead(filename)
                Dim image As ImageResult = ImageResult.FromStream(stream, ColorComponents.RedGreenBlueAlpha)
                GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, image.Width, image.Height, 0, PixelFormat.Rgba, PixelType.UnsignedByte, image.Data)
                mTexWidth = image.Width
                mTexHeight = image.Height
            End Using

            'バイリニアフィルタリングを有効化
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, Int(TextureMinFilter.Linear))
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, Int(TextureMagFilter.Linear))
            Return True
        Else
            Return False
        End If
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
