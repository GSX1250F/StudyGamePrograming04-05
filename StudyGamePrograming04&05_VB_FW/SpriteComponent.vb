﻿Imports OpenTK
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

    Public Overridable Sub Draw(ByRef viewProj As Matrix4)
        If (mTexture IsNot Nothing) And (mVisible = True) Then
            Dim vertices As List(Of Vector3) = New List(Of Vector3) From {
                    New Vector3(-0.5, 0.5, 0.5),        '前面左上
                    New Vector3(-0.5, -0.5, 0.5),       '前面左下
                    New Vector3(0.5, -0.5, 0.5),        '前面右下
                    New Vector3(0.5, 0.5, 0.5)}        '前面右上
            'New Vector3(0.5, 0.5, 0.5),         '右面左上
            'New Vector3(0.5, -0.5, 0.5),        '右面左下
            'New Vector3(0.5, -0.5, -0.5),       '右面右下
            'New Vector3(0.5, 0.5, -0.5),        '右面右上
            'New Vector3(0.5, 0.5, -0.5),        '後面左上
            'New Vector3(0.5, 0.5, -0.5),        '後面左下
            'New Vector3(-0.5, -0.5, -0.5),      '後面右下
            'New Vector3(-0.5, -0.5, -0.5),      '後面右上
            'New Vector3(-0.5, 0.5, -0.5),       '左面左上
            'New Vector3(-0.5, -0.5, -0.5),      '左面左下
            'New Vector3(-0.5, -0.5, 0.5),       '左面右下
            'New Vector3(-0.5, 0.5, 0.5)         '左面右上
            '}
            Dim texcoords As List(Of Vector2) = New List(Of Vector2) From {
                    New Vector2(0.0, 1.0),      '左上
                    New Vector2(0.0, 0.0),      '左下
                    New Vector2(1.0, 0.0),      '右下
                    New Vector2(1.0, 1.0)}      '右上
            'New Vector2(0.0, 1.0),      '左上
            'New Vector2(0.0, 0.0),      '左下
            'New Vector2(1.0, 0.0),      '右下
            'New Vector2(1.0, 1.0),      '右上
            'New Vector2(0.0, 1.0),      '左上
            'New Vector2(0.0, 0.0),      '左下
            'New Vector2(1.0, 0.0),      '右下
            'New Vector2(1.0, 1.0),      '右上
            'New Vector2(0.0, 1.0),      '左上
            'New Vector2(0.0, 0.0),      '左下
            'New Vector2(1.0, 0.0),      '右下
            'New Vector2(1.0, 1.0)       '右上
            '}

            ' テクスチャサイズで再スケーリングしたワールド変換行列を作成
            Dim scaleMat As Matrix4 = Matrix4.CreateScale(mTexture.GetTexWidth)
            Dim world As Matrix4 = scaleMat * mOwner.GetWorldTransform()

            ' 現在のテクスチャをセット
            mTexture.SetActive()

            '短形を描画
            GL.Begin(PrimitiveType.Quads)
            ' 各頂点を行列で変換
            For i = 0 To vertices.Count - 1
                Dim v As Vector4 = New Vector4(vertices(i), 1.0)
                'ワールド＆ビュー射影変換
                v *= world * viewProj

                GL.TexCoord2(texcoords(i).X, texcoords(i).Y)
                GL.Vertex3(v.X, v.Y, v.Z)
            Next
            GL.End()

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
