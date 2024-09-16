Imports OpenTK.Mathematics

Public Class CircleComponent
    Inherits Component

    Sub New(ByRef owner As Actor, ByVal updateOrder As Integer)
        MyBase.New(owner, updateOrder)
    End Sub
    Public Function GetRadius() As Double
        Return mOwner.GetRadius()
    End Function

    Public Function GetCenter() As Vector3
        Return mOwner.GetPosition()
    End Function

End Class
Public Module CircleModule
    Public Function Intersect(ByRef a As CircleComponent, ByRef b As CircleComponent) As Boolean
        ' ２つのCircleComponentの中心間距離を計算
        Dim distSq As Double = (a.GetCenter() - b.GetCenter()).LengthSquared

        ' ２つのCircleComponentの半径の和を計算 
        Dim sumRadiusSq As Double = (a.GetRadius() + b.GetRadius()) * (a.GetRadius() + b.GetRadius())

        ' 中心間距離 <= 半径の和 のとき、衝突したと判定
        If distSq <= sumRadiusSq Then
            Return True
        Else
            Return False
        End If
    End Function
End Module
