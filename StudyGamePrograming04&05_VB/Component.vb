Imports OpenTK.Windowing.GraphicsLibraryFramework

Public Class Component
    Implements IDisposable

    Protected mOwner As Actor      '所有アクター
    Protected mUpdateOrder As Integer      'コンポーネントの更新順序

    Sub New(ByRef owner As Actor, ByVal updateOrder As Integer)
        mOwner = owner
        mUpdateOrder = updateOrder      'updateOrderが小さいコンポーネントほど早く更新される
        mOwner.AddComponent(Me)
    End Sub

    Protected disposed = False
    Public Overloads Sub Dispose() Implements IDisposable.Dispose
        Dispose(True)
    End Sub
    Protected Overridable Overloads Sub Dispose(ByVal disposing As Boolean)
        If Not disposed Then
            If disposing Then
                '*** アンマネージリソースの開放

            End If
            '*** マネージドリソースの開放
            mOwner.RemoveComponent(Me)
        End If
        disposed = True
    End Sub

    Protected Overrides Sub Finalize()
        MyBase.Finalize()
    End Sub

    Public Overridable Sub Update(deltaTime As Double)

    End Sub

    Public Overridable Sub ProcessInput(ByVal keyState As KeyboardState)

    End Sub

    Public Function GetUpdateOrder() As Integer
        Return mUpdateOrder
    End Function

End Class
