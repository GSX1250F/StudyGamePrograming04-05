Imports OpenTK
Imports OpenTK.Graphics
Imports OpenTK.Graphics.OpenGL
Imports OpenTK.Mathematics

Public Class SpriteComponent
    Inherits Component

    Sub New(ByRef owner As Actor, ByVal drawOrder As Integer)
        MyBase.New(owner, drawOrder)
        mTexture = Nothing
        mDrawOrder = drawOrder
        mTexWidth = 0
        mTexHeight = 0
        mVisible = True
        mOwner.GetGame().GetRenderer().AddSprite(Me)
    End Sub
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        If Not Me.disposed Then
            If disposing Then
                '*** アンマネージリソースの開放
            End If
            '*** マネージドリソースの開放
            mOwner.GetGame().GetRenderer().RemoveSprite(Me)
        End If
        MyBase.Dispose(disposing)
    End Sub

    Public Overridable Sub Draw(ByRef shader As Shader)
        If (mTexture IsNot Nothing) And (mVisible = True) Then
            ' テクスチャサイズで再スケーリングしたワールド変換行列を作成
            Dim world = Matrix4.CreateScale(mTexWidth, mTexHeight, mTexWidth)
            world *= mOwner.GetWorldTransform()
            ' ワールド変換
            shader.SetMatrixUniform("uWorldTransform", world)

            ' 現在のテクスチャをセット
            mTexture.SetActive()
            ' 短形を描画
            GL.DrawElements(PrimitiveType.Triangles, 24, DrawElementsType.UnsignedInt, 0)
        End If

    End Sub

    Public Function GetDrawOrder() As Integer
        Return mDrawOrder
    End Function
    Public Function GetTexWidth() As Integer
        Return mTexWidth
    End Function
    Public Function GetTexHeight() As Integer
        Return mTexHeight
    End Function
    Public Function GetTexture() As Texture
        Return mTexture
    End Function
    Public Sub SetTexture(ByRef tex As Texture)
        mTexture = tex
        mTexWidth = tex.GetTexWidth()
        mTexHeight = tex.GetTexHeight()
        ' 高さと幅の平均をActorの直径とする。
        mOwner.SetRadius((mTexWidth + mTexHeight) / 4)
    End Sub
    Public Sub SetVisible(ByVal visible As Boolean)
        mVisible = visible
    End Sub
    Public Function GetVisible() As Boolean
        Return mVisible
    End Function

    'private
    Private mTexture As Texture
    Private mDrawOrder As Integer = 100
    Private mTexWidth As Integer
    Private mTexHeight As Integer
    Private mVisible As Boolean

End Class
